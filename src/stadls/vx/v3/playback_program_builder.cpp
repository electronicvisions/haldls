#include "stadls/vx/v3/playback_program_builder.h"

#include "haldls/vx/common.h"
#include "haldls/vx/v3/container.h"
#include "lola/vx/v3/container.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.tcc"

namespace stadls::vx::detail {

template class PlaybackProgramBuilderAdapter<
    fisch::vx::PlaybackProgramBuilder,
    stadls::vx::PlaybackProgram>;
template class PlaybackProgramBuilderAdapter<
    stadls::vx::v3::Dumper,
    stadls::vx::v3::Dumper::done_type>;

template std::ostream& operator<<(
    std::ostream&,
    PlaybackProgramBuilderAdapter<
        fisch::vx::PlaybackProgramBuilder,
        stadls::vx::PlaybackProgram> const&);

template std::ostream& operator<<(
    std::ostream&,
    PlaybackProgramBuilderAdapter<
        stadls::vx::v3::Dumper,
        stadls::vx::v3::Dumper::done_type> const&);

} // namespace stadls::vx::detail

namespace stadls::vx {

namespace v3 {

PlaybackProgramBuilder convert_to_builder(PlaybackProgramBuilderDumper& dumper)
{
	return convert_to_builder(dumper.done());
}

PlaybackProgramBuilder convert_to_builder(PlaybackProgramBuilderDumper&& dumper)
{
	return convert_to_builder(dumper.done());
}

PlaybackProgramBuilder convert_to_builder(Dumper::done_type const& cocos)
{
	return detail::convert_to_builder<PlaybackProgramBuilder>(cocos);
}

} // namespace v3

} // namespace stadls::vx
