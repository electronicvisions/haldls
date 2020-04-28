#pragma once

#include "haldls/is_leaf_node.h"
#include "haldls/vx/traits.h"
#include "hate/type_traits.h"
#include "stadls/visitors.h"
#include <type_traits>
#include <utility>

namespace stadls {
namespace vx {
namespace detail {

/**
 * Helpers for checking if encode exists and in which form
 * (templated vs. non-templated). This is used to compile-time dispatch to the
 * correct version below.
 */
template <typename T, typename W>
using has_encode_templated = decltype(std::declval<T&>().template encode<W>());

template <typename T>
using has_encode = decltype(std::declval<T&>().encode());

template <typename T, typename W>
using has_encode_coordinate_templated = decltype(
    std::declval<T&>().template encode<W>(std::declval<typename T::coordinate_type const&>()));

template <typename T>
using has_encode_coordinate =
    decltype(std::declval<T&>().encode(std::declval<typename T::coordinate_type const&>()));

} // namespace detail

template <typename WordT, typename ContainerT, typename CoordinateT>
auto get_local_encode(ContainerT const& config, [[maybe_unused]] CoordinateT const& coord)
{
	static_assert(
	    hate::is_detected_v<detail::has_encode_templated, ContainerT, WordT> ||
	        hate::is_detected_v<detail::has_encode, ContainerT> ||
	        hate::is_detected_v<detail::has_encode_coordinate_templated, ContainerT, WordT> ||
	        hate::is_detected_v<detail::has_encode_coordinate, ContainerT>,
	    "Cannot find encode()");
	if constexpr (hate::is_detected_v<detail::has_encode_templated, ContainerT, WordT>) {
		static_assert(std::is_same_v<
		              typename detail::has_encode_templated<ContainerT, WordT>::value_type, WordT>);
		return config.template encode<WordT>();
	} else if constexpr (hate::is_detected_v<detail::has_encode, ContainerT>) {
		static_assert(std::is_same_v<typename detail::has_encode<ContainerT>::value_type, WordT>);
		return config.encode();
	} else if constexpr (hate::is_detected_v<
	                         detail::has_encode_coordinate_templated, ContainerT, WordT>) {
		static_assert(
		    std::is_same_v<
		        typename detail::has_encode_coordinate_templated<ContainerT, WordT>::value_type,
		        WordT>);
		static_assert(!hate::is_empty_v<CoordinateT>);
		return config.template encode<WordT>(coord);
	} else if constexpr (hate::is_detected_v<detail::has_encode_coordinate, ContainerT>) {
		static_assert(
		    std::is_same_v<typename detail::has_encode_coordinate<ContainerT>::value_type, WordT>);
		static_assert(!hate::is_empty_v<CoordinateT>);
		return config.encode(coord);
	}
}

template <typename WordT, typename ContainerT, typename CoordinateT>
auto get_encode(ContainerT const& config, [[maybe_unused]] CoordinateT const& coord)
{
	if constexpr (haldls::IsLeafNode<ContainerT>::value) {
		return get_local_encode<WordT>(config, coord);
	} else {
		typedef std::vector<WordT> WordsT;
		WordsT words;
		haldls::vx::visit_preorder(config, coord, stadls::EncodeVisitor<WordsT>{words});
		return words;
	}
}

} // namespace vx
} // namespace stadls
