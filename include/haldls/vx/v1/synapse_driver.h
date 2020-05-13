#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/synapse_driver.h"

namespace haldls::vx::v1 GENPYBIND_TAG_HALDLS_VX_V1 {

using SynapseDriverConfig GENPYBIND(visible) = haldls::vx::SynapseDriverConfig;
using SynapseDriverSRAMTimingConfig GENPYBIND(visible) = haldls::vx::SynapseDriverSRAMTimingConfig;

} // namespace haldls::vx::v1
