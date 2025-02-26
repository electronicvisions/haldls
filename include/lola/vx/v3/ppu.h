#pragma once
#include "lola/vx/genpybind.h"
#include "lola/vx/ppu.h"

namespace lola::vx { namespace v3 GENPYBIND_TAG_LOLA_VX_V3 {

using ExternalPPUMemoryBlock GENPYBIND(visible) = lola::vx::ExternalPPUMemoryBlock;
using ExternalPPUMemory GENPYBIND(visible) = lola::vx::ExternalPPUMemory;
using PPUProgram GENPYBIND(visible) = lola::vx::PPUProgram;
using PPUElfFile GENPYBIND(visible) = lola::vx::PPUElfFile;

} // namespace v3
} // namespace lola::vx
