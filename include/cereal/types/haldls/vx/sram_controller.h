#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx::detail {

struct SRAMTimingConfig;

} // namespace haldls::vx::detail

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::detail::SRAMTimingConfig& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::detail::SRAMTimingConfig);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_sram_controller)
