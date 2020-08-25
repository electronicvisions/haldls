#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/vector_generator.h"

namespace haldls::vx::v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using VectorGeneratorControl GENPYBIND(visible) = haldls::vx::VectorGeneratorControl;
using VectorGeneratorLUTEntry GENPYBIND(visible) = haldls::vx::VectorGeneratorLUTEntry;

} // namespace haldls::vx::v3
