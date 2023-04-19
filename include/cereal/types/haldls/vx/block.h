#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx {

struct PollingOmnibusBlockConfig;
struct PollingOmnibusBlock;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PollingOmnibusBlockConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PollingOmnibusBlock& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PollingOmnibusBlockConfig);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PollingOmnibusBlock);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_block)
