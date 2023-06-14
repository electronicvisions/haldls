#pragma once
#include "cereal/types/haldls/vx/neuron.h"

#include "cereal/types/halco/common/geometry.h"

namespace cereal {

template <typename Archive, typename Coordinates>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::NeuronBackendConfig<Coordinates>& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_address_out));
	ar(CEREAL_NVP(value.m_reset_holdoff));
	ar(CEREAL_NVP(value.m_refractory_time));
	ar(CEREAL_NVP(value.m_post_overwrite));
	ar(CEREAL_NVP(value.m_select_input_clock));
	ar(CEREAL_NVP(value.m_en_adapt_pulse));
	ar(CEREAL_NVP(value.m_en_bayesian_extension));
	ar(CEREAL_NVP(value.m_en_neuron_slave));
	ar(CEREAL_NVP(value.m_connect_fire_bottom));
	ar(CEREAL_NVP(value.m_connect_fire_from_right));
	ar(CEREAL_NVP(value.m_connect_fire_to_right));
	ar(CEREAL_NVP(value.m_en_spike_out));
	ar(CEREAL_NVP(value.m_en_neuron_master));
	ar(CEREAL_NVP(value.m_en_0_bayesian));
	ar(CEREAL_NVP(value.m_en_1_bayesian));
}

} // namespace cereal
