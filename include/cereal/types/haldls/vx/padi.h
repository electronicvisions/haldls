#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx {

struct PADIEvent;
struct CommonPADIBusConfig;
struct CommonSTPConfig;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PADIEvent& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CommonPADIBusConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CommonSTPConfig& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PADIEvent);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CommonPADIBusConfig);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CommonSTPConfig);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_padi)
