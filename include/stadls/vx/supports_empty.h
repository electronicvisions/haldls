#pragma once
#include "haldls/vx/common.h"
#include "hate/type_traits.h"

namespace stadls::vx {

namespace detail {

struct DummyVisitor
{
	template <typename ContainerT, typename CoordinateT>
	void operator()(CoordinateT const&, ContainerT const&)
	{}
};

template <typename ContainerT, typename = void>
struct container_supports_empty_coordinate : std::true_type
{};

template <typename ContainerT>
struct container_supports_empty_coordinate<
    ContainerT,
    typename boost::enable_if_has_type<typename ContainerT::supports_empty_coordinate>::type>
    : ContainerT::supports_empty_coordinate
{};

template <typename ContainerT>
constexpr bool container_supports_empty_coordinate_v =
    container_supports_empty_coordinate<ContainerT>::value;

} // namespace detail

template <typename ContainerT>
using has_empty_coordinate_visit_call =
    decltype(haldls::vx::detail::VisitPreorderImpl<ContainerT>::template call(
        std::declval<ContainerT&>(),
        std::declval<hate::Empty<typename ContainerT::coordinate_type>>(),
        std::declval<detail::DummyVisitor>()));

template <typename ContainerT>
using has_empty_container_visit_call =
    decltype(haldls::vx::detail::VisitPreorderImpl<ContainerT>::template call(
        std::declval<hate::Empty<ContainerT> const&>(),
        std::declval<typename ContainerT::coordinate_type>(),
        std::declval<detail::DummyVisitor>()));

template <typename ContainerT>
constexpr bool supports_empty_coordinate_v =
    hate::is_detected_v<has_empty_coordinate_visit_call, ContainerT>&&
        detail::container_supports_empty_coordinate_v<ContainerT>;

template <typename ContainerT>
constexpr bool supports_empty_container_v =
    hate::is_detected_v<has_empty_container_visit_call, ContainerT>;

} // namespace stadls::vx
