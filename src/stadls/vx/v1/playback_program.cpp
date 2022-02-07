#include "stadls/vx/v1/playback_program.h"

#include "stadls/vx/playback_program.tcc"

#include "haldls/vx/v1/container.h"
#include "lola/vx/v1/container.h"

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	template class stadls::vx::PlaybackProgram::ContainerTicket<Type>;
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v1/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v1/container.def"

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::v1::PlaybackProgram);
