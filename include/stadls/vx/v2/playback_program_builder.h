#pragma once
#include "stadls/vx/genpybind.h"
#include "stadls/vx/playback_program_builder.h"
#include "stadls/vx/v2/dumper.h"

namespace stadls::vx {

namespace detail {

extern template SYMBOL_VISIBLE std::ostream& operator<<(
    std::ostream&,
    PlaybackProgramBuilderAdapter<
        fisch::vx::PlaybackProgramBuilder,
        stadls::vx::PlaybackProgram> const&);

extern template SYMBOL_VISIBLE std::ostream& operator<<(
    std::ostream&,
    PlaybackProgramBuilderAdapter<
        stadls::vx::v2::Dumper,
        stadls::vx::v2::Dumper::done_type> const&);

} // namespace detail

extern template class SYMBOL_VISIBLE stadls::vx::detail::
    PlaybackProgramBuilderAdapter<fisch::vx::PlaybackProgramBuilder, stadls::vx::PlaybackProgram>;
extern template class SYMBOL_VISIBLE stadls::vx::detail::
    PlaybackProgramBuilderAdapter<stadls::vx::v2::Dumper, stadls::vx::v2::Dumper::done_type>;

namespace v2 GENPYBIND_TAG_STADLS_VX_V2 {

using PlaybackProgramBuilder GENPYBIND(opaque) =
    detail::PlaybackProgramBuilderAdapter<fisch::vx::PlaybackProgramBuilder, PlaybackProgram>;
using PlaybackProgramBuilderDumper GENPYBIND(opaque) =
    detail::PlaybackProgramBuilderAdapter<Dumper, Dumper::done_type>;

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

GENPYBIND_MANUAL({
	haldls::vx::AddPickle<hate::type_list<stadls::vx::v2::PlaybackProgramBuilderDumper>>::apply(
	    parent, {"PlaybackProgramBuilderDumper"});
})

} // namespace v2

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	extern template void v2::PlaybackProgramBuilder::write<Type>(                                  \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    haldls::vx::Backend backend);                                                              \
	extern template void v2::PlaybackProgramBuilder::write<Type>(                                  \
	    typename Type::coordinate_type const& coord, Type const& config);                          \
	extern template void v2::PlaybackProgramBuilder::write<Type>(                                  \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference, haldls::vx::Backend backend);                                \
	extern template void v2::PlaybackProgramBuilder::write<Type>(                                  \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference);                                                             \
	extern template PlaybackProgram::ContainerTicket<Type>                                         \
	v2::PlaybackProgramBuilder::read<typename Type::coordinate_type>(                              \
	    typename Type::coordinate_type const& coord, haldls::vx::Backend backend);                 \
	extern template PlaybackProgram::ContainerTicket<Type>                                         \
	v2::PlaybackProgramBuilder::read<typename Type::coordinate_type>(                              \
	    typename Type::coordinate_type const& coord);                                              \
	extern template void v2::PlaybackProgramBuilderDumper::write<Type>(                            \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    haldls::vx::Backend backend);                                                              \
	extern template void v2::PlaybackProgramBuilderDumper::write<Type>(                            \
	    typename Type::coordinate_type const& coord, Type const& config);                          \
	extern template void v2::PlaybackProgramBuilderDumper::write<Type>(                            \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference, haldls::vx::Backend backend);                                \
	extern template void v2::PlaybackProgramBuilderDumper::write<Type>(                            \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference);                                                             \
	extern template PlaybackProgram::ContainerTicket<Type>                                         \
	v2::PlaybackProgramBuilderDumper::read<typename Type::coordinate_type>(                        \
	    typename Type::coordinate_type const& coord, haldls::vx::Backend backend);                 \
	extern template PlaybackProgram::ContainerTicket<Type>                                         \
	v2::PlaybackProgramBuilderDumper::read<typename Type::coordinate_type>(                        \
	    typename Type::coordinate_type const& coord);
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v2/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v2/container.def"

} // namespace stadls::vx
