#pragma once
#include "lola/vx/genpybind.h"
#include "lola/vx/system.h"

namespace lola::vx {
namespace v3 GENPYBIND_TAG_LOLA_VX_V3 {

using ChipAndMultichipJboaLeafFPGA GENPYBIND(visible) = lola::vx::ChipAndMultichipJboaLeafFPGA;
using ChipAndSinglechipFPGA GENPYBIND(visible) = lola::vx::ChipAndSinglechipFPGA;

} // namespace v3
} // namespace lola::vx
