#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace stadls::vx::v4 {

struct DumperDone;

} // namespace stadls::vx::v4

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, stadls::vx::v4::DumperDone& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(stadls::vx::v4::DumperDone);

CEREAL_FORCE_DYNAMIC_INIT(stadls_vx_v4_dumperdone)
