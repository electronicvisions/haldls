#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx {

struct CrossbarOutputConfig;
struct CrossbarInputDropCounter;
struct CrossbarOutputEventCounter;
struct CrossbarNode;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CrossbarOutputConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CrossbarInputDropCounter& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CrossbarOutputEventCounter& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CrossbarNode& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CrossbarOutputConfig);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CrossbarInputDropCounter);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CrossbarOutputEventCounter);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CrossbarNode);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_routing_crossbar)
