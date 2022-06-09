#ifndef CHIP_REVISION
#error "Requires CHIP_REVISION"
#endif

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/common.h"
#include "lola/vx/readout_chain.h"
#include "stadls/visitors.h"
#include "test-helper.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#if CHIP_REVISION == 2
#define CHIP_REVISION_STR v2
#elif CHIP_REVISION == 3
#define CHIP_REVISION_STR v3
#else
#error "Unknown CHIP_REVISION"
#endif

using namespace haldls::vx;
using namespace lola::vx::CHIP_REVISION_STR;
using namespace halco::hicann_dls::vx::CHIP_REVISION_STR;
using namespace halco::common;

TEST(ReadoutChain, General)
{
	{
		ReadoutChain config;
		ReadoutSourceSelection::SourceMultiplexer mux;
		mux.set_debug_plus(!mux.get_debug_plus());
		config.input_mux.at(SourceMultiplexerOnReadoutSourceSelection(1)) = mux;
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		ReadoutChain::BufferToPadT buffer;
		buffer.enable = !buffer.enable;
		config.buffer_to_pad.at(SourceMultiplexerOnReadoutSourceSelection(1)) = buffer;
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		ReadoutChain::BufferToPadT buffer;
		buffer.amp_i_bias = ReadoutChain::AnalogValueVariant(
		    draw_ranged_non_default_value<ReadoutChain::AnalogValue>());
		config.buffer_to_pad.at(SourceMultiplexerOnReadoutSourceSelection(1)) = buffer;
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.dynamic_mux.enable_amplifiers = !config.dynamic_mux.enable_amplifiers;
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.dynamic_mux.initially_selected_input =
		    draw_ranged_non_default_value<ReadoutChain::DynamicMux::InputSelect>(
		        config.dynamic_mux.initially_selected_input);
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.dynamic_mux.input_select_length =
		    draw_ranged_non_default_value<ReadoutChain::DynamicMux::InputSelectLength>(
		        config.dynamic_mux.input_select_length);
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.dynamic_mux.i_bias = draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.pseudo_diff_converter.enable_reference = ReadoutChain::HemisphereType{false, true};
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.pseudo_diff_converter.v_ref =
		    draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.pseudo_diff_converter.buffer_bias =
		    draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc_input_crossbar.connect_source_measure_unit =
		    !config.madc_input_crossbar.connect_source_measure_unit;
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc_input_crossbar.connect_dynamic_mux =
		    !config.madc_input_crossbar.connect_dynamic_mux;
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc_input_crossbar.connect_pad_mux = !config.madc_input_crossbar.connect_pad_mux;
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc_input_crossbar.connect_madc_preamp =
		    !config.madc_input_crossbar.connect_madc_preamp;
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc_preamp.gain_cap_size =
		    draw_ranged_non_default_value<ReadoutChain::MADCPreamp::GainCapacitorSize>(
		        config.madc_preamp.gain_cap_size);
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc_preamp.sampling_window_start =
		    draw_ranged_non_default_value<ReadoutChain::SamplingWindowTiming>(
		        config.madc_preamp.sampling_window_start);
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc_preamp.sampling_window_end =
		    draw_ranged_non_default_value<ReadoutChain::SamplingWindowTiming>(
		        config.madc_preamp.sampling_window_end);
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc_preamp.v_ref = draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc_preamp.i_bias = draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc_debug_crossbar.connect_preamp_to_madc =
		    !config.madc_debug_crossbar.connect_preamp_to_madc;
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc_debug_crossbar.connect_pads_to_madc =
		    !config.madc_debug_crossbar.connect_pads_to_madc;
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc_debug_crossbar.connect_preamp_to_pads =
		    !config.madc_debug_crossbar.connect_preamp_to_pads;
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc.sample_duration_adjust =
		    draw_ranged_non_default_value<ReadoutChain::MADC::SampleDurationAdjust>(
		        config.madc.sample_duration_adjust);
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc.enable_sar_reset_on_fall = !config.madc.enable_sar_reset_on_fall;
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc.sar_reset_wait =
		    draw_ranged_non_default_value<ReadoutChain::MADC::SARResetWait>(
		        config.madc.sar_reset_wait);
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc.sar_reset_length =
		    draw_ranged_non_default_value<ReadoutChain::MADC::SARResetLength>(
		        config.madc.sar_reset_length);
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc.powerup_wait_value =
		    draw_ranged_non_default_value<ReadoutChain::MADC::PowerupWaitValue>(
		        config.madc.powerup_wait_value);
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc.conversion_cycles_offset =
		    draw_ranged_non_default_value<ReadoutChain::MADC::ConversionCyclesOffset>(
		        config.madc.conversion_cycles_offset);
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc.enable_calibration = !config.madc.enable_calibration;
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc.calibration_wait_value =
		    draw_ranged_non_default_value<ReadoutChain::MADC::CalibrationWaitValue>(
		        config.madc.calibration_wait_value);
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc.number_of_samples =
		    draw_ranged_non_default_value<ReadoutChain::MADC::NumberOfSamples>(
		        config.madc.number_of_samples);
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc.sample_on_positive_edge = !config.madc.sample_on_positive_edge;
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc.enable_dummy_data = !config.madc.enable_dummy_data;
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc.enable_madc_clock_scaling = !config.madc.enable_madc_clock_scaling;
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc.clock_scale_value =
		    draw_ranged_non_default_value<ReadoutChain::MADC::ClockScaleValue>(
		        config.madc.clock_scale_value);
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.madc.in_500na = draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		PadMultiplexerConfig mux;
		mux.set_cadc_v_ramp_mux_to_pad(!mux.get_cadc_v_ramp_mux_to_pad());
		config.pad_mux.at(PadMultiplexerConfigOnDLS(1)) = mux;
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.source_measure_unit.sampling_window_start =
		    draw_ranged_non_default_value<ReadoutChain::SamplingWindowTiming>(
		        config.source_measure_unit.sampling_window_start);
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.source_measure_unit.sampling_window_end =
		    draw_ranged_non_default_value<ReadoutChain::SamplingWindowTiming>(
		        config.source_measure_unit.sampling_window_end);
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.source_measure_unit.connect_neuron_stimulus =
		    ReadoutChain::HemisphereType{false, true};
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.source_measure_unit.connect_synapse_debug =
		    ReadoutChain::SynapseTargetType{false, true};
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.source_measure_unit.test_voltage =
		    draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.source_measure_unit.buffer_i_bias =
		    draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.source_measure_unit.amp_v_ref =
		    draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.source_measure_unit.amp_i_bias =
		    draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.current_dac.enable_current = !config.current_dac.enable_current;
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.current_dac.current =
		    draw_non_default_value<ReadoutChain::CurrentDAC::Current>(config.current_dac.current);
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.current_dac.sign = ReadoutChain::CurrentDAC::Sign::sink;
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.current_dac.connect_neuron_stimulus = ReadoutChain::HemisphereType{false, true};
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.current_dac.connect_synapse_debug = ReadoutChain::SynapseTargetType{false, true};
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.current_dac.i_bias = draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
		EXPECT_NE(config, ReadoutChain());
	}
	{
		ReadoutChain config;
		config.current_dac.i_bias_casc = draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
		EXPECT_NE(config, ReadoutChain());
	}
}

