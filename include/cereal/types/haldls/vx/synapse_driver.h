#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx {

struct SynapseDriverConfig;
struct SynapseDriverSRAMTimingConfig;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SynapseDriverConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SynapseDriverSRAMTimingConfig& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SynapseDriverConfig);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SynapseDriverSRAMTimingConfig);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_synapse_driver)
