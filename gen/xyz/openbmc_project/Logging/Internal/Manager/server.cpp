#include <map>
#include <sdbusplus/sdbus.hpp>
#include <sdbusplus/sdbuspp_support/server.hpp>
#include <sdbusplus/server.hpp>
#include <string>
#include <tuple>

#include <xyz/openbmc_project/Logging/Internal/Manager/server.hpp>


namespace sdbusplus::server::xyz::openbmc_project::logging::internal
{


int Manager::_callback_Commit(
        sd_bus_message* msg, void* context, sd_bus_error* error)
{
    auto o = static_cast<Manager*>(context);

    {
        return sdbusplus::sdbuspp::method_callback(
                msg, o->_intf, error,
                std::function(
                    [=](uint64_t&& transactionId, std::string&& errMsg)
                    {
                        return o->commit(
                                transactionId, errMsg);
                    }
                ));
    }
}

namespace details
{
namespace Manager
{
static const auto _param_Commit =
        utility::tuple_to_array(message::types::type_id<
                uint64_t, std::string>());
static const auto _return_Commit =
        utility::tuple_to_array(message::types::type_id<
                uint32_t>());
}
}

int Manager::_callback_CommitWithLvl(
        sd_bus_message* msg, void* context, sd_bus_error* error)
{
    auto o = static_cast<Manager*>(context);

    {
        return sdbusplus::sdbuspp::method_callback(
                msg, o->_intf, error,
                std::function(
                    [=](uint64_t&& transactionId, std::string&& errMsg, uint32_t&& errLvl)
                    {
                        return o->commitWithLvl(
                                transactionId, errMsg, errLvl);
                    }
                ));
    }
}

namespace details
{
namespace Manager
{
static const auto _param_CommitWithLvl =
        utility::tuple_to_array(message::types::type_id<
                uint64_t, std::string, uint32_t>());
static const auto _return_CommitWithLvl =
        utility::tuple_to_array(message::types::type_id<
                uint32_t>());
}
}





const vtable_t Manager::_vtable[] = {
    vtable::start(),

    vtable::method("Commit",
                   details::Manager::_param_Commit
                        .data(),
                   details::Manager::_return_Commit
                        .data(),
                   _callback_Commit),

    vtable::method("CommitWithLvl",
                   details::Manager::_param_CommitWithLvl
                        .data(),
                   details::Manager::_return_CommitWithLvl
                        .data(),
                   _callback_CommitWithLvl),
    vtable::end()
};

} // namespace sdbusplus::server::xyz::openbmc_project::logging::internal

