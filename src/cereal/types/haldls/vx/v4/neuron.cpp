#include "cereal/types/haldls/vx/v4/neuron.h"

#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/haldls/vx/neuron.tcc"
#include "halco/hicann-dls/vx/v4/coordinates.h"
#include "haldls/vx/v4/neuron.h"

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::v4::NeuronConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_en_comp_cond_div));
	ar(CEREAL_NVP(value.m_en_comp_cond_mul));
	ar(CEREAL_NVP(value.m_connect_soma));
	ar(CEREAL_NVP(value.m_connect_membrane_right));
	ar(CEREAL_NVP(value.m_en_comp_cond));
	ar(CEREAL_NVP(value.m_connect_bottom));
	ar(CEREAL_NVP(value.m_connect_somata));
	ar(CEREAL_NVP(value.m_en_fire));
	ar(CEREAL_NVP(value.m_en_strong_fire));
	ar(CEREAL_NVP(value.m_en_thresh_comp));
	ar(CEREAL_NVP(value.m_en_synin_inh));
	ar(CEREAL_NVP(value.m_en_synin_exc));
	ar(CEREAL_NVP(value.m_en_synin_inh_coba));
	ar(CEREAL_NVP(value.m_en_synin_exc_coba));
	ar(CEREAL_NVP(value.m_en_synin_inh_small_cap));
	ar(CEREAL_NVP(value.m_en_synin_exc_small_cap));
	ar(CEREAL_NVP(value.m_en_synin_inh_high_res));
	ar(CEREAL_NVP(value.m_en_synin_exc_high_res));
	ar(CEREAL_NVP(value.m_en_byp_inh));
	ar(CEREAL_NVP(value.m_en_byp_exc));
	ar(CEREAL_NVP(value.m_en_mem_off));
	ar(CEREAL_NVP(value.m_invert_current));
	ar(CEREAL_NVP(value.m_mem_cap_size));
	ar(CEREAL_NVP(value.m_invert_adapt_a));
	ar(CEREAL_NVP(value.m_invert_adapt_b));
	ar(CEREAL_NVP(value.m_en_adapt));
	ar(CEREAL_NVP(value.m_en_exp));
	ar(CEREAL_NVP(value.m_en_unbuf_access));
	ar(CEREAL_NVP(value.m_en_readout_amp));
	ar(CEREAL_NVP(value.m_readout_select));
	ar(CEREAL_NVP(value.m_en_readout));
	ar(CEREAL_NVP(value.m_en_reset_deg));
	ar(CEREAL_NVP(value.m_en_reset_div));
	ar(CEREAL_NVP(value.m_en_reset_mul));
	ar(CEREAL_NVP(value.m_en_leak_deg));
	ar(CEREAL_NVP(value.m_en_leak_div));
	ar(CEREAL_NVP(value.m_en_leak_mul));
	ar(CEREAL_NVP(value.m_en_pause));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(Archive&, haldls::vx::v4::NeuronResetQuad&, std::uint32_t const)
{}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v4::NeuronConfig);
CEREAL_REGISTER_TYPE(haldls::vx::v4::NeuronConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::v4::NeuronConfig)

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v4::NeuronResetQuad);
CEREAL_REGISTER_TYPE(haldls::vx::v4::NeuronResetQuad)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::v4::NeuronResetQuad)

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v4::NeuronBackendConfig);
CEREAL_REGISTER_TYPE(haldls::vx::v4::NeuronBackendConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::v4::NeuronBackendConfig)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_v4_neuron)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::v4::NeuronConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::v4::NeuronResetQuad)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::v4::NeuronBackendConfig)
