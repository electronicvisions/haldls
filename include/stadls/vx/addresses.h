#pragma once

#include "haldls/is_leaf_node.h"
#include "haldls/vx/traits.h"
#include "hate/type_traits.h"
#include "stadls/visitors.h"
#include "stadls/vx/supports_empty.h"
#include <type_traits>
#include <utility>

namespace stadls::vx {

namespace detail {

/**
 * Helpers for checking if {read_,write_,}addresses exists and in which form
 * (templated vs. non-templated). This is used to compile-time dispatch to the
 * correct version below.
 */
template <typename T, typename C, typename A>
using has_read_addresses_templated =
    decltype(std::declval<T&>().template read_addresses<A>(std::declval<C>()));

template <typename T, typename C>
using has_read_addresses = decltype(std::declval<T&>().read_addresses(std::declval<C>()));

template <typename T, typename C, typename A>
using has_write_addresses_templated =
    decltype(std::declval<T&>().template write_addresses<A>(std::declval<C>()));

template <typename T, typename C>
using has_write_addresses = decltype(std::declval<T&>().write_addresses(std::declval<C>()));

template <typename T, typename C, typename A>
using has_addresses_templated =
    decltype(std::declval<T&>().template addresses<A>(std::declval<C>()));

template <typename T, typename C>
using has_addresses = decltype(std::declval<T&>().addresses(std::declval<C>()));

} // namespace detail


/**
 * Get read addresses which are local to this container, i.e. which can be accessed via
 * container.{read_,}addresses() as opposed to visiting the down-stream containers it is (also)
 * comprised of. In the case of leaf-node containers this are all its read addresses.
 */
template <
    typename AddressT,
    typename CoordinateT,
    template <typename>
    typename CoordinateToContainer>
auto get_local_read_addresses(CoordinateT const& c)
{
	using ContainerT = typename CoordinateToContainer<CoordinateT>::type;
	static_assert(
	    hate::is_detected_v<
	        detail::has_read_addresses_templated, ContainerT, CoordinateT, AddressT> ||
	        hate::is_detected_v<detail::has_read_addresses, ContainerT, CoordinateT> ||
	        hate::is_detected_v<
	            detail::has_addresses_templated, ContainerT, CoordinateT, AddressT> ||
	        hate::is_detected_v<detail::has_addresses, ContainerT, CoordinateT>,
	    "Cannot find {read_,}addresses()");
	if constexpr (hate::is_detected_v<
	                  detail::has_read_addresses_templated, ContainerT, CoordinateT, AddressT>) {
		static_assert(std::is_same_v<
		              typename detail::has_read_addresses_templated<
		                  ContainerT, CoordinateT, AddressT>::value_type,
		              AddressT>);
		return ContainerT::template read_addresses<AddressT>(c);
	} else if constexpr (hate::is_detected_v<detail::has_read_addresses, ContainerT, CoordinateT>) {
		static_assert(std::is_same_v<
		              typename detail::has_read_addresses<ContainerT, CoordinateT>::value_type,
		              AddressT>);
		return ContainerT::read_addresses(c);
	} else if constexpr (hate::is_detected_v<
	                         detail::has_addresses_templated, ContainerT, CoordinateT, AddressT>) {
		static_assert(std::is_same_v<
		              typename detail::has_addresses_templated<
		                  ContainerT, CoordinateT, AddressT>::value_type,
		              AddressT>);
		return ContainerT::template addresses<AddressT>(c);
	} else if constexpr (hate::is_detected_v<detail::has_addresses, ContainerT, CoordinateT>) {
		static_assert(
		    std::is_same_v<
		        typename detail::has_addresses<ContainerT, CoordinateT>::value_type, AddressT>);
		return ContainerT::addresses(c);
	} else {
		// cannot happen
	}
}

/**
 * Get write addresses which are local to this container, i.e. which can be accessed via
 * container.{write,}addresses() as opposed to visiting the down-stream containers it is (also)
 * comprised of. In the case of leaf-node containers this are all its write addresses.
 */
template <
    typename AddressT,
    typename CoordinateT,
    template <typename>
    typename CoordinateToContainer>
auto get_local_write_addresses(CoordinateT const& c)
{
	using ContainerT = typename CoordinateToContainer<CoordinateT>::type;
	static_assert(
	    hate::is_detected_v<
	        detail::has_write_addresses_templated, ContainerT, CoordinateT, AddressT> ||
	        hate::is_detected_v<detail::has_write_addresses, ContainerT, CoordinateT> ||
	        hate::is_detected_v<
	            detail::has_addresses_templated, ContainerT, CoordinateT, AddressT> ||
	        hate::is_detected_v<detail::has_addresses, ContainerT, CoordinateT>,
	    "Cannot find {write_,}addresses()");
	if constexpr (hate::is_detected_v<
	                  detail::has_write_addresses_templated, ContainerT, CoordinateT, AddressT>) {
		return ContainerT::template write_addresses<AddressT>(c);
	} else if constexpr (hate::is_detected_v<
	                         detail::has_write_addresses, ContainerT, CoordinateT>) {
		return ContainerT::write_addresses(c);
	} else if constexpr (hate::is_detected_v<
	                         detail::has_addresses_templated, ContainerT, CoordinateT, AddressT>) {
		return ContainerT::template addresses<AddressT>(c);
	} else if constexpr (hate::is_detected_v<detail::has_addresses, ContainerT, CoordinateT>) {
		return ContainerT::addresses(c);
	} else {
		// cannot happen
	}
}

template <
    typename AddressT,
    typename CoordinateT,
    template <typename>
    typename CoordinateToContainer>
auto get_read_addresses(CoordinateT const& c)
{
	using ContainerT = typename CoordinateToContainer<CoordinateT>::type;
	if constexpr (haldls::IsLeafNode<ContainerT>::value) {
		return get_local_read_addresses<AddressT, CoordinateT, CoordinateToContainer>(c);
	} else {
		typedef std::vector<AddressT> AddressesT;
		AddressesT addresses;
		static_assert(supports_empty_container_v<ContainerT>);
		hate::Empty<ContainerT> config;
		haldls::vx::visit_preorder(config, c, stadls::ReadAddressVisitor<AddressesT>{addresses});
		return addresses;
	}
}

template <
    typename AddressT,
    typename CoordinateT,
    template <typename>
    typename CoordinateToContainer>
auto get_write_addresses(CoordinateT const& c)
{
	using ContainerT = typename CoordinateToContainer<CoordinateT>::type;
	if constexpr (haldls::IsLeafNode<ContainerT>::value) {
		return get_local_write_addresses<AddressT, CoordinateT, CoordinateToContainer>(c);
	} else {
		typedef std::vector<AddressT> AddressesT;
		AddressesT addresses;
		static_assert(supports_empty_container_v<ContainerT>);
		hate::Empty<ContainerT> config;
		haldls::vx::visit_preorder(config, c, stadls::WriteAddressVisitor<AddressesT>{addresses});
		return addresses;
	}
}

} // namespace stadls::vx
