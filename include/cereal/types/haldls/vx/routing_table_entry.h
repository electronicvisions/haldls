#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx {

struct OutputRoutingTableEntry;
struct InputRoutingTableEntry;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::OutputRoutingTableEntry& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::InputRoutingTableEntry& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::OutputRoutingTableEntry);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::InputRoutingTableEntry);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_routing_table_entry)
