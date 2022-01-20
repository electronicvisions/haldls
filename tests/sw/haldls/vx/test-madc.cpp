#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/vx/madc.h"

#include "fisch/vx/word_access/type/jtag.h"
#include "halco/hicann-dls/vx/madc.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/omnibus_constants.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> addresses_type;
typedef fisch::vx::word_access_type::OmnibusChipOverJTAG word_type;
typedef std::vector<word_type> words_type;

TEST(MADCControl, General)
{
	MADCControl control;

	// test getter/setter
	{
		auto value = !control.get_enable_iconv_amplifier();
		control.set_enable_iconv_amplifier(value);
		EXPECT_EQ(control.get_enable_iconv_amplifier(), value);
	}

	{
		auto value = !control.get_enable_pre_amplifier();
		control.set_enable_pre_amplifier(value);
		EXPECT_EQ(control.get_enable_pre_amplifier(), value);
	}

	{
		auto value = !control.get_enable_continuous_sampling();
		control.set_enable_continuous_sampling(value);
		EXPECT_EQ(control.get_enable_continuous_sampling(), value);
	}

	{
		auto value = !control.get_enable_power_down_after_sampling();
		control.set_enable_power_down_after_sampling(value);
		EXPECT_EQ(control.get_enable_power_down_after_sampling(), value);
	}

	{
		auto value = !control.get_stop_recording();
		control.set_stop_recording(value);
		EXPECT_EQ(control.get_stop_recording(), value);
	}

	{
		auto value = !control.get_start_recording();
		control.set_start_recording(value);
		EXPECT_EQ(control.get_start_recording(), value);
	}

	{
		auto value = !control.get_wake_up();
		control.set_wake_up(value);
		EXPECT_EQ(control.get_wake_up(), value);
	}

	MADCControl control_eq = control;
	MADCControl control_default;

	// test comparison
	ASSERT_EQ(control, control_eq);
	ASSERT_FALSE(control == control_default);

	ASSERT_NE(control, control_default);
	ASSERT_FALSE(control != control_eq);
}

TEST(MADCControl, CerealizeCoverage)
{
	MADCControl c1, c2;

	c1.set_enable_iconv_amplifier(true);
	c1.set_enable_pre_amplifier(true);
	c1.set_enable_continuous_sampling(true);
	c1.set_enable_power_down_after_sampling(true);
	c1.set_stop_recording(true);
	c1.set_start_recording(true);
	c1.set_wake_up(true);

	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(c1);
	}
	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(c2);
	}
	ASSERT_EQ(c1, c2);
}

