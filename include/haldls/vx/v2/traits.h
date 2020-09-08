#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"

namespace haldls::vx::v2 GENPYBIND_TAG_HALDLS_VX_V2 {

using DifferentialWriteTrait GENPYBIND(visible) = haldls::vx::DifferentialWriteTrait;
using Backend GENPYBIND(visible) = haldls::vx::Backend;
using BackendContainerList GENPYBIND(visible) = haldls::vx::BackendContainerList;

} // namespace haldls::vx::v2
