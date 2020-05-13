#pragma once
#include "lola/vx/genpybind.h"
#include "lola/vx/neuron.h"

namespace lola::vx::v1 GENPYBIND_TAG_LOLA_VX_V1 {

using AtomicNeuron GENPYBIND(visible) = lola::vx::AtomicNeuron;

} // namespace lola::vx::v1
