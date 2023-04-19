#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx {

struct JTAGClockScaler;
struct ResetJTAGTap;
struct JTAGIdCode;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::JTAGClockScaler& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ResetJTAGTap& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::JTAGIdCode& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::JTAGClockScaler);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ResetJTAGTap);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::JTAGIdCode);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_jtag)
