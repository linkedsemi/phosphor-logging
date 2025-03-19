#pragma once
#include <algorithm>
#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>

#include <sdbusplus/exception.hpp>
#include <sdbusplus/message.hpp>

namespace sdbusplus::common::xyz::openbmc_project::logging::internal
{

struct Manager
{
    static constexpr auto interface = "xyz.openbmc_project.Logging.Internal.Manager";


};



} // sdbusplus::common::xyz::openbmc_project::logging::internal

namespace sdbusplus::message::details
{
} // namespace sdbusplus::message::details

