#pragma once
#include "fisch/vx/container_cast.h"
#include "fisch/vx/traits.h"
#include "fisch/vx/word_access_type.h"
#include "haldls/vx/genpybind.h"
#include "hate/type_list.h"
#include "hate/visibility.h"
#include <type_traits>
#include <boost/utility/enable_if.hpp>
#include <boost/variant.hpp>

namespace haldls::vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Trait signalling derived-from container type support differential write operation.
 */
class GENPYBIND(visible) DifferentialWriteTrait
{};

/**
 * Possible backends to target with PlaybackProgramBuilder::read/write.
 */
enum class GENPYBIND(visible) Backend
{
#define PLAYBACK_CONTAINER(Name, Type) Name,
#define LAST_PLAYBACK_CONTAINER(Name, Type) Name
#include "fisch/vx/container.def"
};

typedef hate::type_list<
#define PLAYBACK_CONTAINER(Name, Type) fisch::vx::word_access_type::Name,
#define LAST_PLAYBACK_CONTAINER(Name, Type) fisch::vx::word_access_type::Name
#include "fisch/vx/container.def"
    >
    BackendContainerList;

namespace detail {

template <typename BackendContainer>
struct backend_from_backend_container_type
{
	constexpr static Backend backend = static_cast<Backend>(
	    hate::index_type_list_by_type<BackendContainer, BackendContainerList>::value);
};

template <Backend B>
struct backend_container_type_from_backend
{
	typedef
	    typename hate::index_type_list_by_integer<static_cast<size_t>(B), BackendContainerList>::
	        type container_type;
};

template <typename T, size_t N>
constexpr bool is_in_array(std::array<T, N> const& arr, T const& test)
{
	for (auto a : arr) {
		if (a == test) {
			return true;
		}
	}
	return false;
}

/**
 * Generate lookup table from backend to readable and writable property.
 */
template <typename TL>
struct gen_is_read_and_writeable_lookup_table;

template <typename... Ts>
struct gen_is_read_and_writeable_lookup_table<hate::type_list<Ts...>>
{
	constexpr static std::array<bool, sizeof...(Ts)> value = {
	    (fisch::vx::IsReadable<decltype(fisch::vx::container_cast(std::declval<Ts>()))>::value &&
	     fisch::vx::IsWritable<decltype(fisch::vx::container_cast(std::declval<Ts>()))>::value)...};
};

/**
 * Get whether given backend container is readable and writable.
 * @param b Backend to check
 * @return Boolean value
 */
constexpr bool is_read_and_writeable(Backend const b)
{
	constexpr auto lookup_table =
	    gen_is_read_and_writeable_lookup_table<BackendContainerList>::value;
	return lookup_table[static_cast<size_t>(b)];
}

/**
 * Backend container trait base. Each container has to support at least one backend container.
 * Multiple supported backends allow specifying a default backend which is used if no backend is
 * given at a PlaybackProgramBuilder::read/write instruction.
 * @tparam ContainerT Container for which to generate backend traits for
 * @tparam DefaultBackendContainer Default backend to use if no is given
 * @tparam AdditionalBackendContainer Additionally supported backends
 */
template <
    typename ContainerT,
    typename DefaultBackendContainer,
    typename... AdditionalBackendContainer>
struct BackendContainerBase
{
	typedef hate::type_list<DefaultBackendContainer, AdditionalBackendContainer...> container_list;
	typedef DefaultBackendContainer default_container;

	constexpr static Backend default_backend =
	    backend_from_backend_container_type<DefaultBackendContainer>::backend;

	constexpr static std::array<Backend, sizeof...(AdditionalBackendContainer) + 1> valid_backends{
	    backend_from_backend_container_type<DefaultBackendContainer>::backend,
	    backend_from_backend_container_type<AdditionalBackendContainer>::backend...};

	template <typename TL>
	struct generate_lookup_table;

	template <typename... Ts>
	struct generate_lookup_table<hate::type_list<Ts...>>
	{
		typedef std::array<Backend, sizeof...(Ts)> table_type;

		template <size_t I, size_t... Is>
		constexpr static table_type gen(table_type table, std::index_sequence<I, Is...>)
		{
			if constexpr (hate::is_in_type_list<
			                  typename hate::index_type_list_by_integer<
			                      I, BackendContainerList>::type,
			                  container_list>::value) {
				table[I] = static_cast<Backend>(
				    hate::index_type_list_by_type<
				        typename hate::index_type_list_by_integer<I, BackendContainerList>::type,
				        container_list>::value);
			} else {
				table[I] = static_cast<Backend>(0);
			}
			if constexpr (sizeof...(Is) != 0) {
				return gen(table, std::index_sequence<Is...>());
			} else {
				return table;
			}
		}

		constexpr static auto backend_index_lookup_table =
		    gen(table_type(), std::make_index_sequence<sizeof...(Ts)>());
	};

	constexpr static auto backend_index_lookup_table =
	    generate_lookup_table<BackendContainerList>::backend_index_lookup_table;

	constexpr static bool valid(Backend backend) { return is_in_array(valid_backends, backend); }
};

template <typename ContainerT>
struct BackendContainerTrait : public BackendContainerBase<ContainerT, ContainerT>
{};

} // namespace detail


template <typename T, typename = void>
struct HasLocalData : public std::false_type
{};

template <typename T>
struct HasLocalData<T, typename boost::enable_if_has_type<typename T::has_local_data>::type>
    : public T::has_local_data
{};

template <typename T>
struct HasLocalData<T, typename boost::enable_if_has_type<typename T::is_leaf_node>::type>
    : public T::is_leaf_node
{};

} // namespace haldls::vx
