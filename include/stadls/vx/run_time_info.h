#pragma once

#include "fisch/vx/run_time_info.h"
#include "stadls/vx/genpybind.h"

namespace stadls { namespace vx GENPYBIND_TAG_STADLS_VX {

typedef fisch::vx::RunTimeInfo RunTimeInfo GENPYBIND(visible);

} // namespace vx
} // namespace stadls
