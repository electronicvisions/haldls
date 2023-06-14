#include "cereal/types/lola/vx/synapse.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_array.h"
#include "cereal/types/halco/common/typed_heap_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/haldls/vx/synapse.h"
#include "cereal/types/lola/detail/hana.tcc"
#include "lola/vx/synapse.h"


EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::ColumnCorrelationRow)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::ColumnCurrentRow)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::SynapseRow)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::SynapseWeightRow)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::SynapseLabelRow)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::SynapseCorrelationCalibRow)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::SynapseWeightMatrix)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::SynapseLabelMatrix)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::SynapseCorrelationCalibMatrix)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::SynapseMatrix)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::CorrelationResetRow)
CEREAL_REGISTER_TYPE(lola::vx::ColumnCorrelationRow)
CEREAL_REGISTER_TYPE(lola::vx::ColumnCurrentRow)
CEREAL_REGISTER_TYPE(lola::vx::SynapseRow)
CEREAL_REGISTER_TYPE(lola::vx::SynapseWeightRow)
CEREAL_REGISTER_TYPE(lola::vx::SynapseLabelRow)
CEREAL_REGISTER_TYPE(lola::vx::SynapseCorrelationCalibRow)
CEREAL_REGISTER_TYPE(lola::vx::SynapseWeightMatrix)
CEREAL_REGISTER_TYPE(lola::vx::SynapseLabelMatrix)
CEREAL_REGISTER_TYPE(lola::vx::SynapseCorrelationCalibMatrix)
CEREAL_REGISTER_TYPE(lola::vx::SynapseMatrix)
CEREAL_REGISTER_TYPE(lola::vx::CorrelationResetRow)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::ColumnCorrelationRow)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::ColumnCurrentRow)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::SynapseRow)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::SynapseWeightRow)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::SynapseLabelRow)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::SynapseCorrelationCalibRow)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::SynapseWeightMatrix)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::SynapseLabelMatrix)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::SynapseCorrelationCalibMatrix)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::SynapseMatrix)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::CorrelationResetRow)
CEREAL_REGISTER_DYNAMIC_INIT(lola_vx_synapse)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::ColumnCorrelationRow)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::ColumnCurrentRow)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::SynapseRow)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::SynapseWeightRow)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::SynapseLabelRow)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::SynapseCorrelationCalibRow)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::SynapseWeightMatrix)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::SynapseLabelMatrix)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::SynapseCorrelationCalibMatrix)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::SynapseMatrix)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::CorrelationResetRow)
