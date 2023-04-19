#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx {

struct ADPLL;
struct PLLSelfTest;
struct PLLSelfTestStatus;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ADPLL& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PLLSelfTest& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PLLSelfTestStatus& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ADPLL);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PLLSelfTest);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PLLSelfTestStatus);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_pll)
