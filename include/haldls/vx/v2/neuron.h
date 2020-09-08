#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/neuron.h"

namespace haldls::vx::v2 GENPYBIND_TAG_HALDLS_VX_V2 {

using CommonNeuronBackendConfig GENPYBIND(visible) = haldls::vx::CommonNeuronBackendConfig;
using NeuronBackendConfig GENPYBIND(visible) = haldls::vx::NeuronBackendConfig;
using NeuronConfig GENPYBIND(visible) = haldls::vx::NeuronConfig;
using NeuronReset GENPYBIND(visible) = haldls::vx::NeuronReset;
using NeuronResetQuad GENPYBIND(visible) = haldls::vx::NeuronResetQuad;
using BlockPostPulse GENPYBIND(visible) = haldls::vx::BlockPostPulse;
using SpikeCounterRead GENPYBIND(visible) = haldls::vx::SpikeCounterRead;
using SpikeCounterReset GENPYBIND(visible) = haldls::vx::SpikeCounterReset;
using NeuronBackendSRAMTimingConfig GENPYBIND(visible) = haldls::vx::NeuronBackendSRAMTimingConfig;
using NeuronSRAMTimingConfig GENPYBIND(visible) = haldls::vx::NeuronSRAMTimingConfig;

} // namespace haldls::vx::v2
