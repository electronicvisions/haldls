#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx {

struct PPUMemoryWord;
struct PPUMemoryBlock;
struct PPUMemory;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PPUMemoryWord& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PPUMemoryBlock& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PPUMemory& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PPUMemoryWord);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PPUMemoryBlock);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PPUMemory);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_ppu)
