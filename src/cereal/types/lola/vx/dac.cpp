#include "cereal/types/lola/vx/dac.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/lola/detail/hana.tcc"
#include "lola/vx/dac.h"


EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::DACChannelBlock)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::DACControlBlock)
CEREAL_REGISTER_TYPE(lola::vx::DACChannelBlock)
CEREAL_REGISTER_TYPE(lola::vx::DACControlBlock)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::DACChannelBlock)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::DACControlBlock)

CEREAL_REGISTER_DYNAMIC_INIT(lola_vx_dac)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::DACChannelBlock)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::DACControlBlock)
