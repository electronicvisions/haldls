#include "cereal/types/haldls/vx/readout.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "haldls/vx/readout.h"

namespace haldls::vx {

template <class Archive>
void ReadoutSourceSelection::SourceMultiplexer::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_debug_plus));
	ar(CEREAL_NVP(m_debug_minus));
	ar(CEREAL_NVP(m_current_dac));
	ar(CEREAL_NVP(m_synin_debug_inhibitory));
	ar(CEREAL_NVP(m_synin_debug_excitatory));
	ar(CEREAL_NVP(m_cadc_debug_causal));
	ar(CEREAL_NVP(m_cadc_debug_acausal));
	ar(CEREAL_NVP(m_synapse_driver_debug));
	ar(CEREAL_NVP(m_neuron_odd));
	ar(CEREAL_NVP(m_neuron_even));
}

} // namespace haldls::vx

namespace cereal {

template <class Archive>
void serialize(Archive& ar, haldls::vx::PadMultiplexerConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_cadc_v_ramp_mux));
	ar(CEREAL_NVP(value.m_cadc_v_ramp_mux_to_pad));
	ar(CEREAL_NVP(value.m_capmem_i_out_mux));
	ar(CEREAL_NVP(value.m_capmem_i_out_mux_to_inter));
	ar(CEREAL_NVP(value.m_capmem_inter_mux_to_pad));
	ar(CEREAL_NVP(value.m_capmem_v_out_mux));
	ar(CEREAL_NVP(value.m_capmem_v_out_mux_to_inter));
	ar(CEREAL_NVP(value.m_capmem_v_ref_mux));
	ar(CEREAL_NVP(value.m_capmem_v_ref_mux_to_inter));
	ar(CEREAL_NVP(value.m_neuron_i_stim_mux));
	ar(CEREAL_NVP(value.m_neuron_i_stim_mux_to_pad));
	ar(CEREAL_NVP(value.m_cadc_debug_acausal_to_inter));
	ar(CEREAL_NVP(value.m_cadc_debug_causal_to_inter));
	ar(CEREAL_NVP(value.m_synin_debug_inhibitory_to_inter));
	ar(CEREAL_NVP(value.m_synin_debug_excitatory_to_inter));
	ar(CEREAL_NVP(value.m_synapse_inter_mux_to_pad));
	ar(CEREAL_NVP(value.m_buffer_to_pad));
	ar(CEREAL_NVP(value.m_debug_to_pad));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::ReadoutSourceSelection& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_buffers));
	ar(CEREAL_NVP(value.m_enable_buffer_to_pad));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PadMultiplexerConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::ReadoutSourceSelection::SourceMultiplexer)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ReadoutSourceSelection)
CEREAL_REGISTER_TYPE(haldls::vx::PadMultiplexerConfig)
CEREAL_REGISTER_TYPE(haldls::vx::ReadoutSourceSelection)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::PadMultiplexerConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::ReadoutSourceSelection)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_readout)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::PadMultiplexerConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::ReadoutSourceSelection)
