#pragma once
#include "cereal/types/lola/detail/hana.h"
#include <cereal/types/polymorphic.hpp>

namespace lola::vx {

struct ExternalPPUMemoryBlock;
struct ExternalPPUDRAMMemoryBlock;

} // namespace lola::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, lola::vx::ExternalPPUMemoryBlock& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, lola::vx::ExternalPPUDRAMMemoryBlock& value, std::uint32_t const version);

} // namespace cereal

CEREAL_FORCE_DYNAMIC_INIT(lola_vx_ppu)
