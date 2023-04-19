#pragma once
#include "cereal/types/haldls/cereal.h"
#include "haldls/vx/v3/pll.h"
#include <cereal/types/polymorphic.hpp>

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::v3::PLLClockOutputBlock& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::v3::PLLClockOutputBlock::ClockOutput);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v3::PLLClockOutputBlock);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_v3_pll)
