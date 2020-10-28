#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/vector_generator.h"

namespace haldls::vx::v1 GENPYBIND_TAG_HALDLS_VX_V1 {

using VectorGeneratorControl GENPYBIND(visible) = haldls::vx::VectorGeneratorControl;
using VectorGeneratorLUTEntry GENPYBIND(visible) = haldls::vx::VectorGeneratorLUTEntry;
using VectorGeneratorNotificationAddress GENPYBIND(visible) =
    haldls::vx::VectorGeneratorNotificationAddress;
using VectorGeneratorTrigger GENPYBIND(visible) = haldls::vx::VectorGeneratorTrigger;
using VectorGeneratorFIFOWord GENPYBIND(visible) = haldls::vx::VectorGeneratorFIFOWord;

} // namespace haldls::vx::v1
