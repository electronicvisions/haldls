#pragma once
#include "lola/vx/genpybind.h"
#include "lola/vx/synapse.h"

namespace lola::vx::v1 GENPYBIND_TAG_LOLA_VX_V1 {

using SynapseWeightRow GENPYBIND(visible) = lola::vx::SynapseWeightRow;
using SynapseLabelRow GENPYBIND(visible) = lola::vx::SynapseLabelRow;
using SynapseCorrelationCalibRow GENPYBIND(visible) = lola::vx::SynapseCorrelationCalibRow;
using SynapseRow GENPYBIND(visible) = lola::vx::SynapseRow;
using SynapseWeightMatrix GENPYBIND(visible) = lola::vx::SynapseWeightMatrix;
using SynapseLabelMatrix GENPYBIND(visible) = lola::vx::SynapseLabelMatrix;
using SynapseCorrelationCalibMatrix GENPYBIND(visible) = lola::vx::SynapseCorrelationCalibMatrix;
using SynapseMatrix GENPYBIND(visible) = lola::vx::SynapseMatrix;
using CorrelationResetRow GENPYBIND(visible) = lola::vx::CorrelationResetRow;

} // namespace lola::vx::v1
