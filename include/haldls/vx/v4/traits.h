#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"

namespace haldls::vx {
namespace v4 GENPYBIND_TAG_HALDLS_VX_V4 {

using DifferentialWriteTrait GENPYBIND(visible) = haldls::vx::DifferentialWriteTrait;
using Backend GENPYBIND(visible) = haldls::vx::Backend;
using BackendContainerList GENPYBIND(visible) = haldls::vx::BackendContainerList;

} // namespace v4
} // namespace haldls::vx
