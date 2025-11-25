#include "cereal/types/lola/vx/routing_table.h"

#include "cereal/types/halco/common/typed_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/lola/detail/hana.tcc"
#include "lola/vx/routing_table.h"


EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::OutputRoutingTable)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::InputRoutingTable)

CEREAL_REGISTER_TYPE(lola::vx::OutputRoutingTable)
CEREAL_REGISTER_TYPE(lola::vx::InputRoutingTable)

CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::OutputRoutingTable)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::InputRoutingTable)

CEREAL_REGISTER_DYNAMIC_INIT(lola_vx_routing_table)

CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::OutputRoutingTable)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::InputRoutingTable)
