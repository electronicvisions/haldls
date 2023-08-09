#include "cereal/types/lola/vx/v4/morphology.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_array.h"
#include "cereal/types/halco/common/typed_heap_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/lola/detail/hana.tcc"
#include "lola/vx/v4/morphology.h"
#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/variant.hpp>
#include <cereal/types/vector.hpp>


namespace lola::vx::v4 {

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

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v4::Morphology)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(lola::vx::v4::MCSafeAtomicNeuron::Multicompartment)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v4::MCSafeAtomicNeuron)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v4::LogicalNeuron)
CEREAL_REGISTER_TYPE(lola::vx::v4::LogicalNeuron)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::v4::LogicalNeuron)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::v4::LogicalNeuron)
CEREAL_REGISTER_DYNAMIC_INIT(lola_vx_v4_morphology)
