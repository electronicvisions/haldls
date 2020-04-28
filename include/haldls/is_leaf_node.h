#pragma once
#include <type_traits>
#include <boost/utility/enable_if.hpp>

namespace haldls {

template <typename ContainerT, typename = void>
struct IsLeafNode : std::false_type
{};

template <typename ContainerT>
struct IsLeafNode<
    ContainerT,
    typename boost::enable_if_has_type<typename ContainerT::is_leaf_node>::type>
{
	constexpr static bool value = ContainerT::is_leaf_node::value;
};

} // namespace haldls::vx
