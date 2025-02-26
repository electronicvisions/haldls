#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/synapse_driver.h"

namespace fisch::vx {
class OmnibusChipOverJTAG;
class Omnibus;
} // namespace fisch::vx

namespace haldls::vx {
namespace v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using SynapseDriverSRAMTimingConfig GENPYBIND(visible) = haldls::vx::SynapseDriverSRAMTimingConfig;
using SynapseDriverConfig GENPYBIND(visible) = haldls::vx::SynapseDriverConfig;

} // namespace v3
} // namespace haldls::vx
