#pragma once
#include "haldls/vx/v2/container.h"
#include "lola/vx/v2/container.h"
#include "stadls/vx/genpybind.h"
#include "stadls/vx/playback_program.h"

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	extern template class SYMBOL_VISIBLE stadls::vx::PlaybackProgram::ContainerTicket<Type>;
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v2/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v2/container.def"

namespace stadls::vx::v2 GENPYBIND_TAG_STADLS_VX_V2 {

// Explicit instantiation of template class for all valid playback container types.
#ifdef __GENPYBIND__
#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	typedef stadls::vx::PlaybackProgram::ContainerTicket<Type> ContainerTicket_##Name GENPYBIND(   \
	    opaque);
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v2/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v2/container.def"
#endif // __GENPYBIND__

using PlaybackProgram GENPYBIND(visible) = stadls::vx::PlaybackProgram;

} // namespace stadls::vx::v2
