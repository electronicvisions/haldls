#include "cereal/types/lola/vx/fpga.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/lola/detail/hana.tcc"
#include "lola/vx/fpga.h"


EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::MultichipJboaLeafFPGA)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::SinglechipFPGA)

CEREAL_REGISTER_TYPE(lola::vx::MultichipJboaLeafFPGA)
CEREAL_REGISTER_TYPE(lola::vx::SinglechipFPGA)

CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::MultichipJboaLeafFPGA)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::SinglechipFPGA)

CEREAL_REGISTER_DYNAMIC_INIT(lola_vx_fpga)

CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::MultichipJboaLeafFPGA)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::SinglechipFPGA)