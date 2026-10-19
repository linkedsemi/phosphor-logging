#include "config.h"

#include <phosphor-logging/elog.hpp>

#include <cstdint>
#include <cstdio>
#include <stdexcept>

namespace phosphor
{
namespace logging
{
namespace details
{
using namespace sdbusplus::xyz::openbmc_project::Logging::server;

auto _prepareMsg(const char* funcName)
{
    using phosphor::logging::log;
    constexpr auto IFACE_INTERNAL(
        "xyz.openbmc_project.Logging.Internal.Manager");

    // Transaction id is located at the end of the string separated by a period.

    auto b = sdbusplus::bus::new_default();

    auto m = b.new_method_call(BUSNAME_LOGGING, OBJ_INTERNAL, IFACE_INTERNAL,
                               funcName);
    return m;
}

namespace
{
/* Zephyr: report<>() is reached from D-Bus method handlers and signal callbacks
 * all over the tree, and an exception raised in one of those cannot unwind
 * through dbus-broker's C dispatch frames - it never reaches the thread
 * trampoline in app/src/task_def.cpp, it aborts the whole BMC image. Catch it
 * here instead, so that a failed Commit only loses one entry rather than the
 * whole BMC.
 *
 * The call keeps the sdbusplus/sd-bus default timeout on purpose: a slow
 * logging service then delays this caller instead of dropping the entry early.
 * The trade is that such a caller stays blocked for the full default timeout.
 */

uint32_t commitImpl(const char* name, Entry::Level* level)
{
    const char* const what = level ? "CommitWithLvl" : "Commit";

    try
    {
        auto msg = _prepareMsg(what);
        uint64_t id = sdbusplus::server::transaction::get_id();

        if (level)
        {
            msg.append(id, name, static_cast<uint32_t>(*level));
        }
        else
        {
            msg.append(id, name);
        }

        auto bus = sdbusplus::bus::new_default();
        auto reply = bus.call(msg);

        uint32_t entryID = 0;
        reply.read(entryID);
        return entryID;
    }
    catch (const std::exception& e)
    {
        fprintf(stderr, "phosphor-logging: %s('%s') failed: %s\n", what,
                name ? name : "", e.what());
        return 0;
    }
}
} // namespace

/* Both overloads funnel through here so neither can throw.
 *
 * Returning 0 drops one error-log entry. That is the intended trade: an
 * unrecorded error is recoverable, an aborted BMC is not.
 *
 * NOTE: both commit() entry points must stay inside namespace details -- they
 * are declared there (elog.hpp) and report<T>() calls details::commit(). */
uint32_t commit(const char* name)
{
    return commitImpl(name, nullptr);
}

uint32_t commit(const char* name, Entry::Level level)
{
    return commitImpl(name, &level);
}
} // namespace details

uint32_t commit(std::string&& name)
{
    log<level::ERR>("method is deprecated, use commit() with exception type");
    return phosphor::logging::details::commit(name.c_str());
}

} // namespace logging
} // namespace phosphor
