#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/vector_generator.h"

namespace haldls::vx { namespace v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using VectorGeneratorControl GENPYBIND(visible) = haldls::vx::VectorGeneratorControl;
using VectorGeneratorLUTEntry GENPYBIND(visible) = haldls::vx::VectorGeneratorLUTEntry;
using VectorGeneratorNotificationAddress GENPYBIND(visible) =
    haldls::vx::VectorGeneratorNotificationAddress;
using VectorGeneratorTrigger GENPYBIND(visible) = haldls::vx::VectorGeneratorTrigger;
using VectorGeneratorFIFOWord GENPYBIND(visible) = haldls::vx::VectorGeneratorFIFOWord;

} // namespace v3
} // namespace haldls::vx
