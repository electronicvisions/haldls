#include "stadls/vx/v4/dumper.h"

#include "cereal/types/halco/hicann-dls/vx/v4/coordinate.h"
#include "cereal/types/haldls/cereal.tcc"
#include "stadls/vx/dumper.tcc"
#include "stadls/vx/playback_program_builder.tcc"

namespace stadls::vx {

namespace v4 {

PlaybackProgramBuilderDumper convert_to_dumper(DumperDone const& cocos)
{
	return detail::convert_to_builder<PlaybackProgramBuilderDumper>(cocos);
}

} // v4

template class stadls::vx::detail::Dumper<stadls::vx::v4::DumperDone>;

template std::ostream& stadls::vx::detail::operator<<(
    std::ostream&, stadls::vx::detail::Dumper<stadls::vx::v4::DumperDone> const&);

} // namespace stadls::vx
