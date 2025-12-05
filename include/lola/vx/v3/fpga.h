#pragma once
#include "lola/vx/fpga.h"
#include "lola/vx/genpybind.h"

namespace lola::vx {
namespace v3 GENPYBIND_TAG_LOLA_VX_V3 {

using MultichipJboaLeafFPGA GENPYBIND(visible) = lola::vx::MultichipJboaLeafFPGA;
using SinglechipFPGA GENPYBIND(visible) = lola::vx::SinglechipFPGA;

} // namespace v3
} // namespace lola::vx
