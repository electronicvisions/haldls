#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/synapse.h"

namespace haldls::vx::v2 GENPYBIND_TAG_HALDLS_VX_V2 {

using CommonSynramConfig GENPYBIND(visible) = haldls::vx::CommonSynramConfig;
using SynapseBiasSelection GENPYBIND(visible) = haldls::vx::SynapseBiasSelection;
using SynapseWeightQuad GENPYBIND(visible) = haldls::vx::SynapseWeightQuad;
using SynapseLabelQuad GENPYBIND(visible) = haldls::vx::SynapseLabelQuad;
using SynapseCorrelationCalibQuad GENPYBIND(visible) = haldls::vx::SynapseCorrelationCalibQuad;
using SynapseQuad GENPYBIND(visible) = haldls::vx::SynapseQuad;
using ColumnCorrelationQuad GENPYBIND(visible) = haldls::vx::ColumnCorrelationQuad;
using ColumnCurrentQuad GENPYBIND(visible) = haldls::vx::ColumnCurrentQuad;
using CorrelationReset GENPYBIND(visible) = haldls::vx::CorrelationReset;

} // namespace haldls::vx::v2
