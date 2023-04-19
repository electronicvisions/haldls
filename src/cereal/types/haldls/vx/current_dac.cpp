#include "cereal/types/haldls/vx/current_dac.h"

#include "cereal/types/haldls/cereal.tcc"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/vx/current_dac.h"

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(Archive& ar, haldls::vx::CurrentDAC& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_current));
	ar(CEREAL_NVP(value.m_enable_current));
	ar(CEREAL_NVP(value.m_sign));
	ar(CEREAL_NVP(value.m_connect_neuron));
	ar(CEREAL_NVP(value.m_connect_synapse));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CurrentDAC);
CEREAL_REGISTER_TYPE(haldls::vx::CurrentDAC)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::CurrentDAC)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_current_dac)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::CurrentDAC)
