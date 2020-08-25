#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/vector_generator.h"

namespace haldls::vx::v2 GENPYBIND_TAG_HALDLS_VX_V2 {

using VectorGeneratorControl GENPYBIND(visible) = haldls::vx::VectorGeneratorControl;
using VectorGeneratorLUTEntry GENPYBIND(visible) = haldls::vx::VectorGeneratorLUTEntry;

} // namespace haldls::vx::v2
