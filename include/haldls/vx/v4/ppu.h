#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/ppu.h"

namespace haldls::vx::v4 GENPYBIND_TAG_HALDLS_VX_V4 {

using PPUMemoryWord GENPYBIND(visible) = haldls::vx::PPUMemoryWord;
using PPUMemoryBlock GENPYBIND(visible) = haldls::vx::PPUMemoryBlock;
using PPUMemory GENPYBIND(visible) = haldls::vx::PPUMemory;
using PPUControlRegister GENPYBIND(visible) = haldls::vx::PPUControlRegister;
using PPUStatusRegister GENPYBIND(visible) = haldls::vx::PPUStatusRegister;

} // namespace haldls::vx::v4
