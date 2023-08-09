#pragma once
#include "cereal/types/haldls/cereal.h"
#include "cereal/types/lola/detail/hana.h"
#include "lola/vx/v4/morphology.h"
#include <cereal/types/polymorphic.hpp>

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(lola::vx::v4::MCSafeAtomicNeuron::Multicompartment)
CEREAL_FORCE_DYNAMIC_INIT(lola_vx_v4_morphology)
