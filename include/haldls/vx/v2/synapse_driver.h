#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/synapse_driver.h"

namespace fisch::vx {
class OmnibusChipOverJTAG;
class Omnibus;
} // namespace fisch::vx

namespace haldls::vx::v2 GENPYBIND_TAG_HALDLS_VX_V2 {

using SynapseDriverSRAMTimingConfig GENPYBIND(visible) = haldls::vx::SynapseDriverSRAMTimingConfig;
using SynapseDriverConfig GENPYBIND(visible) = haldls::vx::SynapseDriverConfig;

} // namespace haldls::vx::v2
