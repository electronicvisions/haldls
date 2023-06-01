#include "stadls/vx/absolute_time_playback_program_builder.tcc"
#include "stadls/vx/v3/playback_program_builder.h"


namespace stadls::vx {
template class SYMBOL_VISIBLE
    AbsoluteTimePlaybackProgramBuilder<stadls::vx::v3::PlaybackProgramBuilder>;

template SYMBOL_VISIBLE std::ostream& operator<<(
    std::ostream&,
    stadls::vx::AbsoluteTimePlaybackProgramBuilder<stadls::vx::v3::PlaybackProgramBuilder> const&);
} // namespace stadls::vx
