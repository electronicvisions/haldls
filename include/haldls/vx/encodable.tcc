#pragma once
#include "haldls/vx/encodable.h"
#ifndef __ppu__
#include "fisch/vx/container.h"
#include "fisch/vx/coordinates.h"
#include "fisch/vx/word_access_type.h"
#include "halco/common/coordinate.h"
#include "haldls/vx/traits.h"
#include "hate/empty.h"
#include "hate/type_list.h"
#include "hate/type_traits.h"
#include "hate/visibility.h"
#include "hxcomm/vx/target.h"
#include "stadls/vx/addresses.h"
#include "stadls/vx/encode.h"
#include "stadls/vx/set_decode.h"
#include <optional>
#include <ostream>
#include <tuple>
#include <variant>
#include <vector>
#endif


namespace haldls::vx {

#ifndef __ppu__
namespace {

template <typename Derived>
struct EncodableBaseImpl
{
	template <typename CoordinateT>
	using CoordinateToEncodable = hate::Empty<Derived>;

	template <size_t SupportedBackendIndex>
	static Encodable::BackendCocoListVariant encode_write(
	    Derived const& container, typename Derived::coordinate_type const& coordinate);

	template <size_t... SupportedBackendIndex>
	static Encodable::BackendCocoListVariant encode_write(
	    Derived const& container,
	    typename Derived::coordinate_type const& coordinate,
	    size_t backend_index,
	    std::index_sequence<SupportedBackendIndex...>);
};


template <typename Derived>
template <size_t SupportedBackendIndex>
Encodable::BackendCocoListVariant EncodableBaseImpl<Derived>::encode_write(
    Derived const& container, typename Derived::coordinate_type const& coordinate)
{
	typedef typename hate::index_type_list_by_integer<
	    SupportedBackendIndex,
	    typename haldls::vx::detail::BackendContainerTrait<Derived>::container_list>::type
	    backend_container_type;

	typedef typename hate::index_type_list_by_integer<
	    hate::index_type_list_by_type<
	        std::vector<backend_container_type>,
	        hate::type_list_from<Encodable::BackendContainerListVariant>::type>::value,
	    hate::type_list_from<Encodable::BackendCoordinateListVariant>::type>::type::value_type
	    backend_coordinate_type;

	auto maybe_array_write_addresses = stadls::vx::get_write_addresses<
	    backend_coordinate_type, typename Derived::coordinate_type, CoordinateToEncodable>(
	    coordinate);

	auto maybe_array_write_words =
	    stadls::vx::get_encode<backend_container_type, Derived, typename Derived::coordinate_type>(
	        container, coordinate);

	if constexpr (
	    std::is_same_v<
	        std::vector<backend_coordinate_type>, decltype(maybe_array_write_addresses)> &&
	    std::is_same_v<std::vector<backend_container_type>, decltype(maybe_array_write_words)>) {
		return std::move(
		    std::pair{std::move(maybe_array_write_addresses), std::move(maybe_array_write_words)});
	} else {
		return std::pair{
		    std::vector<backend_coordinate_type>{
		        maybe_array_write_addresses.begin(), maybe_array_write_addresses.end()},
		    std::vector<backend_container_type>{
		        maybe_array_write_words.begin(), maybe_array_write_words.end()}};
	}
}

template <typename Derived>
template <size_t... SupportedBackendIndex>
Encodable::BackendCocoListVariant EncodableBaseImpl<Derived>::encode_write(
    Derived const& container,
    typename Derived::coordinate_type const& coordinate,
    size_t backend_index,
    std::index_sequence<SupportedBackendIndex...>)
{
	static const std::array<
	    Encodable::BackendCocoListVariant (*)(
	        Derived const& container, typename Derived::coordinate_type const&),
	    sizeof...(SupportedBackendIndex)>
	    encode_write_table{encode_write<SupportedBackendIndex>...};
	return encode_write_table.at(backend_index)(container, coordinate);
}

} // namespace

template <typename Derived, typename Base>
Encodable::BackendCocoListVariant EncodableBase<Derived, Base>::encode_write(
    Coordinate const& coordinate, std::optional<Backend> const& backend) const
{
	size_t const backend_index = static_cast<size_t>(
	    haldls::vx::detail::BackendContainerTrait<Derived>::backend_index_lookup_table.at(
	        static_cast<size_t>(
	            backend ? *backend : detail::BackendContainerTrait<Derived>::default_backend)));

	return EncodableBaseImpl<Derived>::encode_write(
	    static_cast<Derived const&>(*this),
	    dynamic_cast<typename Derived::coordinate_type const&>(coordinate), backend_index,
	    std::make_index_sequence<hate::type_list_size<
	        typename haldls::vx::detail::BackendContainerTrait<Derived>::container_list>::value>());
}

namespace {

template <typename T>
using unsupported_write_targets_checker = decltype(T::unsupported_write_targets);

template <typename T>
constexpr static auto has_unsupported_write_targets =
    hate::is_detected_v<unsupported_write_targets_checker, T>;

} // namespace

template <typename Derived, typename Base>
std::initializer_list<hxcomm::vx::Target>
EncodableBase<Derived, Base>::get_unsupported_write_targets() const
{
	if constexpr (has_unsupported_write_targets<Derived>) {
		return Derived::unsupported_write_targets;
	} else {
		return {};
	}
}

template <typename Derived, typename Base>
bool EncodableBase<Derived, Base>::get_supports_differential_write() const
{
	return std::is_base_of<haldls::vx::DifferentialWriteTrait, Derived>::value;
}

template <typename Derived, typename Base>
std::unique_ptr<Encodable> EncodableBase<Derived, Base>::clone_encodable() const
{
	return std::make_unique<Derived>(static_cast<Derived const&>(*this));
}

template <typename Derived, typename Base>
bool EncodableBase<Derived, Base>::get_is_valid_backend(Backend const backend) const
{
	return detail::BackendContainerTrait<Derived>::valid(backend);
}

template <typename Derived, typename Base>
std::ostream& EncodableBase<Derived, Base>::print(std::ostream& os) const
{
	return os << static_cast<Derived const&>(*this);
}

template <typename Derived, typename Base>
bool EncodableBase<Derived, Base>::operator==(Encodable const& other) const
{
	if (auto const other_ptr = dynamic_cast<Derived const*>(&other)) {
		return static_cast<Derived const&>(*this) == *other_ptr;
	}
	return false;
}

template <typename Derived, typename Base>
bool EncodableBase<Derived, Base>::operator!=(Encodable const& other) const
{
	return !(*this == other);
}

#define EXPLICIT_INSTANTIATE_HALDLS_ENCODABLE_BASE(Derived, Base)                                  \
	template struct haldls::vx::EncodableBase<Derived, Base>;
#else
#define EXPLICIT_INSTANTIATE_HALDLS_ENCODABLE_BASE(Derived, Base)
#endif

} // namespace haldls::vx
