#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx {

struct SystimeSyncBase;
struct SystimeSync;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SystimeSyncBase& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SystimeSync& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SystimeSyncBase);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SystimeSync);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_systime)
