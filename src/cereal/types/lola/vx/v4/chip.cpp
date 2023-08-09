#include "cereal/types/lola/vx/v4/chip.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_array.h"
#include "cereal/types/halco/common/typed_heap_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/haldls/vx/readout.h"
#include "cereal/types/haldls/vx/synapse.h"
#include "cereal/types/lola/detail/hana.tcc"
#include "lola/vx/v4/chip.h"
#include <cereal/types/variant.hpp>


EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v4::Chip)
CEREAL_REGISTER_TYPE(lola::vx::v4::Chip)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::v4::Chip)
CEREAL_REGISTER_DYNAMIC_INIT(lola_vx_v4_chip)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::v4::Chip)
