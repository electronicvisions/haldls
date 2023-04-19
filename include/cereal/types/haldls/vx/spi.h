#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx {

struct ShiftRegister;
struct DACChannel;
struct DACControl;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ShiftRegister& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::DACChannel& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::DACControl& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ShiftRegister);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::DACChannel);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::DACControl);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_spi)
