#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/vector_generator.h"

namespace haldls::vx::v1 GENPYBIND_TAG_HALDLS_VX_V1 {

using VectorGeneratorControl GENPYBIND(visible) = haldls::vx::VectorGeneratorControl;
using VectorGeneratorLUTEntry GENPYBIND(visible) = haldls::vx::VectorGeneratorLUTEntry;

} // namespace haldls::vx::v1
