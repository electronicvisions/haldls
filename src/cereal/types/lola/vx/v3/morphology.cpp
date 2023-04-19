#include "cereal/types/lola/vx/v3/morphology.h"

#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/lola/detail/hana.tcc"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/common/cerealization_typed_heap_array.h"
#include "lola/vx/v3/morphology.h"
#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/variant.hpp>
#include <cereal/types/vector.hpp>


namespace lola::vx::v3 {

template <typename Archive>
void MCSafeAtomicNeuron::Multicompartment::serialize(Archive& ar, std::uint32_t const /* version */)
{
	ar(CEREAL_NVP(connect_soma));
	ar(CEREAL_NVP(connect_soma_right));
	ar(CEREAL_NVP(connect_right));
	ar(CEREAL_NVP(connect_vertical));
	ar(CEREAL_NVP(enable_conductance));
	ar(CEREAL_NVP(enable_conductance_division));
	ar(CEREAL_NVP(enable_conductance_multiplication));
	ar(CEREAL_NVP(i_bias_nmda));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::Morphology)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(lola::vx::v3::MCSafeAtomicNeuron::Multicompartment)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::MCSafeAtomicNeuron)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::LogicalNeuron)
CEREAL_REGISTER_TYPE(lola::vx::v3::LogicalNeuron)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::v3::LogicalNeuron)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::v3::LogicalNeuron)
CEREAL_REGISTER_DYNAMIC_INIT(lola_vx_v3_morphology)
