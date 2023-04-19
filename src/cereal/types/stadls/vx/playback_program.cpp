#include "cereal/types/stadls/vx/playback_program.h"

#include "cereal/types/haldls/cereal.tcc"
#include "stadls/vx/playback_program.h"
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_set.hpp>

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, stadls::vx::PlaybackProgram& value, std::uint32_t const /* version */)
{
	ar(CEREAL_NVP(value.m_program_impl));
	ar(CEREAL_NVP(value.m_unsupported_targets));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(stadls::vx::PlaybackProgram);

CEREAL_REGISTER_DYNAMIC_INIT(stadls_vx_playback_program)
