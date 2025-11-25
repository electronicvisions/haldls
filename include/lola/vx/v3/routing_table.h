#pragma once
#include "lola/vx/genpybind.h"
#include "lola/vx/routing_table.h"

namespace lola::vx {
namespace v3 GENPYBIND_TAG_LOLA_VX_V3 {

using OutputRoutingTable GENPYBIND(visible) = lola::vx::OutputRoutingTable;
using InputRoutingTable GENPYBIND(visible) = lola::vx::InputRoutingTable;

} // namespace v3
} // namespace lola::vx
