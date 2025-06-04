#include "config.h"

#include "config_main.h"

#include "extensions.hpp"
#include "log_manager.hpp"

#include <boost/asio/io_context.hpp>
#include <sdbusplus/asio/connection.hpp>
#include <phosphor-logging/lg2.hpp>
#include <sdbusplus/bus.hpp>
#include <sdbusplus/server/manager.hpp>
#include <sdeventplus/event.hpp>

#include <filesystem>

#include <zbusplus.h>
#include <iostream>

MODULE_DEFINE_CHAN_OBSERVER(phosphor_logging);
static sd_bus *logging_sdbus;
int logging_main(int /*argc*/, char* /*argv*/[])
{
    printk("In logging_main\n");
    PHOSPHOR_LOG2_USING_WITH_FLAGS;

    boost::asio::io_context io;

    sd_bus_open_system(&logging_sdbus);

    printk_thread("Start init_zbus_observers ...");
    ZBUS_SET_OBSERVER(logging_sdbus, phosphor_logging);

    printk_thread("request name ...");
    sd_bus_request_name(logging_sdbus, BUSNAME_LOGGING, 0);

    printk_thread("Create asio::connection bus ...");
    sdbusplus::asio::connection bus(io, logging_sdbus);

    // Add sdbusplus ObjectManager for the 'root' path of the logging manager.
    sdbusplus::server::manager_t objManager(bus, OBJ_LOGGING);

    phosphor::logging::internal::Manager iMgr(bus, OBJ_INTERNAL);

    phosphor::logging::Manager mgr(bus, OBJ_LOGGING, iMgr);

    // Create a directory to persist errors.
    // std::error_code ec;
    // printk_thread("Create directory to persist errors: %s", ERRLOG_PERSIST_PATH);
    // std::filesystem::create_directories(ERRLOG_PERSIST_PATH, ec);

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

    printk_thread("Run ...");
    io.run();

    return 0;
}
