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
/* Zephyr: upper bound for the synchronous Commit round trip.
 *
 * sdbusplus::bus::call() defaults to 25s, which is fatal here:
 *  - report<>() is called from D-Bus method handlers and signal callbacks all
 *    over the tree, and an exception raised in those cannot unwind through
 *    dbus-broker's C dispatch frames, so it does not even reach the thread
 *    trampoline in app/src/task_def.cpp - it aborts the whole BMC image.
 *  - it is reached through sdbusplus::bus::new_default(), i.e. the
 *    process-wide default bus, which on this platform is owned by whichever
 *    thread created it first. Everyone shares it, so a Commit can sit in a
 *    queue this thread never drains while the broker is busy (a bmcweb page
 *    load is enough) and burn the full timeout.
 */
constexpr uint64_t commitTimeoutUs = 2ULL * 1000 * 1000;

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
        auto reply = bus.call(msg, commitTimeoutUs);

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
