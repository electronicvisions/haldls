#pragma once
#include <type_traits>
#include <boost/utility/enable_if.hpp>

namespace haldls {

template <typename ContainerT, typename = void>
struct HasLocalData : std::false_type
{};

template <typename ContainerT>
struct HasLocalData<
    ContainerT,
    typename boost::enable_if_has_type<typename ContainerT::has_local_data>::type>
{
	constexpr static bool value = ContainerT::has_local_data::value;
};

} // namespace haldls::vx
