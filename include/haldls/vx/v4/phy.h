#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/phy.h"

namespace haldls::vx {
namespace v4 GENPYBIND_TAG_HALDLS_VX_V4 {

using PhyConfigFPGA GENPYBIND(visible) = haldls::vx::PhyConfigFPGA;
using PhyConfigChip GENPYBIND(visible) = haldls::vx::PhyConfigChip;
using CommonPhyConfigFPGA GENPYBIND(visible) = haldls::vx::CommonPhyConfigFPGA;
using CommonPhyConfigChip GENPYBIND(visible) = haldls::vx::CommonPhyConfigChip;
using PhyStatus GENPYBIND(visible) = haldls::vx::PhyStatus;

} // namespace v4
} // namespace haldls::vx
