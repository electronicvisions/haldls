#pragma once
#include "haldls/vx/fpga.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx { namespace v4 GENPYBIND_TAG_HALDLS_VX_V4 {

using FPGASystimeSyncNumRetries GENPYBIND(visible) = haldls::vx::FPGASystimeSyncNumRetries;
using FPGASystimeSyncLastRTT GENPYBIND(visible) = haldls::vx::FPGASystimeSyncLastRTT;
using FPGASystimeSyncLastAsicSystime GENPYBIND(visible) =
    haldls::vx::FPGASystimeSyncLastAsicSystime;
using FPGASystimeSyncActiveState GENPYBIND(visible) = haldls::vx::FPGASystimeSyncActiveState;
using FPGADeviceDNA GENPYBIND(visible) = haldls::vx::FPGADeviceDNA;
using EventRecordingConfig GENPYBIND(visible) = haldls::vx::EventRecordingConfig;
using InstructionTimeoutConfig GENPYBIND(visible) = haldls::vx::InstructionTimeoutConfig;
using SystimeCorrectionBarrierConfig GENPYBIND(visible) =
    haldls::vx::SystimeCorrectionBarrierConfig;
using ExternalPPUMemoryByte GENPYBIND(visible) = haldls::vx::ExternalPPUMemoryByte;
using ExternalPPUMemoryQuad GENPYBIND(visible) = haldls::vx::ExternalPPUMemoryQuad;
using SpikeIOConfig GENPYBIND(visible) = haldls::vx::SpikeIOConfig;
using SpikeIOInputRoute GENPYBIND(visible) = haldls::vx::SpikeIOInputRoute;
using SpikeIOOutputRoute GENPYBIND(visible) = haldls::vx::SpikeIOOutputRoute;

} // namespace v4
} // namespace haldls::vx
