#include "cereal/types/stadls/vx/v4/dumper.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/stadls/vx/dumper.tcc"
#include "cereal/types/stadls/vx/playback_program_builder.tcc"
#include "stadls/vx/v4/dumper.h"
#include "stadls/vx/v4/dumperdone.h"

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(stadls::vx::detail::Dumper<stadls::vx::v4::DumperDone>);
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(stadls::vx::v4::PlaybackProgramBuilderDumper)

CEREAL_REGISTER_DYNAMIC_INIT(stadls_vx_v4_dumper)
