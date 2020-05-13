#pragma once
#include "lola/vx/cadc.h"
#include "lola/vx/genpybind.h"

namespace lola::vx::v1 GENPYBIND_TAG_LOLA_VX_V1 {

using CADCSampleRow GENPYBIND(visible) = lola::vx::CADCSampleRow;
using CADCSamples GENPYBIND(visible) = lola::vx::CADCSamples;

} // namespace lola::vx::v1
