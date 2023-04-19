#pragma once
#ifndef __ppu__
#include "fisch/vx/container.h"
#include "fisch/vx/coordinates.h"
#include "fisch/vx/word_access_type.h"
#include "halco/common/coordinate.h"
#include "haldls/vx/encodable.tcc"
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
struct ContainerBaseImpl
{
	template <typename CoordinateT>
	using CoordinateToContainer = hate::Empty<Derived>;

	template <size_t SupportedBackendIndex>
	static Container::BackendCoordinateListVariant encode_read(
	    typename Derived::coordinate_type const& coordinate);

	template <size_t... SupportedBackendIndex>
	static Container::BackendCoordinateListVariant encode_read(
	    typename Derived::coordinate_type const& coordinate,
	    size_t backend_index,
	    std::index_sequence<SupportedBackendIndex...>);
};


template <typename Derived>
template <size_t SupportedBackendIndex>
Container::BackendCoordinateListVariant ContainerBaseImpl<Derived>::encode_read(
    typename Derived::coordinate_type const& coordinate)
{
	typedef typename hate::index_type_list_by_integer<
	    SupportedBackendIndex,
	    typename haldls::vx::detail::BackendContainerTrait<Derived>::container_list>::type
	    backend_container_type;

	typedef typename hate::index_type_list_by_integer<
	    hate::index_type_list_by_type<
	        std::vector<backend_container_type>,
	        hate::type_list_from<Container::BackendContainerListVariant>::type>::value,
	    hate::type_list_from<Container::BackendCoordinateListVariant>::type>::type::value_type
	    backend_coordinate_type;

	auto maybe_array_read_addresses = stadls::vx::get_read_addresses<
	    backend_coordinate_type, typename Derived::coordinate_type, CoordinateToContainer>(
	    coordinate);

	if constexpr (std::is_same_v<
	                  std::vector<backend_coordinate_type>, decltype(maybe_array_read_addresses)>) {
		return maybe_array_read_addresses;
	} else {
		return std::vector<backend_coordinate_type>{
		    maybe_array_read_addresses.begin(), maybe_array_read_addresses.end()};
	}
}

template <typename Derived>
template <size_t... SupportedBackendIndex>
Container::BackendCoordinateListVariant ContainerBaseImpl<Derived>::encode_read(
    typename Derived::coordinate_type const& coordinate,
    size_t backend_index,
    std::index_sequence<SupportedBackendIndex...>)
{
	static const std::array<
	    Container::BackendCoordinateListVariant (*)(typename Derived::coordinate_type const&),
	    sizeof...(SupportedBackendIndex)>
	    encode_read_table{encode_read<SupportedBackendIndex>...};
	return encode_read_table.at(backend_index)(coordinate);
}

} // namespace

template <typename Derived, typename Base>
typename ContainerBase<Derived, Base>::BackendCoordinateListVariant
ContainerBase<Derived, Base>::encode_read(
    Coordinate const& coordinate, std::optional<Backend> const& backend) const
{
	size_t const backend_index = static_cast<size_t>(
	    haldls::vx::detail::BackendContainerTrait<Derived>::backend_index_lookup_table.at(
	        static_cast<size_t>(
	            backend ? *backend : detail::BackendContainerTrait<Derived>::default_backend)));

	return ContainerBaseImpl<Derived>::encode_read(
	    dynamic_cast<typename Derived::coordinate_type const&>(coordinate), backend_index,
	    std::make_index_sequence<hate::type_list_size<
	        typename haldls::vx::detail::BackendContainerTrait<Derived>::container_list>::value>());
}

template <typename Derived, typename Base>
void ContainerBase<Derived, Base>::decode_read(
    BackendContainerListVariant const& data, Coordinate const& coordinate)
{
	auto const& real_coordinate =
	    dynamic_cast<typename Derived::coordinate_type const&>(coordinate);

	auto const visitor = [this, real_coordinate](auto const& visited_data) {
		typedef typename std::decay_t<decltype(visited_data)>::value_type backend_container_type;
		if constexpr (hate::is_in_type_list<
		                  backend_container_type,
		                  typename detail::BackendContainerTrait<Derived>::container_list>::value) {
			stadls::vx::set_decode(static_cast<Derived&>(*this), real_coordinate, visited_data);
		} else {
			throw std::runtime_error(
			    "Backend data type (" + hate::name<backend_container_type>() +
			    ") not supported in decoding of container (" + hate::name<Derived>() + ").");
		}
	};

	std::visit(visitor, data);
}

namespace {

template <typename T>
using unsupported_read_targets_checker = decltype(T::unsupported_read_targets);

template <typename T>
constexpr static auto has_unsupported_read_targets =
    hate::is_detected_v<unsupported_read_targets_checker, T>;

} // namespace

template <typename Derived, typename Base>
std::initializer_list<hxcomm::vx::Target>
ContainerBase<Derived, Base>::get_unsupported_read_targets() const
{
	if constexpr (has_unsupported_read_targets<Derived>) {
		return Derived::unsupported_read_targets;
	} else {
		return {};
	}
}

template <typename Derived, typename Base>
std::unique_ptr<Container> ContainerBase<Derived, Base>::clone_container() const
{
	return std::make_unique<Derived>(static_cast<Derived const&>(*this));
}

template <typename Derived, typename Base>
bool ContainerBase<Derived, Base>::operator==(Container const& other) const
{
	if (auto const other_ptr = dynamic_cast<Derived const*>(&other)) {
		return static_cast<Derived const&>(*this) == *other_ptr;
	}
	return false;
}

template <typename Derived, typename Base>
bool ContainerBase<Derived, Base>::operator!=(Container const& other) const
{
	return !(*this == other);
}

#define HALDLS_CONCAT_(x, y) x##y
#define HALDLS_CONCAT(x, y) HALDLS_CONCAT_(x, y)

#define HALDLS_ANONYMOUS_NAME() HALDLS_CONCAT(anonymous_, __LINE__)

#define EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(Derived)                                        \
	EXPLICIT_INSTANTIATE_HALDLS_ENCODABLE_BASE(Derived, haldls::vx::Container)                     \
	template struct haldls::vx::ContainerBase<Derived, haldls::vx::Container>;                     \
                                                                                                   \
	namespace HALDLS_ANONYMOUS_NAME()                                                              \
	{                                                                                              \
		static std::unique_ptr<haldls::vx::Container> construct_container(                         \
		    haldls::vx::Container::Coordinate const&)                                              \
		{                                                                                          \
			return std::make_unique<Derived>();                                                    \
		}                                                                                          \
                                                                                                   \
		static auto _ =                                                                            \
		    haldls::vx::register_container(typeid(Derived::coordinate_type), construct_container); \
                                                                                                   \
	} // namespace


#define EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE_CUSTOM_CONSTRUCTION(                            \
    Derived, ConstructContainer)                                                                   \
	EXPLICIT_INSTANTIATE_HALDLS_ENCODABLE_BASE(Derived, haldls::vx::Container)                     \
	template struct haldls::vx::ContainerBase<Derived, haldls::vx::Container>;                     \
                                                                                                   \
	namespace HALDLS_ANONYMOUS_NAME()                                                              \
	{                                                                                              \
		static auto _ =                                                                            \
		    haldls::vx::register_container(typeid(Derived::coordinate_type), ConstructContainer);  \
	} // namespace
#else
#define EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(Derived)
#define EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE_CUSTOM_CONSTRUCTION(Derived, ConstructContainer)
#endif
} // namespace haldls::vx