TEST(MADCControl, EncodeDecode)
{
	MADCControl control;
	control.set_enable_iconv_amplifier(true);
	control.set_enable_pre_amplifier(true);
	control.set_enable_continuous_sampling(false);
	control.set_enable_power_down_after_sampling(true);
	control.set_stop_recording(false);
	control.set_start_recording(false);
	control.set_wake_up(true);

	auto const coord = MADCControlOnDLS(0);

	std::array<
	    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, MADCControl::write_config_size_in_words>
	    ref_addresses = {halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{0x000c'0000}};
	std::array<
	    fisch::vx::word_access_type::OmnibusChipOverJTAG, MADCControl::write_config_size_in_words>
	    ref_data = {fisch::vx::word_access_type::OmnibusChipOverJTAG(0b1101001)};

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(
		    control, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(control, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
}

TEST(MADCConfig, General)
{
	MADCConfig config;

	// test getter/setter
	{
		auto value_in = draw_ranged_non_default_value<decltype(
		    config.get_active_mux_initially_selected_input())>(
		    config.get_active_mux_initially_selected_input());
		config.set_active_mux_initially_selected_input(value_in);
		auto value_out = config.get_active_mux_initially_selected_input();
		EXPECT_EQ(value_in, value_out);
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(config.get_active_mux_input_select_length())>(
		        config.get_active_mux_input_select_length());
		config.set_active_mux_input_select_length(value_in);
		auto value_out = config.get_active_mux_input_select_length();
		EXPECT_EQ(value_in, value_out);
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(config.get_sample_duration_adjust())>(
		        config.get_sample_duration_adjust());
		config.set_sample_duration_adjust(value_in);
		auto value_out = config.get_sample_duration_adjust();
		EXPECT_EQ(value_in, value_out);
	}

	{
		bool value_in = !config.get_enable_sar_reset_on_fall();
		config.set_enable_sar_reset_on_fall(value_in);
		bool value_out = config.get_enable_sar_reset_on_fall();
		EXPECT_EQ(value_in, value_out);
	}

	{
		auto value_in = draw_ranged_non_default_value<decltype(config.get_sar_reset_wait())>(
		    config.get_sar_reset_wait());
		config.set_sar_reset_wait(value_in);
		auto value_out = config.get_sar_reset_wait();
		EXPECT_EQ(value_in, value_out);
	}

	{
		auto value_in = draw_ranged_non_default_value<decltype(config.get_sar_reset_length())>(
		    config.get_sar_reset_length());
		config.set_sar_reset_length(value_in);
		auto value_out = config.get_sar_reset_length();
		EXPECT_EQ(value_in, value_out);
	}

	{
		auto value_in = draw_ranged_non_default_value<decltype(config.get_powerup_wait_value())>(
		    config.get_powerup_wait_value());
		config.set_powerup_wait_value(value_in);
		auto value_out = config.get_powerup_wait_value();
		EXPECT_EQ(value_in, value_out);
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(config.get_conversion_cycles_offset())>(
		        config.get_conversion_cycles_offset());
		config.set_conversion_cycles_offset(value_in);
		auto value_out = config.get_conversion_cycles_offset();
		EXPECT_EQ(value_in, value_out);
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(config.get_calibration_wait_value())>(
		        config.get_calibration_wait_value());
		config.set_calibration_wait_value(value_in);
		auto value_out = config.get_calibration_wait_value();
		EXPECT_EQ(value_in, value_out);
	}

	{
		bool value_in = !config.get_enable_calibration();
		config.set_enable_calibration(value_in);
		bool value_out = config.get_enable_calibration();
		EXPECT_EQ(value_in, value_out);
	}

	{
		auto value_in = draw_ranged_non_default_value<decltype(config.get_number_of_samples())>(
		    config.get_number_of_samples());
		config.set_number_of_samples(value_in);
		auto value_out = config.get_number_of_samples();
		EXPECT_EQ(value_in, value_out);
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(config.get_preamp_sampling_window_start())>(
		        config.get_preamp_sampling_window_start());
		config.set_preamp_sampling_window_start(value_in);
		auto value_out = config.get_preamp_sampling_window_start();
		EXPECT_EQ(value_in, value_out);
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(config.get_preamp_sampling_window_end())>(
		        config.get_preamp_sampling_window_end());
		config.set_preamp_sampling_window_end(value_in);
		auto value_out = config.get_preamp_sampling_window_end();
		EXPECT_EQ(value_in, value_out);
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(config.get_iconv_sampling_window_start())>(
		        config.get_iconv_sampling_window_start());
		config.set_iconv_sampling_window_start(value_in);
		auto value_out = config.get_iconv_sampling_window_start();
		EXPECT_EQ(value_in, value_out);
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(config.get_iconv_sampling_window_end())>(
		        config.get_iconv_sampling_window_end());
		config.set_iconv_sampling_window_end(value_in);
		auto value_out = config.get_iconv_sampling_window_end();
		EXPECT_EQ(value_in, value_out);
	}

	{
		bool value_in = !config.get_sample_on_positive_edge();
		config.set_sample_on_positive_edge(value_in);
		bool value_out = config.get_sample_on_positive_edge();
		EXPECT_EQ(value_in, value_out);
	}

	{
		bool value_in = !config.get_enable_dummy_data();
		config.set_enable_dummy_data(value_in);
		bool value_out = config.get_enable_dummy_data();
		EXPECT_EQ(value_in, value_out);
	}

	{
		bool value_in = !config.get_connect_preamp_to_madc();
		config.set_connect_preamp_to_madc(value_in);
		bool value_out = config.get_connect_preamp_to_madc();
		EXPECT_EQ(value_in, value_out);
	}

	{
		bool value_in = !config.get_connect_pads_to_madc();
		config.set_connect_pads_to_madc(value_in);
		bool value_out = config.get_connect_pads_to_madc();
		EXPECT_EQ(value_in, value_out);
	}

	{
		bool value_in = !config.get_connect_preamp_to_pads();
		config.set_connect_preamp_to_pads(value_in);
		bool value_out = config.get_connect_preamp_to_pads();
		EXPECT_EQ(value_in, value_out);
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(config.get_preamp_gain_capacitor_size())>(
		        config.get_preamp_gain_capacitor_size());
		config.set_preamp_gain_capacitor_size(value_in);
		auto value_out = config.get_preamp_gain_capacitor_size();
		EXPECT_EQ(value_in, value_out);
	}

	{
		bool value_in = !config.get_enable_madc_clock_scaling();
		config.set_enable_madc_clock_scaling(value_in);
		bool value_out = config.get_enable_madc_clock_scaling();
		EXPECT_EQ(value_in, value_out);
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(config.get_madc_clock_scale_value())>(
		        config.get_madc_clock_scale_value());
		config.set_madc_clock_scale_value(value_in);
		auto value_out = config.get_madc_clock_scale_value();
		EXPECT_EQ(value_in, value_out);
	}

	{
		bool value_in = !config.get_enable_active_mux_amplifiers();
		config.set_enable_active_mux_amplifiers(value_in);
		bool value_out = config.get_enable_active_mux_amplifiers();
		EXPECT_EQ(value_in, value_out);
	}

	{
		auto value_in = config.get_enable_pseudo_differential_reference();
		for (auto coord : iter_all<HemisphereOnDLS>()) {
			value_in[coord] = !value_in[coord];
		}
		config.set_enable_pseudo_differential_reference(value_in);
		EXPECT_EQ(config.get_enable_pseudo_differential_reference(), value_in);
	}

	{
		bool value_in = !config.get_signal_selection_connect_iconv();
		config.set_signal_selection_connect_iconv(value_in);
		bool value_out = config.get_signal_selection_connect_iconv();
		EXPECT_EQ(value_in, value_out);
	}

	{
		bool value_in = !config.get_signal_selection_connect_active_mux();
		config.set_signal_selection_connect_active_mux(value_in);
		bool value_out = config.get_signal_selection_connect_active_mux();
		EXPECT_EQ(value_in, value_out);
	}

	{
		bool value_in = !config.get_signal_selection_connect_debug();
		config.set_signal_selection_connect_debug(value_in);
		bool value_out = config.get_signal_selection_connect_debug();
		EXPECT_EQ(value_in, value_out);
	}

	{
		bool value_in = !config.get_signal_selection_connect_preamp();
		config.set_signal_selection_connect_preamp(value_in);
		bool value_out = config.get_signal_selection_connect_preamp();
		EXPECT_EQ(value_in, value_out);
	}

	{
		bool value_in = !config.get_enable_iconv_amplifier();
		config.set_enable_iconv_amplifier(value_in);
		bool value_out = config.get_enable_iconv_amplifier();
		EXPECT_EQ(value_in, value_out);
	}

	{
		auto value_in = config.get_connect_iconv_neuron();
		for (auto coord : iter_all<HemisphereOnDLS>()) {
			value_in[coord] = !value_in[coord];
		}
		config.set_connect_iconv_neuron(value_in);
		EXPECT_EQ(config.get_connect_iconv_neuron(), value_in);
	}

	{
		auto value_in = config.get_connect_iconv_synapse();
		for (auto coord : iter_all<SynapticInputOnNeuron>()) {
			value_in[coord] = !value_in[coord];
		}
		config.set_connect_iconv_synapse(value_in);
		EXPECT_EQ(config.get_connect_iconv_synapse(), value_in);
	}

	MADCConfig config_eq = config;
	MADCConfig config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(MADCConfig, CerealizeCoverage)
{
	MADCConfig c1, c2;
	MADCConfig config;

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(c1.get_active_mux_initially_selected_input())>(
		        c1.get_active_mux_initially_selected_input());
		c1.set_active_mux_initially_selected_input(value_in);
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(c1.get_active_mux_input_select_length())>(
		        c1.get_active_mux_input_select_length());
		c1.set_active_mux_input_select_length(value_in);
	}

	{
		auto value_in = draw_ranged_non_default_value<decltype(c1.get_sample_duration_adjust())>(
		    c1.get_sample_duration_adjust());
		c1.set_sample_duration_adjust(value_in);
	}

	{
		bool value_in = !c1.get_enable_sar_reset_on_fall();
		c1.set_enable_sar_reset_on_fall(value_in);
	}

	{
		auto value_in = draw_ranged_non_default_value<decltype(c1.get_sar_reset_wait())>(
		    c1.get_sar_reset_wait());
		c1.set_sar_reset_wait(value_in);
	}

	{
		auto value_in = draw_ranged_non_default_value<decltype(c1.get_sar_reset_length())>(
		    c1.get_sar_reset_length());
		c1.set_sar_reset_length(value_in);
	}

	{
		auto value_in = draw_ranged_non_default_value<decltype(c1.get_powerup_wait_value())>(
		    c1.get_powerup_wait_value());
		c1.set_powerup_wait_value(value_in);
	}

	{
		auto value_in = draw_ranged_non_default_value<decltype(c1.get_conversion_cycles_offset())>(
		    c1.get_conversion_cycles_offset());
		c1.set_conversion_cycles_offset(value_in);
	}

	{
		auto value_in = draw_ranged_non_default_value<decltype(c1.get_calibration_wait_value())>(
		    c1.get_calibration_wait_value());
		c1.set_calibration_wait_value(value_in);
	}

	{
		bool value_in = !c1.get_enable_calibration();
		c1.set_enable_calibration(value_in);
	}

	{
		auto value_in = draw_ranged_non_default_value<decltype(config.get_number_of_samples())>(
		    config.get_number_of_samples());
		config.set_number_of_samples(value_in);
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(c1.get_preamp_sampling_window_start())>(
		        c1.get_preamp_sampling_window_start());
		c1.set_preamp_sampling_window_start(value_in);
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(c1.get_preamp_sampling_window_end())>(
		        c1.get_preamp_sampling_window_end());
		c1.set_preamp_sampling_window_end(value_in);
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(c1.get_iconv_sampling_window_start())>(
		        c1.get_iconv_sampling_window_start());
		c1.set_iconv_sampling_window_start(value_in);
	}

	{
		auto value_in = draw_ranged_non_default_value<decltype(c1.get_iconv_sampling_window_end())>(
		    c1.get_iconv_sampling_window_end());
		c1.set_iconv_sampling_window_end(value_in);
	}

	{
		bool value_in = !c1.get_sample_on_positive_edge();
		c1.set_sample_on_positive_edge(value_in);
	}

	{
		bool value_in = !c1.get_enable_dummy_data();
		c1.set_enable_dummy_data(value_in);
	}

	{
		bool value_in = !c1.get_connect_preamp_to_madc();
		c1.set_connect_preamp_to_madc(value_in);
	}

	{
		bool value_in = !c1.get_connect_pads_to_madc();
		c1.set_connect_pads_to_madc(value_in);
	}

	{
		bool value_in = !c1.get_connect_preamp_to_pads();
		c1.set_connect_preamp_to_pads(value_in);
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(c1.get_preamp_gain_capacitor_size())>(
		        c1.get_preamp_gain_capacitor_size());
		c1.set_preamp_gain_capacitor_size(value_in);
	}

	{
		bool value_in = !c1.get_enable_madc_clock_scaling();
		c1.set_enable_madc_clock_scaling(value_in);
	}

	{
		auto value_in = draw_ranged_non_default_value<decltype(c1.get_madc_clock_scale_value())>(
		    c1.get_madc_clock_scale_value());
		c1.set_madc_clock_scale_value(value_in);
	}

	{
		bool value_in = !c1.get_enable_active_mux_amplifiers();
		c1.set_enable_active_mux_amplifiers(value_in);
	}

	{
		auto value_in = c1.get_enable_pseudo_differential_reference();
		for (auto coord : iter_all<HemisphereOnDLS>()) {
			value_in[coord] = !value_in[coord];
		}
		c1.set_enable_pseudo_differential_reference(value_in);
	}

	{
		bool value_in = !c1.get_signal_selection_connect_iconv();
		c1.set_signal_selection_connect_iconv(value_in);
	}

	{
		bool value_in = !c1.get_signal_selection_connect_active_mux();
		c1.set_signal_selection_connect_active_mux(value_in);
	}

	{
		bool value_in = !c1.get_signal_selection_connect_debug();
		c1.set_signal_selection_connect_debug(value_in);
	}

	{
		bool value_in = !c1.get_signal_selection_connect_preamp();
		c1.set_signal_selection_connect_preamp(value_in);
	}

	{
		bool value_in = !c1.get_enable_iconv_amplifier();
		c1.set_enable_iconv_amplifier(value_in);
	}

	{
		auto value_in = c1.get_connect_iconv_neuron();
		for (auto coord : iter_all<HemisphereOnDLS>()) {
			value_in[coord] = !value_in[coord];
		}
		c1.set_connect_iconv_neuron(value_in);
	}

	{
		auto value_in = c1.get_connect_iconv_synapse();
		for (auto coord : iter_all<SynapticInputOnNeuron>()) {
			value_in[coord] = !value_in[coord];
		}
		c1.set_connect_iconv_synapse(value_in);
	}

	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(c1);
	}
	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(c2);
	}
	ASSERT_EQ(c1, c2);
}

TEST(MADCConfig, EncodeDecode)
{
	MADCConfig config;

	// manually implement the expected bitformatting
	std::array<uint32_t, MADCConfig::config_size_in_words> ref_data_raw{};

	{
		auto value_in = draw_ranged_non_default_value<decltype(
		    config.get_active_mux_initially_selected_input())>(
		    config.get_active_mux_initially_selected_input());
		config.set_active_mux_initially_selected_input(value_in);

		ref_data_raw[0] |= value_in.value() << 3;
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(config.get_active_mux_input_select_length())>(
		        config.get_active_mux_input_select_length());
		config.set_active_mux_input_select_length(value_in);

		ref_data_raw[0] |= value_in.value() << 4;
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(config.get_sample_duration_adjust())>(
		        config.get_sample_duration_adjust());
		config.set_sample_duration_adjust(value_in);

		ref_data_raw[0] |= value_in.value() << 8;
	}

	{
		bool value_in = !config.get_enable_sar_reset_on_fall();
		config.set_enable_sar_reset_on_fall(value_in);

		ref_data_raw[0] |= value_in << 11;
	}

	{
		auto value_in = draw_ranged_non_default_value<decltype(config.get_sar_reset_wait())>(
		    config.get_sar_reset_wait());
		config.set_sar_reset_wait(value_in);

		ref_data_raw[0] |= value_in.value() << 12;
	}

	{
		auto value_in = draw_ranged_non_default_value<decltype(config.get_sar_reset_length())>(
		    config.get_sar_reset_length());
		config.set_sar_reset_length(value_in);

		ref_data_raw[0] |= value_in.value() << 14;
	}

	{
		auto value_in = draw_ranged_non_default_value<decltype(config.get_powerup_wait_value())>(
		    config.get_powerup_wait_value());
		config.set_powerup_wait_value(value_in);

		ref_data_raw[1] |= value_in.value() << 0;
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(config.get_conversion_cycles_offset())>(
		        config.get_conversion_cycles_offset());
		config.set_conversion_cycles_offset(value_in);

		ref_data_raw[1] |= value_in.value() << 12;
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(config.get_calibration_wait_value())>(
		        config.get_calibration_wait_value());
		config.set_calibration_wait_value(value_in);

		ref_data_raw[2] |= value_in.value() << 0;
	}

	{
		bool value_in = !config.get_enable_calibration();
		config.set_enable_calibration(value_in);

		ref_data_raw[2] |= value_in << 15;
	}

	{
		auto value_in = draw_ranged_non_default_value<decltype(config.get_number_of_samples())>(
		    config.get_number_of_samples());
		config.set_number_of_samples(value_in);

		ref_data_raw[3] |= value_in.value();
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(config.get_preamp_sampling_window_start())>(
		        config.get_preamp_sampling_window_start());
		config.set_preamp_sampling_window_start(value_in);

		ref_data_raw[4] |= value_in.value() << 0;
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(config.get_preamp_sampling_window_end())>(
		        config.get_preamp_sampling_window_end());
		config.set_preamp_sampling_window_end(value_in);

		ref_data_raw[4] |= value_in.value() << 5;
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(config.get_iconv_sampling_window_start())>(
		        config.get_iconv_sampling_window_start());
		config.set_iconv_sampling_window_start(value_in);

		ref_data_raw[5] |= value_in.value() << 0;
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(config.get_iconv_sampling_window_end())>(
		        config.get_iconv_sampling_window_end());
		config.set_iconv_sampling_window_end(value_in);

		ref_data_raw[5] |= value_in.value() << 5;
	}

	{
		bool value_in = !config.get_sample_on_positive_edge();
		config.set_sample_on_positive_edge(value_in);

		ref_data_raw[6] |= value_in << 0;
	}

	{
		bool value_in = !config.get_enable_dummy_data();
		config.set_enable_dummy_data(value_in);

		ref_data_raw[6] |= value_in << 1;
	}

	{
		bool value_in = !config.get_connect_preamp_to_madc();
		config.set_connect_preamp_to_madc(value_in);

		ref_data_raw[7] |= value_in << 0;
	}

	{
		bool value_in = !config.get_connect_pads_to_madc();
		config.set_connect_pads_to_madc(value_in);

		ref_data_raw[7] |= value_in << 1;
	}

	{
		bool value_in = !config.get_connect_preamp_to_pads();
		config.set_connect_preamp_to_pads(value_in);

		ref_data_raw[7] |= value_in << 2;
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(config.get_preamp_gain_capacitor_size())>(
		        config.get_preamp_gain_capacitor_size());
		config.set_preamp_gain_capacitor_size(value_in);

		ref_data_raw[7] |= value_in.value() << 3;
	}

	{
		bool value_in = !config.get_enable_madc_clock_scaling();
		config.set_enable_madc_clock_scaling(value_in);

		ref_data_raw[8] |= value_in << 0;
	}

	{
		auto value_in =
		    draw_ranged_non_default_value<decltype(config.get_madc_clock_scale_value())>(
		        config.get_madc_clock_scale_value());
		config.set_madc_clock_scale_value(value_in);

		ref_data_raw[8] |= value_in.value() << 1;
	}

	{
		bool value_in = !config.get_enable_active_mux_amplifiers();
		config.set_enable_active_mux_amplifiers(value_in);

		ref_data_raw[9] |= value_in << 0;
	}

	{
		auto value_in = config.get_enable_pseudo_differential_reference();
		for (auto coord : iter_all<HemisphereOnDLS>()) {
			value_in[coord] = !value_in[coord];
		}
		config.set_enable_pseudo_differential_reference(value_in);

		ref_data_raw[9] |= value_in[HemisphereOnDLS(0)] << 1;
		ref_data_raw[9] |= value_in[HemisphereOnDLS(1)] << 2;
	}

	{
		bool value_in = !config.get_signal_selection_connect_iconv();
		config.set_signal_selection_connect_iconv(value_in);

		ref_data_raw[9] |= value_in << 3;
	}

	{
		bool value_in = !config.get_signal_selection_connect_active_mux();
		config.set_signal_selection_connect_active_mux(value_in);

		ref_data_raw[9] |= value_in << 4;
	}

	{
		bool value_in = !config.get_signal_selection_connect_debug();
		config.set_signal_selection_connect_debug(value_in);

		ref_data_raw[9] |= value_in << 5;
	}

	{
		bool value_in = !config.get_signal_selection_connect_preamp();
		config.set_signal_selection_connect_preamp(value_in);

		ref_data_raw[9] |= value_in << 6;
	}

	{
		bool value_in = !config.get_enable_iconv_amplifier();
		config.set_enable_iconv_amplifier(value_in);

		ref_data_raw[9] |= value_in << 7;
	}

	{
		auto value_in = config.get_connect_iconv_neuron();
		for (auto coord : iter_all<HemisphereOnDLS>()) {
			value_in[coord] = !value_in[coord];
		}
		config.set_connect_iconv_neuron(value_in);

		ref_data_raw[9] |= value_in[HemisphereOnDLS(0)] << 8;
		ref_data_raw[9] |= value_in[HemisphereOnDLS(1)] << 9;
	}

	{
		auto value_in = config.get_connect_iconv_synapse();
		for (auto coord : iter_all<SynapticInputOnNeuron>()) {
			value_in[coord] = !value_in[coord];
		}
		config.set_connect_iconv_synapse(value_in);

		ref_data_raw[9] |= value_in[SynapticInputOnNeuron::inhibitory] << 10;
		ref_data_raw[9] |= value_in[SynapticInputOnNeuron::excitatory] << 11;
	}

	auto const coord = MADCConfigOnDLS();

	// compare addresses to reference addresses
	std::array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, MADCConfig::config_size_in_words>
	    ref_addresses = {halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{0x000c'0001},
	                     halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{0x000c'0002},
	                     halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{0x000c'0003},
	                     halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{0x000c'0004},
	                     halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{0x000c'0005},
	                     halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{0x000c'0006},
	                     halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{0x000c'0007},
	                     halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{0x000c'0008},
	                     halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{0x000c'0009},
	                     halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{0x000c'000a}};

	{
		addresses_type addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{addresses});
		EXPECT_THAT(addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// compare data to reference data
	std::array<word_type, MADCConfig::config_size_in_words> ref_data;
	std::transform(
	    ref_data_raw.begin(), ref_data_raw.end(), ref_data.begin(),
	    [](uint32_t const& w) { return word_type(w); });

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
}
