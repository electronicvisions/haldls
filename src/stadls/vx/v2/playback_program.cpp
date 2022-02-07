#include "stadls/vx/v2/playback_program.h"

#include "stadls/vx/playback_program.tcc"

#include "haldls/vx/v2/container.h"
#include "lola/vx/v2/container.h"

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	template class stadls::vx::PlaybackProgram::ContainerTicket<Type>;
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v2/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v2/container.def"

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::v2::PlaybackProgram);
