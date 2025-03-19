#pragma once
#include <sdbusplus/async/client.hpp>
#include <type_traits>

#include <xyz/openbmc_project/Logging/Internal/Manager/common.hpp>

#ifndef SDBUSPP_REMOVE_DEPRECATED_NAMESPACE
namespace sdbusplus::xyz::openbmc_project::Logging::Internal::client::Manager
{

constexpr auto interface =
    sdbusplus::common::xyz::openbmc_project::logging::internal::Manager::interface;

} // namespace sdbusplus::xyz::openbmc_project::Logging::Internal::client::Manager
#endif

namespace sdbusplus::client::xyz::openbmc_project::logging::internal
{

namespace details
{

template <typename Proxy>
class Manager :
    public sdbusplus::common::xyz::openbmc_project::logging::internal::Manager
{
  public:
    template <bool S, bool P, bool Preserved,
              template <typename> typename... Types>
    friend class sdbusplus::async::client::client;
    // Delete default constructor as these should only be constructed
    // indirectly through sdbusplus::async::client_t.
    Manager() = delete;

    /** @brief Commit
     *  Write the requested error/event entry with its associated metadata fields to flash. The "level" of the committed error log is same as the level defined in error YAML definitions.
     *
     *  @param[in] transactionId - The unique identifier of the journal entry(ies) to be committed.
     *  @param[in] errMsg - The error exception message associated with the error event log to be committed.
     *
     *  @return entryID[uint32_t] - The ID of the entry.
     */
    auto commit(uint64_t transactionId, std::string errMsg)
    {
        return proxy.template call<uint32_t>(ctx, "Commit", transactionId, errMsg);
    }

    /** @brief CommitWithLvl
     *  Write the requested error/event entry with its associated metadata fields to flash. This interface allows the caller to override the error level specified in the error YAML definition.
     *
     *  @param[in] transactionId - The unique identifier of the journal entry(ies) to be committed.
     *  @param[in] errMsg - The error exception message associated with the error event log to be committed.
     *  @param[in] errLvl - The error level/severity indicator.
     *
     *  @return entryID[uint32_t] - The ID of the entry.
     */
    auto commitWithLvl(uint64_t transactionId, std::string errMsg, uint32_t errLvl)
    {
        return proxy.template call<uint32_t>(ctx, "CommitWithLvl", transactionId, errMsg, errLvl);
    }

  private:
    // Conversion constructor from proxy used by client_t.
    constexpr Manager(sdbusplus::async::context& ctx, Proxy p) :
        ctx(ctx), proxy(p.interface(interface)) {}

    sdbusplus::async::context& ctx{};
    decltype(std::declval<Proxy>().interface(interface)) proxy = {};
};

} // namespace details

/** Alias class so we can use the client in both a client_t aggregation
 *  and individually.
 *
 *  sdbusplus::async::client_t<Manager>() or
 *  Manager() both construct an equivalent instance.
 */
template <typename Proxy = void>
struct Manager : public
    std::conditional_t<
        std::is_void_v<Proxy>,
        sdbusplus::async::client_t<details::Manager>,
        details::Manager<Proxy>>
{
    template <typename... Args>
    Manager(Args&&... args) :
        std::conditional_t<std::is_void_v<Proxy>,
                           sdbusplus::async::client_t<details::Manager>,
                           details::Manager<Proxy>>(
            std::forward<Args>(args)...)
    {}
};

} // namespace sdbusplus::client::xyz::openbmc_project::logging::internal::Manager

