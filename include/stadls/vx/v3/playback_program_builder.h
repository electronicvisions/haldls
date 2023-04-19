#pragma once
#include "haldls/vx/v3/container.h"
#include "lola/vx/v3/container.h"
#include "stadls/vx/genpybind.h"
#include "stadls/vx/playback_program_builder.h"
#include "stadls/vx/v3/coordinate_to_container.h"
#include "stadls/vx/v3/dumper.h"
#include <cereal/macros.hpp>

namespace stadls::vx {

namespace detail {

extern template SYMBOL_VISIBLE std::ostream& operator<<(
    std::ostream&,
    PlaybackProgramBuilderAdapter<
        fisch::vx::PlaybackProgramBuilder,
        stadls::vx::PlaybackProgram> const&);

} // namespace detail

extern template class SYMBOL_VISIBLE stadls::vx::detail::
    PlaybackProgramBuilderAdapter<fisch::vx::PlaybackProgramBuilder, stadls::vx::PlaybackProgram>;

namespace v3 GENPYBIND_TAG_STADLS_VX_V3 {

using PlaybackProgramBuilder GENPYBIND(opaque) =
    detail::PlaybackProgramBuilderAdapter<fisch::vx::PlaybackProgramBuilder, PlaybackProgram>;

/**
 * Convert a PlaybackProgramBuilderDumper to a PlaybackProgramBuilder.
 * The dumper is emptied during the process.
 * @param dumper Dumper to convert
 */
PlaybackProgramBuilder GENPYBIND(visible)
    convert_to_builder(PlaybackProgramBuilderDumper& dumper) SYMBOL_VISIBLE;

/**
 * Convert a PlaybackProgramBuilderDumper to a PlaybackProgramBuilder (rvalue reference overload).
 * The dumper is emptied during the process.
 * @param dumper Dumper to convert
 */
PlaybackProgramBuilder convert_to_builder(PlaybackProgramBuilderDumper&& dumper) SYMBOL_VISIBLE;

/**
 * Convert a sequence of coordinate container pairs to a PlaybackProgramBuilder.
 * @param cocos Coordinate-Container pair sequence
 */
PlaybackProgramBuilder GENPYBIND(visible)
    convert_to_builder(Dumper::done_type const& cocos) SYMBOL_VISIBLE;

} // namespace v3

} // namespace stadls::vx
