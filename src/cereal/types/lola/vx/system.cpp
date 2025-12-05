#include "cereal/types/lola/vx/system.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_array.h"
#include "cereal/types/halco/common/typed_heap_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/lola/detail/hana.tcc"
#include "cereal/types/lola/vx/v3/chip.h"
#include "lola/vx/system.h"
#include <cereal/types/variant.hpp>


EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::ChipAndMultichipJboaLeafFPGA)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::ChipAndSinglechipFPGA)

CEREAL_REGISTER_TYPE(lola::vx::ChipAndMultichipJboaLeafFPGA)
CEREAL_REGISTER_TYPE(lola::vx::ChipAndSinglechipFPGA)

CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::ChipAndMultichipJboaLeafFPGA)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::ChipAndSinglechipFPGA)

CEREAL_REGISTER_DYNAMIC_INIT(lola_vx_system)

CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::ChipAndMultichipJboaLeafFPGA)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::ChipAndSinglechipFPGA)