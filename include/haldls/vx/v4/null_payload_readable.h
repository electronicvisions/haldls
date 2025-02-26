#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/null_payload_readable.h"

namespace haldls::vx {
namespace v4 GENPYBIND_TAG_HALDLS_VX_V4 {

using NullPayloadReadable GENPYBIND(visible) = haldls::vx::NullPayloadReadable;

} // namespace v4
} // namespace haldls::vx
