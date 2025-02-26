#pragma once
#include "lola/vx/dac.h"
#include "lola/vx/genpybind.h"

namespace lola::vx {
namespace v3 GENPYBIND_TAG_LOLA_VX_V3 {

using DACChannelBlock GENPYBIND(visible) = lola::vx::DACChannelBlock;
using DACControlBlock GENPYBIND(visible) = lola::vx::DACControlBlock;

} // namespace v3
} // namespace lola::vx
