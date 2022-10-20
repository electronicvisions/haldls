#pragma once
#include "haldls/vx/fpga.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx::v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using FPGADeviceDNA GENPYBIND(visible) = haldls::vx::FPGADeviceDNA;
using EventRecordingConfig GENPYBIND(visible) = haldls::vx::EventRecordingConfig;
using InstructionTimeoutConfig GENPYBIND(visible) = haldls::vx::InstructionTimeoutConfig;
using ExternalPPUMemoryByte GENPYBIND(visible) = haldls::vx::ExternalPPUMemoryByte;
using ExternalPPUMemoryQuad GENPYBIND(visible) = haldls::vx::ExternalPPUMemoryQuad;
using SpikeIOConfig GENPYBIND(visible) = haldls::vx::SpikeIOConfig;
using SpikeIOInputRoute GENPYBIND(visible) = haldls::vx::SpikeIOInputRoute;
using SpikeIOOutputRoute GENPYBIND(visible) = haldls::vx::SpikeIOOutputRoute;

} // namespace haldls::vx::v3
