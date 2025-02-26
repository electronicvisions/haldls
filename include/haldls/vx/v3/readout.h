#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/readout.h"

namespace haldls::vx {
namespace v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using PadMultiplexerConfig GENPYBIND(visible) = haldls::vx::PadMultiplexerConfig;
using ReadoutSourceSelection GENPYBIND(visible) = haldls::vx::ReadoutSourceSelection;

} // namespace v3
} // namespace haldls::vx
