#pragma once
#include "stadls/vx/genpybind.h"

namespace stadls::vx GENPYBIND_TAG_STADLS_VX {

enum class GENPYBIND(visible) ExecutorBackend
{
	hardware,
	simulation
};

} // namespace stadls::vx
