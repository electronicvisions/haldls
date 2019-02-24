#pragma once
#include <type_traits>

#include "haldls/vx/genpybind.h"

namespace haldls::vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Possible backends to target with PlaybackProgramBuilder::read/write.
 */
enum class GENPYBIND(visible) Backend
{
	OmnibusOnChipOverJTAG, /** On-chip omnibus over JTAG backend. */
	Direct                 /** Backend for container providing UT messages directly. */
};

namespace detail {

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
 * Backend trait base. Each container is to be supported by at least one backend.
 * Multiple supported backends allow specifying a default backend which is used if no backend is
 * given at a PlaybackProgramBuilder::read/write instruction.
 * @tparam ContainerT Container for which to generate backend traits for
 * @tparam Default Default backend to use if no is given
 * @tparam Backends Additionally supported backends
 */
template <typename ContainerT, Backend Default, Backend... Backends>
struct BackendTraitBase
{
	constexpr static bool valid(Backend backend)
	{
		return is_in_array(valid_backends, backend);
	}

	constexpr static std::array<Backend, sizeof...(Backends) + 1> valid_backends{Default,
	                                                                             Backends...};
	constexpr static Backend default_backend = Default;
};

template <typename ContainerT>
struct BackendTrait : public BackendTraitBase<ContainerT, Backend::Direct>
{};

} // namespace detail

} // namespace haldls::vx::detail
