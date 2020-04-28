#pragma once
#include "haldls/vx/common.h"
#include "hate/type_traits.h"
#include "stadls/visitors.h"
#include "stadls/vx/supports_empty.h"
#include <type_traits>

namespace stadls::vx {

namespace detail {

/**
 * Get coordinate matching container, e.g. with respect to size.
 * @tparam T Container type
 * @param Container
 * @return Coordinate
 */
template <typename T>
typename T::coordinate_type get_coord(T const& config);

/**
 * Helpers for checking if decode exists. This is used to compile-time dispatch to the
 * correct version below.
 */
template <typename T, typename C, typename A>
using has_decode_templated = decltype(std::declval<T&>().template decode<A>(std::declval<C>()));

template <typename T, typename C>
using has_decode = decltype(std::declval<T&>().decode(std::declval<C>()));

/**
 * Helpers for checking if decode exists. This is used to compile-time dispatch to the
 * correct version below.
 */
template <typename T, typename C, typename CC, typename A>
using has_decode_coordinate_templated =
    decltype(std::declval<T&>().template decode<A>(std::declval<CC const&>(), std::declval<C>()));

template <typename T, typename C, typename CC>
using has_decode_coordinate =
    decltype(std::declval<T&>().decode(std::declval<CC const&>(), std::declval<C>()));

} // namespace detail

template <typename WordT, typename ContainerT, typename CoordinateT, size_t N>
void set_local_decode(
    ContainerT& config, [[maybe_unused]] CoordinateT const& coord, std::array<WordT, N> const& data)
{
	typedef std::array<WordT, N> WordsT;
	static_assert(
	    hate::is_detected_v<detail::has_decode, ContainerT, WordsT> ||
	        hate::is_detected_v<detail::has_decode_templated, ContainerT, WordsT> ||
	        hate::is_detected_v<
	            detail::has_decode_coordinate, ContainerT, typename ContainerT::coordinate_type,
	            WordsT> ||
	        hate::is_detected_v<
	            detail::has_decode_coordinate_templated, ContainerT,
	            typename ContainerT::coordinate_type, WordsT>,
	    "Cannot find decode()");
	if constexpr (hate::is_detected_v<detail::has_decode, ContainerT, WordsT>) {
		config.decode(data);
	} else if constexpr (hate::is_detected_v<detail::has_decode_templated, ContainerT, WordsT>) {
		config.template decode<WordsT>(data);
	} else if constexpr (hate::is_detected_v<
	                         detail::has_decode_coordinate, ContainerT,
	                         typename ContainerT::coordinate_type, WordsT>) {
		static_assert(!hate::is_empty_v<CoordinateT>);
		config.decode(coord, data);
	} else if constexpr (hate::is_detected_v<
	                         detail::has_decode_coordinate_templated, ContainerT,
	                         typename ContainerT::coordinate_type, WordsT>) {
		static_assert(!hate::is_empty_v<CoordinateT>);
		config.template decode<WordsT>(coord, data);
	}
}

template <typename WordT, typename ContainerT, typename CoordinateT>
void set_decode(
    ContainerT& config, [[maybe_unused]] CoordinateT const& coord, std::vector<WordT> const& data)
{
	if constexpr (supports_empty_coordinate_v<ContainerT>) {
		haldls::vx::visit_preorder(
		    config, hate::Empty<typename ContainerT::coordinate_type>{},
		    stadls::DecodeVisitor<std::vector<WordT>>{data});
	} else {
		haldls::vx::visit_preorder(config, coord, stadls::DecodeVisitor<std::vector<WordT>>{data});
	}
}

template <typename WordT, typename ContainerT, typename CoordinateT, size_t N>
void set_decode(
    ContainerT& config, [[maybe_unused]] CoordinateT const& coord, std::array<WordT, N> const& data)
{
	if constexpr (haldls::IsLeafNode<ContainerT>::value) {
		set_local_decode(config, coord, data);
	} else if constexpr (supports_empty_coordinate_v<ContainerT>) {
		haldls::vx::visit_preorder(
		    config, hate::Empty<typename ContainerT::coordinate_type>{},
		    stadls::DecodeVisitor<std::array<WordT, N>>{data});
	} else {
		haldls::vx::visit_preorder(
		    config, config, stadls::DecodeVisitor<std::array<WordT, N>>{data});
	}
}

} // namespace stadls::vx
