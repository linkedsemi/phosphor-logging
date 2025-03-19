#pragma once
#include <limits>
#include <map>
#include <sdbusplus/sdbus.hpp>
#include <sdbusplus/server.hpp>
#include <sdbusplus/utility/dedup_variant.hpp>
#include <string>
#include <systemd/sd-bus.h>

#include <xyz/openbmc_project/Logging/Internal/Manager/common.hpp> 

namespace sdbusplus::server::xyz::openbmc_project::logging::internal
{

class Manager :
    public sdbusplus::common::xyz::openbmc_project::logging::internal::Manager
{
    public:
        /* Define all of the basic class operations:
         *     Not allowed:
         *         - Default constructor to avoid nullptrs.
         *         - Copy operations due to internal unique_ptr.
         *         - Move operations due to 'this' being registered as the
         *           'context' with sdbus.
         *     Allowed:
         *         - Destructor.
         */
        Manager() = delete;
        Manager(const Manager&) = delete;
        Manager& operator=(const Manager&) = delete;
        Manager(Manager&&) = delete;
        Manager& operator=(Manager&&) = delete;
        virtual ~Manager() = default;

        /** @brief Constructor to put object onto bus at a dbus path.
         *  @param[in] bus - Bus to attach to.
         *  @param[in] path - Path to attach at.
         */
        Manager(bus_t& bus, const char* path) :
            _xyz_openbmc_project_logging_internal_Manager_interface(
                bus, path, interface, _vtable, this),
            _intf(bus.getInterface()) {}


        /** @brief Implementation for Commit
         *  Write the requested error/event entry with its associated metadata fields to flash. The "level" of the committed error log is same as the level defined in error YAML definitions.
         *
         *  @param[in] transactionId - The unique identifier of the journal entry(ies) to be committed.
         *  @param[in] errMsg - The error exception message associated with the error event log to be committed.
         *
         *  @return entryID[uint32_t] - The ID of the entry.
         */
        virtual uint32_t commit(
            uint64_t transactionId,
            std::string errMsg) = 0;

        /** @brief Implementation for CommitWithLvl
         *  Write the requested error/event entry with its associated metadata fields to flash. This interface allows the caller to override the error level specified in the error YAML definition.
         *
         *  @param[in] transactionId - The unique identifier of the journal entry(ies) to be committed.
         *  @param[in] errMsg - The error exception message associated with the error event log to be committed.
         *  @param[in] errLvl - The error level/severity indicator.
         *
         *  @return entryID[uint32_t] - The ID of the entry.
         */
        virtual uint32_t commitWithLvl(
            uint64_t transactionId,
            std::string errMsg,
            uint32_t errLvl) = 0;



        /** @brief Emit interface added */
        void emit_added()
        {
            _xyz_openbmc_project_logging_internal_Manager_interface.emit_added();
        }

        /** @brief Emit interface removed */
        void emit_removed()
        {
            _xyz_openbmc_project_logging_internal_Manager_interface.emit_removed();
        }

    private:

        /** @brief sd-bus callback for Commit
         */
        static int _callback_Commit(
            sd_bus_message*, void*, sd_bus_error*);

        /** @brief sd-bus callback for CommitWithLvl
         */
        static int _callback_CommitWithLvl(
            sd_bus_message*, void*, sd_bus_error*);

        static const vtable_t _vtable[];
        sdbusplus::server::interface_t
                _xyz_openbmc_project_logging_internal_Manager_interface;
        sdbusplus::SdBusInterface* _intf;

};

} // namespace sdbusplus::server::xyz::openbmc_project::logging::internal

#ifndef SDBUSPP_REMOVE_DEPRECATED_NAMESPACE
namespace sdbusplus::xyz::openbmc_project::Logging::Internal::server {

using sdbusplus::server::xyz::openbmc_project::logging::internal::Manager;

} // namespace sdbusplus::xyz::openbmc_project::Logging::Internal::server
#endif

