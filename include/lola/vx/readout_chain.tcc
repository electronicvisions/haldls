#include "halco/common/iter_all.h"
#include <boost/hana/adapt_struct.hpp>

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::CHIP_REVISION_STR::ReadoutChain>
    : public BackendContainerBase<
          lola::vx::CHIP_REVISION_STR::ReadoutChain,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::CHIP_REVISION_STR::ReadoutChain>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    lola::vx::CHIP_REVISION_STR::ReadoutChain::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using namespace halco::hicann_dls::vx::CHIP_REVISION_STR;
		using namespace haldls::vx::CHIP_REVISION_STR;

		visitor(coord, config);

		// input_mux + buffer_to_pad
		ReadoutSourceSelection dynamic_mux_config;
		ReadoutSourceSelection::source_multiplexer_type enable2pad;
		for (auto mux : halco::common::iter_all<SourceMultiplexerOnReadoutSourceSelection>()) {
			dynamic_mux_config.set_buffer(mux, config.input_mux[mux]);
			enable2pad[mux] = config.buffer_to_pad[mux].enable;
		}
		dynamic_mux_config.set_enable_buffer_to_pad(enable2pad);
		visit_preorder(dynamic_mux_config, ReadoutSourceSelectionOnDLS(), visitor);
		haldls::vx::CHIP_REVISION_STR::CapMemCell cell_out_amp_i_bias_0(
		    config.buffer_to_pad[SourceMultiplexerOnReadoutSourceSelection(0)].amp_i_bias);
		haldls::vx::CHIP_REVISION_STR::CapMemCell cell_out_amp_i_bias_1(
		    config.buffer_to_pad[SourceMultiplexerOnReadoutSourceSelection(1)].amp_i_bias);
		visit_preorder(cell_out_amp_i_bias_0, CapMemCellOnDLS::readout_out_amp_i_bias_0, visitor);
		visit_preorder(cell_out_amp_i_bias_1, CapMemCellOnDLS::readout_out_amp_i_bias_1, visitor);

		MADCConfig madc_config;

		// dynamic_mux
		madc_config.set_enable_active_mux_amplifiers(config.dynamic_mux.enable_amplifiers);
		madc_config.set_active_mux_initially_selected_input(
		    config.dynamic_mux.initially_selected_input);
		madc_config.set_active_mux_input_select_length(config.dynamic_mux.input_select_length);
		haldls::vx::CHIP_REVISION_STR::CapMemCell cell_ac_mux_i_bias(config.dynamic_mux.i_bias);
		visit_preorder(cell_ac_mux_i_bias, CapMemCellOnDLS::readout_ac_mux_i_bias, visitor);

		// pseudo_diff_converter
		madc_config.set_enable_pseudo_differential_reference(
		    config.pseudo_diff_converter.enable_reference);
		haldls::vx::CHIP_REVISION_STR::CapMemCell cell_pseudo_diff_v_ref(
		    config.pseudo_diff_converter.v_ref);
		haldls::vx::CHIP_REVISION_STR::CapMemCell cell_pseudo_diff_buffer_bias(
		    config.pseudo_diff_converter.buffer_bias);
		visit_preorder(cell_pseudo_diff_v_ref, CapMemCellOnDLS::readout_pseudo_diff_v_ref, visitor);
		visit_preorder(
		    cell_pseudo_diff_buffer_bias, CapMemCellOnDLS::readout_pseudo_diff_buffer_bias,
		    visitor);

		// madc_input_crossbar
		madc_config.set_signal_selection_connect_current_meter(
		    config.madc_input_crossbar.connect_source_measure_unit);
		madc_config.set_signal_selection_connect_active_mux(
		    config.madc_input_crossbar.connect_dynamic_mux);
		madc_config.set_signal_selection_connect_debug(config.madc_input_crossbar.connect_pad_mux);
		madc_config.set_signal_selection_connect_preamp(
		    config.madc_input_crossbar.connect_madc_preamp);

		// madc_preamp
		madc_config.set_preamp_gain_capacitor_size(config.madc_preamp.gain_cap_size);
		madc_config.set_preamp_sampling_window_start(config.madc_preamp.sampling_window_start);
		madc_config.set_preamp_sampling_window_end(config.madc_preamp.sampling_window_end);
		haldls::vx::CHIP_REVISION_STR::CapMemCell cell_sc_amp_v_ref(config.madc_preamp.v_ref);
		haldls::vx::CHIP_REVISION_STR::CapMemCell cell_sc_amp_i_bias(config.madc_preamp.i_bias);
		visit_preorder(cell_sc_amp_v_ref, CapMemCellOnDLS::readout_sc_amp_v_ref, visitor);
		visit_preorder(cell_sc_amp_i_bias, CapMemCellOnDLS::readout_sc_amp_i_bias, visitor);

		// madc_debug_crossbar
		madc_config.set_connect_preamp_to_madc(config.madc_debug_crossbar.connect_preamp_to_madc);
		madc_config.set_connect_pads_to_madc(config.madc_debug_crossbar.connect_pads_to_madc);
		madc_config.set_connect_preamp_to_pads(config.madc_debug_crossbar.connect_preamp_to_pads);

		// madc
		madc_config.set_sample_duration_adjust(config.madc.sample_duration_adjust);
		madc_config.set_enable_sar_reset_on_fall(config.madc.enable_sar_reset_on_fall);
		madc_config.set_sar_reset_wait(config.madc.sar_reset_wait);
		madc_config.set_sar_reset_length(config.madc.sar_reset_length);
		madc_config.set_powerup_wait_value(config.madc.powerup_wait_value);
		madc_config.set_conversion_cycles_offset(config.madc.conversion_cycles_offset);
		madc_config.set_enable_calibration(config.madc.enable_calibration);
		madc_config.set_calibration_wait_value(config.madc.calibration_wait_value);
		madc_config.set_number_of_samples(config.madc.number_of_samples);
		madc_config.set_sample_on_positive_edge(config.madc.sample_on_positive_edge);
		madc_config.set_enable_dummy_data(config.madc.enable_dummy_data);
		madc_config.set_enable_madc_clock_scaling(config.madc.enable_madc_clock_scaling);
		madc_config.set_madc_clock_scale_value(config.madc.clock_scale_value);
		haldls::vx::CHIP_REVISION_STR::CapMemCell cell_madc_in_500na(config.madc.in_500na);
		visit_preorder(cell_madc_in_500na, CapMemCellOnDLS::readout_madc_in_500na, visitor);

		// pad_mux
		for (auto mux : halco::common::iter_all<PadMultiplexerConfigOnDLS>()) {
			visit_preorder(config.pad_mux[mux], mux, visitor);
		}

		// source measure unit
		madc_config.set_iconv_sampling_window_start(
		    config.source_measure_unit.sampling_window_start);
		madc_config.set_iconv_sampling_window_end(config.source_measure_unit.sampling_window_end);
		madc_config.set_connect_iconv_neuron(config.source_measure_unit.connect_neuron_stimulus);
		madc_config.set_connect_iconv_synapse(config.source_measure_unit.connect_synapse_debug);
		haldls::vx::CHIP_REVISION_STR::CapMemCell cell_iconv_sc_amp_v_ref(
		    config.source_measure_unit.amp_v_ref);
		haldls::vx::CHIP_REVISION_STR::CapMemCell cell_iconv_sc_amp_i_bias(
		    config.source_measure_unit.amp_i_bias);
		haldls::vx::CHIP_REVISION_STR::CapMemCell cell_iconv_test_voltage(
		    config.source_measure_unit.test_voltage);
		haldls::vx::CHIP_REVISION_STR::CapMemCell cell_iconv_i_bias_buffer(
		    config.source_measure_unit.buffer_i_bias);
		visit_preorder(
		    cell_iconv_sc_amp_v_ref, CapMemCellOnDLS::readout_iconv_sc_amp_v_ref, visitor);
		visit_preorder(
		    cell_iconv_sc_amp_i_bias, CapMemCellOnDLS::readout_iconv_sc_amp_i_bias, visitor);
		visit_preorder(
		    cell_iconv_test_voltage, CapMemCellOnDLS::readout_iconv_test_voltage, visitor);
		visit_preorder(
		    cell_iconv_i_bias_buffer, CapMemCellOnDLS::readout_iconv_i_bias_buffer, visitor);


		CurrentDAC dac_config;

		// current_dac
		dac_config.set_enable_current(config.current_dac.enable_current);
		dac_config.set_current(config.current_dac.current);
		dac_config.set_sign(config.current_dac.sign);
		dac_config.set_connect_neuron(config.current_dac.connect_neuron_stimulus);
		dac_config.set_connect_synapse(config.current_dac.connect_synapse_debug);
		haldls::vx::CHIP_REVISION_STR::CapMemCell cell_idac_i_bias(config.current_dac.i_bias);
		haldls::vx::CHIP_REVISION_STR::CapMemCell cell_idac_i_bias_casc(
		    config.current_dac.i_bias_casc);
		visit_preorder(cell_idac_i_bias, CapMemCellOnDLS::readout_idac_i_bias, visitor);
		visit_preorder(cell_idac_i_bias_casc, CapMemCellOnDLS::readout_idac_i_bias_casc, visitor);
		visit_preorder(dac_config, CurrentDACOnDLS(), visitor);

		visit_preorder(madc_config, MADCConfigOnDLS(), visitor);

		if constexpr (!std::is_same<
		                  ContainerT, lola::vx::CHIP_REVISION_STR::ReadoutChain const>::value) {
			// input_mux + buffer_to_pad
			enable2pad = dynamic_mux_config.get_enable_buffer_to_pad();
			for (auto mux : halco::common::iter_all<SourceMultiplexerOnReadoutSourceSelection>()) {
				config.input_mux[mux] = dynamic_mux_config.get_buffer(mux);
				config.buffer_to_pad[mux].enable = enable2pad[mux];
			}
			config.buffer_to_pad[SourceMultiplexerOnReadoutSourceSelection(0)].amp_i_bias =
			    cell_out_amp_i_bias_0.get_value();
			config.buffer_to_pad[SourceMultiplexerOnReadoutSourceSelection(1)].amp_i_bias =
			    cell_out_amp_i_bias_1.get_value();

			config.dynamic_mux.enable_amplifiers = madc_config.get_enable_active_mux_amplifiers();
			config.dynamic_mux.initially_selected_input =
			    madc_config.get_active_mux_initially_selected_input();
			config.dynamic_mux.input_select_length =
			    madc_config.get_active_mux_input_select_length();
			config.pseudo_diff_converter.enable_reference =
			    madc_config.get_enable_pseudo_differential_reference();
			config.madc_input_crossbar.connect_source_measure_unit =
			    madc_config.get_signal_selection_connect_current_meter();
			config.madc_input_crossbar.connect_dynamic_mux =
			    madc_config.get_signal_selection_connect_active_mux();
			config.madc_input_crossbar.connect_pad_mux =
			    madc_config.get_signal_selection_connect_debug();
			config.madc_input_crossbar.connect_madc_preamp =
			    madc_config.get_signal_selection_connect_preamp();
			config.madc_preamp.gain_cap_size = madc_config.get_preamp_gain_capacitor_size();
			config.madc_preamp.sampling_window_start =
			    madc_config.get_preamp_sampling_window_start();
			config.madc_preamp.sampling_window_end = madc_config.get_preamp_sampling_window_end();
			config.madc_debug_crossbar.connect_preamp_to_madc =
			    madc_config.get_connect_preamp_to_madc();
			config.madc_debug_crossbar.connect_pads_to_madc =
			    madc_config.get_connect_pads_to_madc();
			config.madc_debug_crossbar.connect_preamp_to_pads =
			    madc_config.get_connect_preamp_to_pads();
			config.madc.sample_duration_adjust = madc_config.get_sample_duration_adjust();
			config.madc.enable_sar_reset_on_fall = madc_config.get_enable_sar_reset_on_fall();
			config.madc.sar_reset_wait = madc_config.get_sar_reset_wait();
			config.madc.sar_reset_length = madc_config.get_sar_reset_length();
			config.madc.powerup_wait_value = madc_config.get_powerup_wait_value();
			config.madc.conversion_cycles_offset = madc_config.get_conversion_cycles_offset();
			config.madc.enable_calibration = madc_config.get_enable_calibration();
			config.madc.calibration_wait_value = madc_config.get_calibration_wait_value();
			config.madc.number_of_samples = madc_config.get_number_of_samples();
			config.madc.sample_on_positive_edge = madc_config.get_sample_on_positive_edge();
			config.madc.enable_dummy_data = madc_config.get_enable_dummy_data();
			config.madc.enable_madc_clock_scaling = madc_config.get_enable_madc_clock_scaling();
			config.madc.clock_scale_value = madc_config.get_madc_clock_scale_value();
			config.source_measure_unit.sampling_window_start =
			    madc_config.get_iconv_sampling_window_start();
			config.source_measure_unit.sampling_window_end =
			    madc_config.get_iconv_sampling_window_end();
			config.source_measure_unit.connect_neuron_stimulus =
			    madc_config.get_connect_iconv_neuron();
			config.source_measure_unit.connect_synapse_debug =
			    madc_config.get_connect_iconv_synapse();

			config.current_dac.enable_current = dac_config.get_enable_current();
			config.current_dac.current = dac_config.get_current();
			config.current_dac.sign = dac_config.get_sign();
			config.current_dac.connect_neuron_stimulus = dac_config.get_connect_neuron();
			config.current_dac.connect_synapse_debug = dac_config.get_connect_synapse();

			config.dynamic_mux.i_bias = cell_ac_mux_i_bias.get_value();
			config.pseudo_diff_converter.v_ref = cell_pseudo_diff_v_ref.get_value();
			config.pseudo_diff_converter.buffer_bias = cell_pseudo_diff_buffer_bias.get_value();
			config.madc_preamp.v_ref = cell_sc_amp_v_ref.get_value();
			config.madc_preamp.i_bias = cell_sc_amp_i_bias.get_value();
			config.madc.in_500na = cell_madc_in_500na.get_value();
			config.source_measure_unit.amp_v_ref = cell_iconv_sc_amp_v_ref.get_value();
			config.source_measure_unit.amp_i_bias = cell_iconv_sc_amp_i_bias.get_value();
			config.source_measure_unit.test_voltage = cell_iconv_test_voltage.get_value();
			config.source_measure_unit.buffer_i_bias = cell_iconv_i_bias_buffer.get_value();
			config.current_dac.i_bias = cell_idac_i_bias.get_value();
			config.current_dac.i_bias_casc = cell_idac_i_bias_casc.get_value();
		}
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::CHIP_REVISION_STR::ReadoutChain::BufferToPadT, enable, amp_i_bias);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::CHIP_REVISION_STR::ReadoutChain::DynamicMux,
    enable_amplifiers,
    initially_selected_input,
    input_select_length,
    i_bias);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::CHIP_REVISION_STR::ReadoutChain::PseudoDifferentialConverter,
    enable_reference,
    v_ref,
    buffer_bias);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::CHIP_REVISION_STR::ReadoutChain::MADCInputCrossbar,
    connect_source_measure_unit,
    connect_dynamic_mux,
    connect_pad_mux,
    connect_madc_preamp);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::CHIP_REVISION_STR::ReadoutChain::MADCPreamp,
    gain_cap_size,
    sampling_window_start,
    sampling_window_end,
    v_ref,
    i_bias);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::CHIP_REVISION_STR::ReadoutChain::MADCDebugCrossbar,
    connect_preamp_to_madc,
    connect_pads_to_madc,
    connect_preamp_to_pads);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::CHIP_REVISION_STR::ReadoutChain::MADC,
    sample_duration_adjust,
    enable_sar_reset_on_fall,
    sar_reset_wait,
    sar_reset_length,
    powerup_wait_value,
    conversion_cycles_offset,
    enable_calibration,
    calibration_wait_value,
    number_of_samples,
    sample_on_positive_edge,
    enable_dummy_data,
    enable_madc_clock_scaling,
    clock_scale_value,
    in_500na);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::CHIP_REVISION_STR::ReadoutChain::SourceMeasureUnit,
    sampling_window_start,
    sampling_window_end,
    connect_neuron_stimulus,
    connect_synapse_debug,
    test_voltage,
    buffer_i_bias,
    amp_v_ref,
    amp_i_bias);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::CHIP_REVISION_STR::ReadoutChain::CurrentDAC,
    enable_current,
    current,
    sign,
    connect_neuron_stimulus,
    connect_synapse_debug,
    i_bias,
    i_bias_casc);


BOOST_HANA_ADAPT_STRUCT(
    lola::vx::CHIP_REVISION_STR::ReadoutChain,
    input_mux,
    buffer_to_pad,
    dynamic_mux,
    pseudo_diff_converter,
    madc_input_crossbar,
    madc_preamp,
    madc_debug_crossbar,
    madc,
    pad_mux,
    source_measure_unit,
    current_dac);

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::CHIP_REVISION_STR::ReadoutChain::BufferToPadT)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::CHIP_REVISION_STR::ReadoutChain::DynamicMux)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(
    lola::vx::CHIP_REVISION_STR::ReadoutChain::PseudoDifferentialConverter)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(
    lola::vx::CHIP_REVISION_STR::ReadoutChain::MADCInputCrossbar)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::CHIP_REVISION_STR::ReadoutChain::MADCPreamp)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(
    lola::vx::CHIP_REVISION_STR::ReadoutChain::MADCDebugCrossbar)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::CHIP_REVISION_STR::ReadoutChain::MADC)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(
    lola::vx::CHIP_REVISION_STR::ReadoutChain::SourceMeasureUnit)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::CHIP_REVISION_STR::ReadoutChain::CurrentDAC)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::CHIP_REVISION_STR::ReadoutChain)
