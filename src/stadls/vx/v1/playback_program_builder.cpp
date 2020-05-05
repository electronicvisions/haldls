#include "stadls/vx/v1/playback_program_builder.h"

#include "haldls/cerealization.tcc"
#include "haldls/vx/common.h"
#include "haldls/vx/v1/container.h"
#include "lola/vx/v1/container.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.tcc"

namespace stadls::vx::detail {

template class PlaybackProgramBuilderAdapter<
    fisch::vx::PlaybackProgramBuilder,
    stadls::vx::PlaybackProgram,
    stadls::vx::v1::CoordinateToContainer>;
template class PlaybackProgramBuilderAdapter<
    stadls::vx::v1::Dumper,
    stadls::vx::v1::Dumper::done_type,
    stadls::vx::v1::CoordinateToContainer>;

template std::ostream& operator<<(
    std::ostream&,
    PlaybackProgramBuilderAdapter<
        fisch::vx::PlaybackProgramBuilder,
        stadls::vx::PlaybackProgram,
        stadls::vx::v1::CoordinateToContainer> const&);

template std::ostream& operator<<(
    std::ostream&,
    PlaybackProgramBuilderAdapter<
        stadls::vx::v1::Dumper,
        stadls::vx::v1::Dumper::done_type,
        stadls::vx::v1::CoordinateToContainer> const&);

} // namespace stadls::vx::detail

namespace stadls::vx {

namespace v1 {

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
	typedef hate::type_list<
	    haldls::vx::Timer::Value, haldls::vx::Barrier, haldls::vx::PollingOmnibusBlock>
	    block_types;
	PlaybackProgramBuilder builder;
	for (auto const& coco : cocos.values) {
		std::visit(
		    [&builder](auto const& cc) {
			    auto const& [coord, config] = cc;
			    typedef std::remove_cv_t<std::remove_reference_t<decltype(config)>> config_type;
			    if constexpr (hate::is_in_type_list<config_type, block_types>::value) {
				    builder.block_until(coord, config);
			    } else {
				    builder.write(coord, config);
			    }
		    },
		    coco);
	}
	return builder;
}

} // namespace v1

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	template void v1::PlaybackProgramBuilder::write<Type>(                                         \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    haldls::vx::Backend backend);                                                              \
	template void v1::PlaybackProgramBuilder::write<Type>(                                         \
	    typename Type::coordinate_type const& coord, Type const& config);                          \
	template void v1::PlaybackProgramBuilder::write<Type>(                                         \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference, haldls::vx::Backend backend);                                \
	template void v1::PlaybackProgramBuilder::write<Type>(                                         \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference);                                                             \
	template PlaybackProgram::ContainerTicket<Type>                                                \
	v1::PlaybackProgramBuilder::read<typename Type::coordinate_type>(                              \
	    typename Type::coordinate_type const& coord, haldls::vx::Backend backend);                 \
	template PlaybackProgram::ContainerTicket<Type>                                                \
	v1::PlaybackProgramBuilder::read<typename Type::coordinate_type>(                              \
	    typename Type::coordinate_type const& coord);                                              \
	template void v1::PlaybackProgramBuilderDumper::write<Type>(                                   \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    haldls::vx::Backend backend);                                                              \
	template void v1::PlaybackProgramBuilderDumper::write<Type>(                                   \
	    typename Type::coordinate_type const& coord, Type const& config);                          \
	template void v1::PlaybackProgramBuilderDumper::write<Type>(                                   \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference, haldls::vx::Backend backend);                                \
	template void v1::PlaybackProgramBuilderDumper::write<Type>(                                   \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference);                                                             \
	template PlaybackProgram::ContainerTicket<Type>                                                \
	v1::PlaybackProgramBuilderDumper::read<typename Type::coordinate_type>(                        \
	    typename Type::coordinate_type const& coord, haldls::vx::Backend backend);                 \
	template PlaybackProgram::ContainerTicket<Type>                                                \
	v1::PlaybackProgramBuilderDumper::read<typename Type::coordinate_type>(                        \
	    typename Type::coordinate_type const& coord);
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v1/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v1/container.def"

} // namespace stadls::vx

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::v1::PlaybackProgramBuilder)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::v1::PlaybackProgramBuilderDumper)
