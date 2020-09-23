#pragma once
#include "haldls/cerealization.h"
#include "haldls/vx/v1/container.h"
#include "lola/vx/v1/container.h"
#include "stadls/vx/dumper.h"
#include "stadls/vx/genpybind.h"
#include "stadls/vx/playback_program_builder.h"
#include "stadls/vx/v1/coordinate_to_container.h"
#include "stadls/vx/v1/dumperdone.h"

namespace stadls::vx GENPYBIND_TAG_STADLS_VX_V1 {

namespace v1 GENPYBIND_TAG_STADLS_VX_V1 {

// Dumper is a detail, its template parameter DumperDone isn't.
// The latter is used by the PlaybackProgramBuilderDumper API.
// Hence, we don't wrap the detail, but only the public API type.
using Dumper GENPYBIND(hidden) = stadls::vx::detail::Dumper<DumperDone>;

} // namespace v1

extern template class SYMBOL_VISIBLE stadls::vx::detail::Dumper<stadls::vx::v1::DumperDone>;

namespace detail {

extern template SYMBOL_VISIBLE std::ostream& operator<<(
    std::ostream&,
    PlaybackProgramBuilderAdapter<
        stadls::vx::v1::Dumper,
        stadls::vx::v1::Dumper::done_type,
        stadls::vx::v1::CoordinateToContainer> const&);

} // namespace detail

extern template class SYMBOL_VISIBLE stadls::vx::detail::PlaybackProgramBuilderAdapter<
    stadls::vx::v1::Dumper,
    stadls::vx::v1::Dumper::done_type,
    stadls::vx::v1::CoordinateToContainer>;

namespace v1 GENPYBIND_TAG_STADLS_VX_V1 {

using PlaybackProgramBuilderDumper GENPYBIND(opaque) =
    detail::PlaybackProgramBuilderAdapter<Dumper, Dumper::done_type, CoordinateToContainer>;

GENPYBIND_MANUAL({
	haldls::vx::AddPickle<hate::type_list<stadls::vx::v1::PlaybackProgramBuilderDumper>>::apply(
	    parent, {"PlaybackProgramBuilderDumper"});
})

} // namespace v1

extern template SYMBOL_VISIBLE std::ostream& stadls::vx::detail::operator<<(
    std::ostream&, stadls::vx::detail::Dumper<stadls::vx::v1::DumperDone> const&);

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	extern template SYMBOL_VISIBLE void stadls::vx::v1::Dumper::write<Type>(                       \
	    typename Type::coordinate_type const&, Type const&);                                       \
	extern template SYMBOL_VISIBLE stadls::vx::PlaybackProgram::ContainerTicket<Type>              \
	stadls::vx::v1::Dumper::read<Type>(typename Type::coordinate_type const&);
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v1/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v1/container.def"

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
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

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::v1::Dumper)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::v1::PlaybackProgramBuilderDumper)

#if defined(__GENPYBIND_GENERATED__)
#include <pybind11/pybind11.h>
PYBIND11_MAKE_OPAQUE(stadls::vx::v1::Dumper::done_type)
#endif
