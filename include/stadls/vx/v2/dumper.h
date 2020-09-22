#pragma once
#include "haldls/cerealization.h"
#include "haldls/vx/v2/container.h"
#include "lola/vx/v2/container.h"
#include "stadls/vx/dumper.h"
#include "stadls/vx/genpybind.h"
#include "stadls/vx/v2/dumperdone.h"

namespace stadls::vx GENPYBIND_TAG_STADLS_VX_V2 {

namespace v2 GENPYBIND_TAG_STADLS_VX_V2 {

// Dumper is a detail, its template parameter DumperDone isn't.
// The latter is used by the PlaybackProgramBuilderDumper API.
// Hence, we don't wrap the detail, but only the public API type.
using Dumper GENPYBIND(hidden) = stadls::vx::detail::Dumper<DumperDone>;

} // namespace v2

extern template class SYMBOL_VISIBLE stadls::vx::detail::Dumper<stadls::vx::v2::DumperDone>;

extern template SYMBOL_VISIBLE std::ostream& stadls::vx::detail::operator<<(
    std::ostream&, stadls::vx::detail::Dumper<stadls::vx::v2::DumperDone> const&);

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	extern template SYMBOL_VISIBLE void stadls::vx::v2::Dumper::write<Type>(                       \
	    typename Type::coordinate_type const&, Type const&);                                       \
	extern template SYMBOL_VISIBLE stadls::vx::PlaybackProgram::ContainerTicket<Type>              \
	stadls::vx::v2::Dumper::read<Type>(typename Type::coordinate_type const&);
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v2/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v2/container.def"

} // namespace stadls::vx

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::v2::Dumper)

#if defined(__GENPYBIND_GENERATED__)
#include <pybind11/pybind11.h>
PYBIND11_MAKE_OPAQUE(stadls::vx::v2::Dumper::done_type)
#endif
