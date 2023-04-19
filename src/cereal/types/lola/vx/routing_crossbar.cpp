#include "cereal/types/lola/vx/routing_crossbar.h"

#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/lola/detail/hana.tcc"
#include "halco/common/cerealization_typed_array.h"
#include "lola/vx/routing_crossbar.h"


EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::Crossbar)
CEREAL_REGISTER_TYPE(lola::vx::Crossbar)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::Crossbar)
CEREAL_REGISTER_DYNAMIC_INIT(lola_vx_routing_crossbar)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::Crossbar)
