#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/routing_table_entry.h"

namespace haldls::vx {
namespace v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using OutputRoutingTableEntry GENPYBIND(visible) = haldls::vx::OutputRoutingTableEntry;
using InputRoutingTableEntry GENPYBIND(visible) = haldls::vx::InputRoutingTableEntry;

} // namespace v3
} // namespace haldls::vx
