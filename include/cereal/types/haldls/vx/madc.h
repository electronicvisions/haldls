#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx {

struct MADCControl;
struct MADCConfig;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::MADCControl& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::MADCConfig& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::MADCControl);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::MADCConfig);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_madc)
