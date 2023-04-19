#include "cereal/types/haldls/vx/synapse.h"

#include "cereal/types/haldls/cereal.tcc"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/vx/synapse.h"

namespace haldls::vx {

template <class Archive>
void ColumnCorrelationQuad::ColumnCorrelationSwitch::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_enable_internal_causal));
	ar(CEREAL_NVP(m_enable_internal_acausal));
	ar(CEREAL_NVP(m_enable_debug_causal));
	ar(CEREAL_NVP(m_enable_debug_acausal));
	ar(CEREAL_NVP(m_enable_cadc_neuron_readout_causal));
	ar(CEREAL_NVP(m_enable_cadc_neuron_readout_acausal));
}

template <class Archive>
void ColumnCurrentQuad::ColumnCurrentSwitch::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_enable_synaptic_current_excitatory));
	ar(CEREAL_NVP(m_enable_synaptic_current_inhibitory));
	ar(CEREAL_NVP(m_enable_debug_excitatory));
	ar(CEREAL_NVP(m_enable_debug_inhibitory));
}

} // namespace haldls::vx

namespace cereal {

template <class Archive>
void serialize(Archive& ar, haldls::vx::CommonSynramConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_pc_conf_west));
	ar(CEREAL_NVP(value.m_pc_conf_east));
	ar(CEREAL_NVP(value.m_w_conf_west));
	ar(CEREAL_NVP(value.m_w_conf_east));
	ar(CEREAL_NVP(value.m_wait_ctr_clear));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::SynapseBiasSelection& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_int_dac_bias));
	ar(CEREAL_NVP(value.m_int_ramp_bias));
	ar(CEREAL_NVP(value.m_int_store_bias));
	ar(CEREAL_NVP(value.m_int_output_bias));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::SynapseWeightQuad& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_values));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::SynapseLabelQuad& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_values));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::SynapseCorrelationCalibQuad& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_time_calibs));
	ar(CEREAL_NVP(value.m_amp_calibs));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::SynapseQuad& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_weights));
	ar(CEREAL_NVP(value.m_labels));
	ar(CEREAL_NVP(value.m_time_calibs));
	ar(CEREAL_NVP(value.m_amp_calibs));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::ColumnCorrelationQuad& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_switches));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::ColumnCurrentQuad& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_switches));
}

template <class Archive>
void serialize(Archive&, haldls::vx::CorrelationReset&, std::uint32_t const)
{}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CommonSynramConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SynapseBiasSelection)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SynapseWeightQuad)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SynapseLabelQuad)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SynapseCorrelationCalibQuad)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SynapseQuad)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ColumnCorrelationQuad)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::ColumnCorrelationQuad::ColumnCorrelationSwitch)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::ColumnCurrentQuad::ColumnCurrentSwitch)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ColumnCurrentQuad)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CorrelationReset)
CEREAL_REGISTER_TYPE(haldls::vx::CommonSynramConfig)
CEREAL_REGISTER_TYPE(haldls::vx::SynapseWeightQuad)
CEREAL_REGISTER_TYPE(haldls::vx::SynapseLabelQuad)
CEREAL_REGISTER_TYPE(haldls::vx::SynapseCorrelationCalibQuad)
CEREAL_REGISTER_TYPE(haldls::vx::SynapseQuad)
CEREAL_REGISTER_TYPE(haldls::vx::ColumnCorrelationQuad)
CEREAL_REGISTER_TYPE(haldls::vx::ColumnCurrentQuad)
CEREAL_REGISTER_TYPE(haldls::vx::CorrelationReset)
CEREAL_REGISTER_TYPE(haldls::vx::SynapseBiasSelection)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::CommonSynramConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::SynapseWeightQuad)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::SynapseLabelQuad)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::SynapseCorrelationCalibQuad)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::SynapseQuad)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::ColumnCorrelationQuad)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::ColumnCurrentQuad)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::CorrelationReset)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::SynapseBiasSelection)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_synapse)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::CommonSynramConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::SynapseWeightQuad)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::SynapseLabelQuad)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::SynapseCorrelationCalibQuad)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::SynapseQuad)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::ColumnCorrelationQuad)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::ColumnCurrentQuad)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::CorrelationReset)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::SynapseBiasSelection)
