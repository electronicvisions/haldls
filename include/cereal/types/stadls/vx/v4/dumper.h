#pragma once
#include "cereal/types/haldls/cereal.h"
#include "cereal/types/stadls/vx/dumper.h"
#include "cereal/types/stadls/vx/playback_program_builder.h"
#include <cereal/types/polymorphic.hpp>

namespace stadls::vx::v4 {

struct DumperDone;

typedef stadls::vx::detail::PlaybackProgramBuilderAdapter<
    stadls::vx::detail::Dumper<stadls::vx::v4::DumperDone>,
    stadls::vx::v4::DumperDone>
    PlaybackProgramBuilderDumper;

} // namespace stadls::vx::v4

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(stadls::vx::detail::Dumper<stadls::vx::v4::DumperDone>);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(stadls::vx::v4::PlaybackProgramBuilderDumper)

CEREAL_FORCE_DYNAMIC_INIT(stadls_vx_v4_dumper)
