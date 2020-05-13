#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/synapse.h"

namespace haldls::vx::v1 GENPYBIND_TAG_HALDLS_VX_V1 {

using CommonSynramConfig GENPYBIND(visible) = haldls::vx::CommonSynramConfig;
using SynapseWeightQuad GENPYBIND(visible) = haldls::vx::SynapseWeightQuad;
using SynapseLabelQuad GENPYBIND(visible) = haldls::vx::SynapseLabelQuad;
using SynapseCorrelationCalibQuad GENPYBIND(visible) = haldls::vx::SynapseCorrelationCalibQuad;
using SynapseQuad GENPYBIND(visible) = haldls::vx::SynapseQuad;
using ColumnCorrelationQuad GENPYBIND(visible) = haldls::vx::ColumnCorrelationQuad;
using ColumnCurrentQuad GENPYBIND(visible) = haldls::vx::ColumnCurrentQuad;
using SynapseBiasSelection GENPYBIND(visible) = haldls::vx::SynapseBiasSelection;
using CorrelationReset GENPYBIND(visible) = haldls::vx::CorrelationReset;

} // namespace haldls::vx::v1
