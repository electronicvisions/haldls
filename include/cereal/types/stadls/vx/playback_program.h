#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace stadls::vx {
struct PlaybackProgram;
} // namespace stadls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, stadls::vx::PlaybackProgram& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(stadls::vx::PlaybackProgram);

CEREAL_FORCE_DYNAMIC_INIT(stadls_vx_playback_program)
