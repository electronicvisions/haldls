#include "cereal/types/stadls/vx/v4/playback_program_builder.h"

#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/stadls/vx/playback_program_builder.tcc"
#include "stadls/vx/v4/playback_program_builder.h"


EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(stadls::vx::v4::PlaybackProgramBuilder)

CEREAL_REGISTER_DYNAMIC_INIT(stadls_vx_v4_playback_program_builder)
