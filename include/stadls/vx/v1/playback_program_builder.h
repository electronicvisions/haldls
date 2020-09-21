#pragma once
#include "haldls/cerealization.h"
#include "haldls/vx/v1/container.h"
#include "lola/vx/v1/container.h"
#include "stadls/vx/genpybind.h"
#include "stadls/vx/playback_program_builder.h"
#include "stadls/vx/v1/dumper.h"

namespace stadls::vx {

namespace v1 {

/**
 * List of container types used to resolve a container type from a coordinate type.
 */
typedef hate::type_list<
#define PLAYBACK_CONTAINER(Name, Type) Type,
#include "haldls/vx/v1/container.def"
#define LAST_PLAYBACK_CONTAINER(Name, Type) Type
#define PLAYBACK_CONTAINER(Name, Type) LAST_PLAYBACK_CONTAINER(Name, Type),
#include "lola/vx/v1/container.def"
    >
    container_list;

/**
 * List of coordinate types sorted the same way as the container list.
 */
typedef hate::type_list<
#define PLAYBACK_CONTAINER(Name, Type) typename Type::coordinate_type,
#include "haldls/vx/v1/container.def"
#define LAST_PLAYBACK_CONTAINER(Name, Type) typename Type::coordinate_type
#define PLAYBACK_CONTAINER(Name, Type) LAST_PLAYBACK_CONTAINER(Name, Type),
#include "lola/vx/v1/container.def"
    >
    coordinate_list;

/**
 * Given a coordinate type, resolves the corresponding unique container type.
 * @tparam CoordinateT Coordinate type to resolve container type for
 */
template <typename CoordinateT>
struct CoordinateToContainer
{
	typedef typename hate::index_type_list_by_integer<
	    hate::index_type_list_by_type<CoordinateT, coordinate_list>::value,
	    container_list>::type type;
};

} // namespace v1

namespace detail {

extern template SYMBOL_VISIBLE std::ostream& operator<<(
    std::ostream&,
    PlaybackProgramBuilderAdapter<
        fisch::vx::PlaybackProgramBuilder,
        stadls::vx::PlaybackProgram,
        stadls::vx::v1::CoordinateToContainer> const&);

extern template SYMBOL_VISIBLE std::ostream& operator<<(
    std::ostream&,
    PlaybackProgramBuilderAdapter<
        stadls::vx::v1::Dumper,
        stadls::vx::v1::Dumper::done_type,
        stadls::vx::v1::CoordinateToContainer> const&);

} // namespace detail

extern template class SYMBOL_VISIBLE stadls::vx::detail::PlaybackProgramBuilderAdapter<
    fisch::vx::PlaybackProgramBuilder,
    stadls::vx::PlaybackProgram,
    stadls::vx::v1::CoordinateToContainer>;
extern template class SYMBOL_VISIBLE stadls::vx::detail::PlaybackProgramBuilderAdapter<
    stadls::vx::v1::Dumper,
    stadls::vx::v1::Dumper::done_type,
    stadls::vx::v1::CoordinateToContainer>;

namespace v1 GENPYBIND_TAG_STADLS_VX_V1 {

using PlaybackProgramBuilder GENPYBIND(opaque) = detail::PlaybackProgramBuilderAdapter<
    fisch::vx::PlaybackProgramBuilder,
    PlaybackProgram,
    CoordinateToContainer>;
using PlaybackProgramBuilderDumper GENPYBIND(opaque) =
    detail::PlaybackProgramBuilderAdapter<Dumper, Dumper::done_type, CoordinateToContainer>;

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
	haldls::vx::AddPickle<hate::type_list<stadls::vx::v1::PlaybackProgramBuilderDumper>>::apply(
	    parent, {"PlaybackProgramBuilderDumper"});
})

} // namespace v1

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	extern template void v1::PlaybackProgramBuilder::write<Type>(                                  \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    haldls::vx::Backend backend);                                                              \
	extern template void v1::PlaybackProgramBuilder::write<Type>(                                  \
	    typename Type::coordinate_type const& coord, Type const& config);                          \
	extern template void v1::PlaybackProgramBuilder::write<Type>(                                  \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference, haldls::vx::Backend backend);                                \
	extern template void v1::PlaybackProgramBuilder::write<Type>(                                  \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference);                                                             \
	extern template PlaybackProgram::ContainerTicket<Type>                                         \
	v1::PlaybackProgramBuilder::read<typename Type::coordinate_type>(                              \
	    typename Type::coordinate_type const& coord, haldls::vx::Backend backend);                 \
	extern template PlaybackProgram::ContainerTicket<Type>                                         \
	v1::PlaybackProgramBuilder::read<typename Type::coordinate_type>(                              \
	    typename Type::coordinate_type const& coord);                                              \
	extern template void v1::PlaybackProgramBuilderDumper::write<Type>(                            \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    haldls::vx::Backend backend);                                                              \
	extern template void v1::PlaybackProgramBuilderDumper::write<Type>(                            \
	    typename Type::coordinate_type const& coord, Type const& config);                          \
	extern template void v1::PlaybackProgramBuilderDumper::write<Type>(                            \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference, haldls::vx::Backend backend);                                \
	extern template void v1::PlaybackProgramBuilderDumper::write<Type>(                            \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference);                                                             \
	extern template PlaybackProgram::ContainerTicket<Type>                                         \
	v1::PlaybackProgramBuilderDumper::read<typename Type::coordinate_type>(                        \
	    typename Type::coordinate_type const& coord, haldls::vx::Backend backend);                 \
	extern template PlaybackProgram::ContainerTicket<Type>                                         \
	v1::PlaybackProgramBuilderDumper::read<typename Type::coordinate_type>(                        \
	    typename Type::coordinate_type const& coord);
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v1/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v1/container.def"

} // namespace stadls::vx

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::v1::PlaybackProgramBuilder)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::v1::PlaybackProgramBuilderDumper)
