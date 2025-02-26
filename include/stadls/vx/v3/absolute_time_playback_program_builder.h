#pragma once
#include "hate/visibility.h"
#include "stadls/vx/absolute_time_playback_program_builder.h"
#include "stadls/vx/genpybind.h"
#include "stadls/vx/v3/playback_program_builder.h"

extern template class SYMBOL_VISIBLE
    stadls::vx::AbsoluteTimePlaybackProgramBuilder<stadls::vx::v3::PlaybackProgramBuilder>;

namespace stadls::vx { namespace v3 GENPYBIND_TAG_STADLS_VX_V3 {

using AbsoluteTimePlaybackProgramBuilder GENPYBIND(opaque) =
    stadls::vx::AbsoluteTimePlaybackProgramBuilder<stadls::vx::v3::PlaybackProgramBuilder>;
} // namespace v3
} // namespace stadls::vx

namespace stadls::vx {
extern template SYMBOL_VISIBLE std::ostream& operator<<(
    std::ostream&,
    stadls::vx::AbsoluteTimePlaybackProgramBuilder<stadls::vx::v3::PlaybackProgramBuilder> const&);
}
