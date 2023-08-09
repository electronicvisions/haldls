#include "cereal/types/lola/vx/v4/neuron.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_array.h"
#include "cereal/types/halco/common/typed_heap_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/haldls/vx/synapse.h"
#include "cereal/types/lola/detail/hana.tcc"
#include "lola/vx/v4/neuron.h"
#include <cereal/types/variant.hpp>


EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v4::AtomicNeuron::SynapticInput)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v4::AtomicNeuron::Leak)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v4::AtomicNeuron::Reset)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v4::AtomicNeuron::Threshold)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v4::AtomicNeuron::Multicompartment)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v4::AtomicNeuron::ConstantCurrent)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v4::AtomicNeuron::MembraneCapacitance)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v4::AtomicNeuron::Adaptation)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v4::AtomicNeuron::Exponential)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v4::AtomicNeuron::Readout)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v4::AtomicNeuron::EventRouting)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v4::AtomicNeuron::RefractoryPeriod)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v4::AtomicNeuron::Bayesian)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v4::AtomicNeuron)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v4::NeuronBlock)
CEREAL_REGISTER_TYPE(lola::vx::v4::AtomicNeuron)
CEREAL_REGISTER_TYPE(lola::vx::v4::NeuronBlock)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::v4::AtomicNeuron)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::v4::NeuronBlock)
CEREAL_REGISTER_DYNAMIC_INIT(lola_vx_v4_neuron)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::v4::AtomicNeuron)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::v4::NeuronBlock)
