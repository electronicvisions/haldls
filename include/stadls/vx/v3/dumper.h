#pragma once
#include "haldls/vx/v3/container.h"
#include "lola/vx/v3/container.h"
#include "stadls/vx/dumper.h"
#include "stadls/vx/genpybind.h"
#include "stadls/vx/playback_program_builder.h"
#include "stadls/vx/v3/coordinate_to_container.h"
#include "stadls/vx/v3/dumperdone.h"
#include <cereal/macros.hpp>

namespace stadls::vx GENPYBIND_TAG_STADLS_VX_V3 {

namespace v3 GENPYBIND_TAG_STADLS_VX_V3 {

// Dumper is a detail, its template parameter DumperDone isn't.
// The latter is used by the PlaybackProgramBuilderDumper API.
// Hence, we don't wrap the detail, but only the public API type.
using Dumper GENPYBIND(hidden) = stadls::vx::detail::Dumper<DumperDone>;

} // namespace v3

extern template class SYMBOL_VISIBLE stadls::vx::detail::Dumper<stadls::vx::v3::DumperDone>;

namespace detail {

extern template SYMBOL_VISIBLE std::ostream& operator<<(
    std::ostream&,
    PlaybackProgramBuilderAdapter<
        stadls::vx::v3::Dumper,
        stadls::vx::v3::Dumper::done_type> const&);

} // namespace detail

extern template class SYMBOL_VISIBLE stadls::vx::detail::
    PlaybackProgramBuilderAdapter<stadls::vx::v3::Dumper, stadls::vx::v3::Dumper::done_type>;

namespace v3 GENPYBIND_TAG_STADLS_VX_V3 {

using PlaybackProgramBuilderDumper GENPYBIND(opaque) =
    detail::PlaybackProgramBuilderAdapter<Dumper, Dumper::done_type>;

GENPYBIND_MANUAL({
	haldls::vx::AddPickle<hate::type_list<stadls::vx::v3::PlaybackProgramBuilderDumper>>::apply(
	    parent, {"PlaybackProgramBuilderDumper"});
})

/**
 * Convert a sequence of coordinate container pairs to a PlaybackProgramBuilderDumper.
 * @param cocos Coordinate-Container pair sequence
 */
PlaybackProgramBuilderDumper GENPYBIND(visible)
    convert_to_dumper(DumperDone const& cocos) SYMBOL_VISIBLE;

} // namespace v3

extern template SYMBOL_VISIBLE std::ostream& stadls::vx::detail::operator<<(
    std::ostream&, stadls::vx::detail::Dumper<stadls::vx::v3::DumperDone> const&);

} // namespace stadls::vx

#if defined(__GENPYBIND_GENERATED__)
#include <pybind11/pybind11.h>
PYBIND11_MAKE_OPAQUE(stadls::vx::v3::Dumper::done_type)
#endif
