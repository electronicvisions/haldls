#pragma once
#include "cereal/types/haldls/cereal.h"


namespace stadls::vx::detail {

template <typename BuilderStorage, typename DoneType>
struct PlaybackProgramBuilderAdapter;

} // namespace stadls::vx::detail

namespace cereal {

template <typename Archive, typename BuilderStorage, typename DoneType>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    stadls::vx::detail::PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>& value,
    std::uint32_t const version);

} // namespace cereal
