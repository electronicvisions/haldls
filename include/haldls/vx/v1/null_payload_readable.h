#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/null_payload_readable.h"

namespace haldls::vx::v1 GENPYBIND_TAG_HALDLS_VX_V1 {

using NullPayloadReadable GENPYBIND(visible) = haldls::vx::NullPayloadReadable;

} // namespace haldls::vx::v1
