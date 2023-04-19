#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx {

struct PerfTest;
struct PerfTestStatus;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PerfTest& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PerfTestStatus& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PerfTest);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PerfTestStatus);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_perftest)
