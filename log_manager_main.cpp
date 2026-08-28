#include "config.h"

#include "config_main.h"

#include "extensions.hpp"
#include "log_manager.hpp"

#include <boost/asio/io_context.hpp>
#include <sdbusplus/asio/connection.hpp>
#include <phosphor-logging/lg2.hpp>
#include <sdbusplus/bus.hpp>
#include <sdbusplus/server/manager.hpp>

#include <filesystem>

#ifdef __ZEPHYR__
#include "dbus_broker.h"
#include <zephyr/kernel.h>

extern "C" {
extern struct k_sem logging_ready_sem;
}
#endif

#ifdef __ZEPHYR__
extern "C" int logging_main(void)
#else
extern "C" int logging_main(int /*argc*/, char* /*argv*/[])
#endif
{
    PHOSPHOR_LOG2_USING_WITH_FLAGS;

#ifdef __ZEPHYR__
    sd_bus* busp = nullptr;
    if (connect_to_dbroker(&busp) < 0 || !busp) {
        return -1;
    }
    boost::asio::io_context io;
    auto bus = std::make_shared<sdbusplus::asio::connection>(io, busp);
#else
    boost::asio::io_context io;
    auto bus = std::make_shared<sdbusplus::asio::connection>(io);
#endif

    // Add sdbusplus ObjectManager for the 'root' path of the logging manager.
    sdbusplus::server::manager_t objManager(*bus, OBJ_LOGGING);

    phosphor::logging::internal::Manager iMgr(*bus, OBJ_INTERNAL);

    phosphor::logging::Manager mgr(*bus, OBJ_LOGGING, iMgr);

    // Create a directory to persist errors.
    std::filesystem::create_directories(ERRLOG_PERSIST_PATH);

    // Recreate error d-bus objects from persisted errors.
    iMgr.restore();

    for (auto& startup : phosphor::logging::Extensions::getStartupFunctions())
    {
        try
        {
            startup(iMgr);
        }
        catch (const std::exception& e)
        {
            error("An extension's startup function threw an exception: {ERROR}",
                  "ERROR", e);
        }
    }

    bus->request_name(BUSNAME_LOGGING);

#ifdef __ZEPHYR__
    k_sem_give(&logging_ready_sem);
#endif

    for (;;)
    {
        try
        {
            io.run();
            break;
        }
        catch (const std::exception& e)
        {
            error("log_manager io.run threw an exception: {ERROR}", "ERROR", e);
        }
    }
    return 0;
}
