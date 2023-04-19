#include "cereal/types/haldls/vx/neuron.h"

#include "cereal/types/haldls/cereal.tcc"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/vx/neuron.h"

namespace cereal {


template <class Archive>
void serialize(Archive& ar, haldls::vx::CommonNeuronBackendConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_en_event_regs));
	ar(CEREAL_NVP(value.m_force_reset));
	ar(CEREAL_NVP(value.m_en_clocks));
	ar(CEREAL_NVP(value.m_clock_scale_slow));
	ar(CEREAL_NVP(value.m_clock_scale_fast));
	ar(CEREAL_NVP(value.m_sample_pos_edge));
	ar(CEREAL_NVP(value.m_clock_scale_adapt_pulse));
	ar(CEREAL_NVP(value.m_clock_scale_post_pulse));
	ar(CEREAL_NVP(value.m_wait_global_post_pulse));
	ar(CEREAL_NVP(value.m_wait_spike_counter_reset));
	ar(CEREAL_NVP(value.m_wait_spike_counter_read));
	ar(CEREAL_NVP(value.m_wait_fire_neuron));
}

template <class Archive>
void serialize(Archive&, haldls::vx::NeuronReset&, std::uint32_t const)
{}

template <class Archive>
void serialize(Archive&, haldls::vx::BlockPostPulse&, std::uint32_t const)
{}

template <class Archive>
void serialize(Archive& ar, haldls::vx::SpikeCounterRead& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_count));
	ar(CEREAL_NVP(value.m_overflow));
}

template <class Archive>
void serialize(Archive&, haldls::vx::SpikeCounterReset&, std::uint32_t const)
{}

template <class Archive>
void serialize(Archive& ar, haldls::vx::NeuronSRAMTimingConfig& value, std::uint32_t const)
{
	ar(cereal::base_class<haldls::vx::detail::SRAMTimingConfig>(&value));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::NeuronBackendSRAMTimingConfig& value, std::uint32_t const)
{
	ar(cereal::base_class<haldls::vx::detail::SRAMTimingConfig>(&value));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CommonNeuronBackendConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::NeuronReset)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::BlockPostPulse)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SpikeCounterRead)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SpikeCounterReset)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::NeuronSRAMTimingConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::NeuronBackendSRAMTimingConfig)
CEREAL_REGISTER_TYPE(haldls::vx::NeuronSRAMTimingConfig)
CEREAL_REGISTER_TYPE(haldls::vx::NeuronBackendSRAMTimingConfig)
CEREAL_REGISTER_TYPE(haldls::vx::CommonNeuronBackendConfig)
CEREAL_REGISTER_TYPE(haldls::vx::NeuronReset)
CEREAL_REGISTER_TYPE(haldls::vx::BlockPostPulse)
CEREAL_REGISTER_TYPE(haldls::vx::SpikeCounterRead)
CEREAL_REGISTER_TYPE(haldls::vx::SpikeCounterReset)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::NeuronSRAMTimingConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::NeuronBackendSRAMTimingConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::CommonNeuronBackendConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::NeuronReset)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::BlockPostPulse)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::SpikeCounterRead)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::SpikeCounterReset)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_neuron)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::NeuronSRAMTimingConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::NeuronBackendSRAMTimingConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::CommonNeuronBackendConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::NeuronReset)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::BlockPostPulse)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::SpikeCounterRead)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::SpikeCounterReset)
