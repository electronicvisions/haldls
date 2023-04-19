#pragma once
#include "cereal/types/haldls/cereal.h"
#include "haldls/vx/readout.h"
#include <cereal/types/polymorphic.hpp>

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PadMultiplexerConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ReadoutSourceSelection& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PadMultiplexerConfig);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::ReadoutSourceSelection::SourceMultiplexer);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ReadoutSourceSelection);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_readout)
