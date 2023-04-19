#include "cereal/types/lola/vx/v3/cadc.h"

#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/lola/detail/hana.tcc"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/common/cerealization_typed_heap_array.h"
#include "lola/vx/v3/cadc.h"
#include <cereal/types/variant.hpp>


EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::CADCReadoutChain)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::CADCSampleRow)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::CADCSamples)
CEREAL_REGISTER_TYPE(lola::vx::v3::CADCReadoutChain)
CEREAL_REGISTER_TYPE(lola::vx::v3::CADCSampleRow)
CEREAL_REGISTER_TYPE(lola::vx::v3::CADCSamples)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::v3::CADCReadoutChain)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::v3::CADCSampleRow)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::v3::CADCSamples)
CEREAL_REGISTER_DYNAMIC_INIT(lola_vx_v3_cadc)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::v3::CADCReadoutChain)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::v3::CADCSampleRow)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::v3::CADCSamples)
