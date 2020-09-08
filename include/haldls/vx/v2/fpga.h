#pragma once
#include "haldls/vx/fpga.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx::v2 GENPYBIND_TAG_HALDLS_VX_V2 {

using FPGADeviceDNA GENPYBIND(visible) = haldls::vx::FPGADeviceDNA;
using EventRecordingConfig GENPYBIND(visible) = haldls::vx::EventRecordingConfig;
using ExternalPPUMemoryByte GENPYBIND(visible) = haldls::vx::ExternalPPUMemoryByte;

} // namespace haldls::vx::v2
