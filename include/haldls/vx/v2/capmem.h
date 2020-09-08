#pragma once
#include "haldls/vx/capmem.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx::v2 GENPYBIND_TAG_HALDLS_VX_V2 {

using CapMemCell GENPYBIND(visible) = haldls::vx::CapMemCell;
using CapMemBlock GENPYBIND(visible) = haldls::vx::CapMemBlock;
using CapMemBlockConfig GENPYBIND(visible) = haldls::vx::CapMemBlockConfig;
using ReferenceGeneratorConfig GENPYBIND(visible) = haldls::vx::ReferenceGeneratorConfig;

} // namespace haldls::vx::v2
