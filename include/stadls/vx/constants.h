#pragma once
#include "fisch/vx/constants.h"

namespace stadls::vx {

/** Playback memory FIFO size on FPGA in instructions for playback execution. */
constexpr size_t playback_memory_size_to_fpga GENPYBIND(visible) =
    fisch::vx::playback_memory_size_to_fpga;

} // namespace stadls::vx
