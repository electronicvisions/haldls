#include "cereal/types/haldls/vx/madc.h"

#include "cereal/types/haldls/cereal.tcc"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/vx/madc.h"

namespace cereal {

template <class Archive>
void serialize(Archive& ar, haldls::vx::MADCControl& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable_iconv_amplifier));
	ar(CEREAL_NVP(value.m_enable_pre_amplifier));
	ar(CEREAL_NVP(value.m_enable_continuous_sampling));
	ar(CEREAL_NVP(value.m_enable_power_down_after_sampling));
	ar(CEREAL_NVP(value.m_stop_recording));
	ar(CEREAL_NVP(value.m_start_recording));
	ar(CEREAL_NVP(value.m_wake_up));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::MADCConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_active_mux_initially_selected_input));
	ar(CEREAL_NVP(value.m_active_mux_input_select_length));
	ar(CEREAL_NVP(value.m_sample_duration_adjust));
	ar(CEREAL_NVP(value.m_enable_sar_reset_on_fall));
	ar(CEREAL_NVP(value.m_sar_reset_wait));
	ar(CEREAL_NVP(value.m_sar_reset_length));
	ar(CEREAL_NVP(value.m_powerup_wait_value));
	ar(CEREAL_NVP(value.m_conversion_cycles_offset));
	ar(CEREAL_NVP(value.m_calibration_wait_value));
	ar(CEREAL_NVP(value.m_enable_calibration));
	ar(CEREAL_NVP(value.m_number_of_samples));
	ar(CEREAL_NVP(value.m_preamp_sampling_window_start));
	ar(CEREAL_NVP(value.m_preamp_sampling_window_end));
	ar(CEREAL_NVP(value.m_iconv_sampling_window_start));
	ar(CEREAL_NVP(value.m_iconv_sampling_window_end));
	ar(CEREAL_NVP(value.m_sample_on_positive_edge));
	ar(CEREAL_NVP(value.m_enable_dummy_data));
	ar(CEREAL_NVP(value.m_connect_preamp_to_madc));
	ar(CEREAL_NVP(value.m_connect_pads_to_madc));
	ar(CEREAL_NVP(value.m_connect_preamp_to_pads));
	ar(CEREAL_NVP(value.m_preamp_gain_capacitor_size));
	ar(CEREAL_NVP(value.m_enable_madc_clock_scaling));
	ar(CEREAL_NVP(value.m_madc_clock_scale_value));
	ar(CEREAL_NVP(value.m_enable_active_mux_amplifiers));
	ar(CEREAL_NVP(value.m_enable_pseudo_differential_reference));
	ar(CEREAL_NVP(value.m_signal_selection_connect_current_meter));
	ar(CEREAL_NVP(value.m_signal_selection_connect_active_mux));
	ar(CEREAL_NVP(value.m_signal_selection_connect_debug));
	ar(CEREAL_NVP(value.m_signal_selection_connect_preamp));
	ar(CEREAL_NVP(value.m_enable_iconv_amplifier));
	ar(CEREAL_NVP(value.m_connect_iconv_neuron));
	ar(CEREAL_NVP(value.m_connect_iconv_synapse));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::MADCControl)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::MADCConfig)
CEREAL_REGISTER_TYPE(haldls::vx::MADCControl)
CEREAL_REGISTER_TYPE(haldls::vx::MADCConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::MADCControl)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::MADCConfig)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_madc)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::MADCControl)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::MADCConfig)
