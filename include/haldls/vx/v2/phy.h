#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/phy.h"

namespace haldls::vx::v2 GENPYBIND_TAG_HALDLS_VX_V2 {

using PhyConfigFPGA GENPYBIND(visible) = haldls::vx::PhyConfigFPGA;
using PhyConfigChip GENPYBIND(visible) = haldls::vx::PhyConfigChip;
using CommonPhyConfigFPGA GENPYBIND(visible) = haldls::vx::CommonPhyConfigFPGA;
using CommonPhyConfigChip GENPYBIND(visible) = haldls::vx::CommonPhyConfigChip;
using PhyStatus GENPYBIND(visible) = haldls::vx::PhyStatus;

} // namespace haldls::vx::v2
