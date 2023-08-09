#pragma once
#include "lola/vx/genpybind.h"
#include "lola/vx/ppu.h"

namespace lola::vx::v4 GENPYBIND_TAG_LOLA_VX_V4 {

using ExternalPPUMemoryBlock GENPYBIND(visible) = lola::vx::ExternalPPUMemoryBlock;
using ExternalPPUMemory GENPYBIND(visible) = lola::vx::ExternalPPUMemory;
using PPUProgram GENPYBIND(visible) = lola::vx::PPUProgram;
using PPUElfFile GENPYBIND(visible) = lola::vx::PPUElfFile;

} // namespace lola::vx::v4
