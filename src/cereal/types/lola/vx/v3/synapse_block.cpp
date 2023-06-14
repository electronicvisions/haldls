#include "cereal/types/lola/vx/v3/synapse_block.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_array.h"
#include "cereal/types/halco/common/typed_heap_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/lola/detail/hana.tcc"
#include "lola/vx/v3/synapse_block.h"
#include <cereal/types/variant.hpp>


EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::SynapseBlock)
CEREAL_REGISTER_TYPE(lola::vx::v3::SynapseBlock)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::v3::SynapseBlock)
CEREAL_REGISTER_DYNAMIC_INIT(lola_vx_v3_synapse_block)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::v3::SynapseBlock)
