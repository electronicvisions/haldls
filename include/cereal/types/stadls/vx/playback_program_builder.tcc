#pragma once
#include "cereal/types/haldls/cereal.h"
#include "cereal/types/stadls/vx/playback_program_builder.h"
#include <stdexcept>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_set.hpp>

namespace fisch::vx {

struct PlaybackProgramBuilder;

} // namespace fisch::vx


namespace cereal {

template <typename Archive, typename BuilderStorage, typename DoneType>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    stadls::vx::detail::PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>& value,
    std::uint32_t const /* version */)
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		throw std::runtime_error(
		    "fisch-based PlaybackProgramBuilderAdapter doesn't support serialization");
	} else {
		ar(CEREAL_NVP(value.m_builder_impl));
		ar(CEREAL_NVP(value.m_unsupported_targets));
	}
}

} // namespace cereal
