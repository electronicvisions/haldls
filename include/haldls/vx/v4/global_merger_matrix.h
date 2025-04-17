#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/global_merger_matrix.h"

namespace haldls::vx {
namespace v4 GENPYBIND_TAG_HALDLS_VX_V4 {

using GlobalMergerMatrixOutputConfig GENPYBIND(visible) =
    haldls::vx::GlobalMergerMatrixOutputConfig;
using GlobalMergerMatrixInputDropCounter GENPYBIND(visible) =
    haldls::vx::GlobalMergerMatrixInputDropCounter;
using GlobalMergerMatrixOutputEventCounter GENPYBIND(visible) =
    haldls::vx::GlobalMergerMatrixOutputEventCounter;
using GlobalMergerMatrixNode GENPYBIND(visible) = haldls::vx::GlobalMergerMatrixNode;
using NeuronLabelToGL1EventLUTEntry GENPYBIND(visible) = haldls::vx::NeuronLabelToGL1EventLUTEntry;

} // namespace v4
} // namespace haldls::vx