ReadoutChain create_non_default_container()
{
	ReadoutChain config;
	ReadoutSourceSelection::SourceMultiplexer mux;
	mux.set_debug_plus(!mux.get_debug_plus());
	config.input_mux.at(SourceMultiplexerOnReadoutSourceSelection(1)) = mux;
	ReadoutChain::BufferToPadT buffer;
	buffer.enable = !buffer.enable;
	buffer.amp_i_bias = ReadoutChain::AnalogValueVariant(
	    draw_ranged_non_default_value<ReadoutChain::AnalogValue>());
	config.buffer_to_pad.at(SourceMultiplexerOnReadoutSourceSelection(1)) = buffer;
	config.dynamic_mux.enable_amplifiers = !config.dynamic_mux.enable_amplifiers;
	config.dynamic_mux.initially_selected_input =
	    draw_ranged_non_default_value<ReadoutChain::DynamicMux::InputSelect>(
	        config.dynamic_mux.initially_selected_input);
	config.dynamic_mux.input_select_length =
	    draw_ranged_non_default_value<ReadoutChain::DynamicMux::InputSelectLength>(
	        config.dynamic_mux.input_select_length);
	config.dynamic_mux.i_bias = draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
	config.pseudo_diff_converter.enable_reference = ReadoutChain::HemisphereType{false, true};
	config.pseudo_diff_converter.v_ref = draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
	config.pseudo_diff_converter.buffer_bias =
	    draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
	config.madc_input_crossbar.connect_source_measure_unit =
	    !config.madc_input_crossbar.connect_source_measure_unit;
	config.madc_input_crossbar.connect_dynamic_mux =
	    !config.madc_input_crossbar.connect_dynamic_mux;
	config.madc_input_crossbar.connect_pad_mux = !config.madc_input_crossbar.connect_pad_mux;
	config.madc_input_crossbar.connect_madc_preamp =
	    !config.madc_input_crossbar.connect_madc_preamp;
	config.madc_preamp.gain_cap_size =
	    draw_ranged_non_default_value<ReadoutChain::MADCPreamp::GainCapacitorSize>(
	        config.madc_preamp.gain_cap_size);
	config.madc_preamp.sampling_window_start =
	    draw_ranged_non_default_value<ReadoutChain::SamplingWindowTiming>(
	        config.madc_preamp.sampling_window_start);
	config.madc_preamp.sampling_window_end =
	    draw_ranged_non_default_value<ReadoutChain::SamplingWindowTiming>(
	        config.madc_preamp.sampling_window_end);
	config.madc_preamp.v_ref = draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
	config.madc_preamp.i_bias = draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
	config.madc_debug_crossbar.connect_preamp_to_madc =
	    !config.madc_debug_crossbar.connect_preamp_to_madc;
	config.madc_debug_crossbar.connect_pads_to_madc =
	    !config.madc_debug_crossbar.connect_pads_to_madc;
	config.madc_debug_crossbar.connect_preamp_to_pads =
	    !config.madc_debug_crossbar.connect_preamp_to_pads;
	config.madc.sample_duration_adjust =
	    draw_ranged_non_default_value<ReadoutChain::MADC::SampleDurationAdjust>(
	        config.madc.sample_duration_adjust);
	config.madc.enable_sar_reset_on_fall = !config.madc.enable_sar_reset_on_fall;
	config.madc.sar_reset_wait =
	    draw_ranged_non_default_value<ReadoutChain::MADC::SARResetWait>(config.madc.sar_reset_wait);
	config.madc.sar_reset_length =
	    draw_ranged_non_default_value<ReadoutChain::MADC::SARResetLength>(
	        config.madc.sar_reset_length);
	config.madc.powerup_wait_value =
	    draw_ranged_non_default_value<ReadoutChain::MADC::PowerupWaitValue>(
	        config.madc.powerup_wait_value);
	config.madc.conversion_cycles_offset =
	    draw_ranged_non_default_value<ReadoutChain::MADC::ConversionCyclesOffset>(
	        config.madc.conversion_cycles_offset);
	config.madc.enable_calibration = !config.madc.enable_calibration;
	config.madc.calibration_wait_value =
	    draw_ranged_non_default_value<ReadoutChain::MADC::CalibrationWaitValue>(
	        config.madc.calibration_wait_value);
	config.madc.number_of_samples =
	    draw_ranged_non_default_value<ReadoutChain::MADC::NumberOfSamples>(
	        config.madc.number_of_samples);
	config.madc.sample_on_positive_edge = !config.madc.sample_on_positive_edge;
	config.madc.enable_dummy_data = !config.madc.enable_dummy_data;
	config.madc.enable_madc_clock_scaling = !config.madc.enable_madc_clock_scaling;
	config.madc.clock_scale_value =
	    draw_ranged_non_default_value<ReadoutChain::MADC::ClockScaleValue>(
	        config.madc.clock_scale_value);
	config.madc.in_500na = draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
	PadMultiplexerConfig padmux;
	padmux.set_cadc_v_ramp_mux_to_pad(!padmux.get_cadc_v_ramp_mux_to_pad());
	config.pad_mux.at(PadMultiplexerConfigOnDLS(1)) = padmux;
	config.source_measure_unit.sampling_window_start =
	    draw_ranged_non_default_value<ReadoutChain::SamplingWindowTiming>(
	        config.source_measure_unit.sampling_window_start);
	config.source_measure_unit.sampling_window_end =
	    draw_ranged_non_default_value<ReadoutChain::SamplingWindowTiming>(
	        config.source_measure_unit.sampling_window_end);
	config.source_measure_unit.connect_neuron_stimulus = ReadoutChain::HemisphereType{false, true};
	config.source_measure_unit.connect_synapse_debug = ReadoutChain::SynapseTargetType{false, true};
	config.source_measure_unit.test_voltage =
	    draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
	config.source_measure_unit.buffer_i_bias =
	    draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
	config.source_measure_unit.amp_v_ref =
	    draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
	config.source_measure_unit.amp_i_bias =
	    draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
	config.current_dac.enable_current = !config.current_dac.enable_current;
	config.current_dac.current =
	    draw_non_default_value<ReadoutChain::CurrentDAC::Current>(config.current_dac.current);
	config.current_dac.sign = ReadoutChain::CurrentDAC::Sign::sink;
	config.current_dac.connect_neuron_stimulus = ReadoutChain::HemisphereType{false, true};
	config.current_dac.connect_synapse_debug = ReadoutChain::SynapseTargetType{false, true};
	config.current_dac.i_bias = draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
	config.current_dac.i_bias_casc = draw_ranged_non_default_value<ReadoutChain::AnalogValue>();
	return config;
}

TEST(ReadoutChain, CerealizeCoverage)
{
	ReadoutChain obj1, obj2;

	obj1 = create_non_default_container();

	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(obj1);
	}

	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(obj2);
	}
	ASSERT_EQ(obj1, obj2);
}

TEST(ReadoutChain, EncodeDecode)
{
	ReadoutChain config = create_non_default_container();

	typedef std::vector<OmnibusAddress> addresses_type;
	typedef std::vector<fisch::vx::word_access_type::Omnibus> words_type;

	ReadoutChainOnDLS coord;

	addresses_type read_addresses;
	addresses_type write_addresses;
	visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
	visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
	EXPECT_THAT(read_addresses, write_addresses);

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});

	ReadoutChain config_decoded;
	visit_preorder(config_decoded, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	EXPECT_EQ(config_decoded, config);
}
