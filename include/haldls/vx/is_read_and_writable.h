#pragma once
#include "fisch/vx/container_cast.h"
#include "fisch/vx/traits.h"
#include "haldls/vx/traits.h"
#include "hate/type_list.h"
#include <array>
#include <type_traits>

namespace haldls::vx::detail {

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

} // namespace haldls::vx::detail
