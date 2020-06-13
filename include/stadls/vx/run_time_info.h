#pragma once

#include "fisch/vx/run_time_info.h"
#include "stadls/vx/genpybind.h"

namespace stadls::vx GENPYBIND_TAG_STADLS_VX {

typedef fisch::vx::RunTimeInfo RunTimeInfo GENPYBIND(visible);

} // namespace stadls::vx
