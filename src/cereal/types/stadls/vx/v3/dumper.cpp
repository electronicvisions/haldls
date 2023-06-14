#include "cereal/types/stadls/vx/v3/dumper.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/stadls/vx/dumper.tcc"
#include "cereal/types/stadls/vx/playback_program_builder.tcc"
#include "stadls/vx/v3/dumper.h"
#include "stadls/vx/v3/dumperdone.h"

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(stadls::vx::detail::Dumper<stadls::vx::v3::DumperDone>);
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(stadls::vx::v3::PlaybackProgramBuilderDumper)

CEREAL_REGISTER_DYNAMIC_INIT(stadls_vx_v3_dumper)
