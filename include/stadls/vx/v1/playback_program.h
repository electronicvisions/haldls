#pragma once
#include "haldls/vx/v1/container.h"
#include "lola/vx/v1/container.h"
#include "stadls/vx/genpybind.h"
#include "stadls/vx/playback_program.h"

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	extern template class SYMBOL_VISIBLE stadls::vx::PlaybackProgram::ContainerTicket<Type>;
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v1/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v1/container.def"

namespace stadls::vx::v1 GENPYBIND_TAG_STADLS_VX_V1 {

// Explicit instantiation of template class for all valid playback container types.
#ifdef __GENPYBIND__
#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	typedef stadls::vx::PlaybackProgram::ContainerTicket<Type> ContainerTicket_##Name GENPYBIND(   \
	    opaque);
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v1/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v1/container.def"
#endif // __GENPYBIND__

using PlaybackProgram GENPYBIND(visible) = stadls::vx::PlaybackProgram;

} // namespace stadls::vx::v1

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::v1::PlaybackProgram);
