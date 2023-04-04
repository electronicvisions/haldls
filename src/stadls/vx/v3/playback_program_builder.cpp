#include "stadls/vx/v3/playback_program_builder.h"

#include "haldls/cerealization.tcc"
#include "haldls/vx/common.h"
#include "haldls/vx/v3/container.h"
#include "lola/vx/v3/container.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.tcc"

namespace stadls::vx::detail {

template class PlaybackProgramBuilderAdapter<
    fisch::vx::PlaybackProgramBuilder,
    stadls::vx::PlaybackProgram,
    stadls::vx::v3::CoordinateToContainer>;
template class PlaybackProgramBuilderAdapter<
    stadls::vx::v3::Dumper,
    stadls::vx::v3::Dumper::done_type,
    stadls::vx::v3::CoordinateToContainer>;

template std::ostream& operator<<(
    std::ostream&,
    PlaybackProgramBuilderAdapter<
        fisch::vx::PlaybackProgramBuilder,
        stadls::vx::PlaybackProgram,
        stadls::vx::v3::CoordinateToContainer> const&);

template std::ostream& operator<<(
    std::ostream&,
    PlaybackProgramBuilderAdapter<
        stadls::vx::v3::Dumper,
        stadls::vx::v3::Dumper::done_type,
        stadls::vx::v3::CoordinateToContainer> const&);

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

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	template void v3::PlaybackProgramBuilder::write<Type>(                                         \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    haldls::vx::Backend backend);                                                              \
	template void v3::PlaybackProgramBuilder::write<Type>(                                         \
	    typename Type::coordinate_type const& coord, Type const& config);                          \
	template void v3::PlaybackProgramBuilder::write<Type>(                                         \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference, haldls::vx::Backend backend);                                \
	template void v3::PlaybackProgramBuilder::write<Type>(                                         \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference);                                                             \
	template PlaybackProgram::ContainerTicket<Type>                                                \
	v3::PlaybackProgramBuilder::read<typename Type::coordinate_type>(                              \
	    typename Type::coordinate_type const& coord, haldls::vx::Backend backend);                 \
	template PlaybackProgram::ContainerTicket<Type>                                                \
	v3::PlaybackProgramBuilder::read<typename Type::coordinate_type>(                              \
	    typename Type::coordinate_type const& coord);                                              \
	template void v3::PlaybackProgramBuilderDumper::write<Type>(                                   \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    haldls::vx::Backend backend);                                                              \
	template void v3::PlaybackProgramBuilderDumper::write<Type>(                                   \
	    typename Type::coordinate_type const& coord, Type const& config);                          \
	template void v3::PlaybackProgramBuilderDumper::write<Type>(                                   \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference, haldls::vx::Backend backend);                                \
	template void v3::PlaybackProgramBuilderDumper::write<Type>(                                   \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference);                                                             \
	template PlaybackProgram::ContainerTicket<Type>                                                \
	v3::PlaybackProgramBuilderDumper::read<typename Type::coordinate_type>(                        \
	    typename Type::coordinate_type const& coord, haldls::vx::Backend backend);                 \
	template PlaybackProgram::ContainerTicket<Type>                                                \
	v3::PlaybackProgramBuilderDumper::read<typename Type::coordinate_type>(                        \
	    typename Type::coordinate_type const& coord);
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v3/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v3/container.def"

} // namespace stadls::vx

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::v3::PlaybackProgramBuilder)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::v3::PlaybackProgramBuilderDumper)
