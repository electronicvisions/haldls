#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "fisch/vx/jtag.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/vx/common.h"
#include "haldls/vx/neuron.h"
#include "haldls/vx/omnibus_constants.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<fisch::vx::OmnibusChipOverJTAG> words_type;

TEST(CommonNeuronBackendConfig, General)
{
	CommonNeuronBackendConfig config;

	auto value = !config.get_enable_event_registers();
	config.set_enable_event_registers(value);
	ASSERT_EQ(config.get_enable_event_registers(), value);

	value = !config.get_force_reset();
	config.set_force_reset(value);
	ASSERT_EQ(config.get_force_reset(), value);

	value = !config.get_enable_clocks();
	config.set_enable_clocks(value);
	ASSERT_EQ(config.get_enable_clocks(), value);

	auto clock = draw_ranged_non_default_value<CommonNeuronBackendConfig::ClockScale>();
	config.set_clock_scale_slow(clock);
	ASSERT_EQ(config.get_clock_scale_slow(), clock);

	clock = draw_ranged_non_default_value<CommonNeuronBackendConfig::ClockScale>();
	config.set_clock_scale_fast(clock);
	ASSERT_EQ(config.get_clock_scale_fast(), clock);

	config.set_sample_positive_edge(EventOutputOnNeuronBackendBlock(0), false);
	ASSERT_EQ(config.get_sample_positive_edge(EventOutputOnNeuronBackendBlock(0)), false);

	clock = draw_ranged_non_default_value<CommonNeuronBackendConfig::ClockScale>();
	config.set_clock_scale_adaptation_pulse(clock);
	ASSERT_EQ(config.get_clock_scale_adaptation_pulse(), clock);

	clock = draw_ranged_non_default_value<CommonNeuronBackendConfig::ClockScale>();
	config.set_clock_scale_post_pulse(clock);
	ASSERT_EQ(config.get_clock_scale_post_pulse(), clock);

	auto w_post_debug = draw_ranged_non_default_value<CommonNeuronBackendConfig::WaitGlobalPostPulse>();
	config.set_wait_global_post_pulse(w_post_debug);
	ASSERT_EQ(config.get_wait_global_post_pulse(), w_post_debug);

	auto w_counter_reset = draw_ranged_non_default_value<CommonNeuronBackendConfig::WaitSpikeCounterReset>();
	config.set_wait_spike_counter_reset(w_counter_reset);
	ASSERT_EQ(config.get_wait_spike_counter_reset(), w_counter_reset);

	auto w_counter_read = draw_ranged_non_default_value<CommonNeuronBackendConfig::WaitSpikeCounterRead>();
	config.set_wait_spike_counter_read(w_counter_read);
	ASSERT_EQ(config.get_wait_spike_counter_read(), w_counter_read);

	auto w_post_ind = draw_ranged_non_default_value<CommonNeuronBackendConfig::WaitFireNeuron>();
	config.set_wait_fire_neuron(w_post_ind);
	ASSERT_EQ(config.get_wait_fire_neuron(), w_post_ind);

	CommonNeuronBackendConfig default_config;
	ASSERT_NE(config, default_config);
	ASSERT_TRUE(config != default_config);
	ASSERT_FALSE(config == default_config);
	CommonNeuronBackendConfig copy_config = config;
	ASSERT_EQ(config, copy_config);
	ASSERT_TRUE(config == copy_config);
	ASSERT_FALSE(config != copy_config);
}

TEST(CommonNeuronBackendConfig, EncodeDecode)
{
	CommonNeuronBackendConfig config;
	config.set_enable_event_registers(false);
	config.set_force_reset(true);
	config.set_enable_clocks(true);
	config.set_clock_scale_slow(CommonNeuronBackendConfig::ClockScale(3));
	config.set_clock_scale_fast(CommonNeuronBackendConfig::ClockScale(7));
	config.set_sample_positive_edge(EventOutputOnNeuronBackendBlock(1), false);
	config.set_clock_scale_adaptation_pulse(CommonNeuronBackendConfig::ClockScale(4));
	config.set_clock_scale_post_pulse(CommonNeuronBackendConfig::ClockScale(8));
	config.set_wait_global_post_pulse(CommonNeuronBackendConfig::WaitGlobalPostPulse(186));
	config.set_wait_spike_counter_reset(CommonNeuronBackendConfig::WaitSpikeCounterReset(220));
	config.set_wait_spike_counter_read(CommonNeuronBackendConfig::WaitSpikeCounterRead(13));
	config.set_wait_fire_neuron(CommonNeuronBackendConfig::WaitFireNeuron(237));

	auto backend_coord = NeuronBackendConfigBlockOnDLS(1);

	std::array<OmnibusChipOverJTAGAddress, CommonNeuronBackendConfig::config_size_in_words>
	    ref_addresses = {OmnibusChipOverJTAGAddress{0x1a9800 + 0},
	                     OmnibusChipOverJTAGAddress{0x1a9800 + 1}};

	{ // check if write addresses are correct
		addresses_type write_addresses;
		visit_preorder(
		    config, backend_coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // check if read addresses are correct
		addresses_type read_addresses;
		visit_preorder(
		    config, backend_coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// Encode
	words_type data;
	visit_preorder(config, backend_coord, stadls::EncodeVisitor<words_type>{data});
	ASSERT_FALSE(data[0].get() & 0b1);                                 // en_event_regs
	ASSERT_TRUE(data[0].get() & 0b10);                                 // force_reset
	ASSERT_TRUE(data[0].get() & 0b100);                                // en_clocks
	ASSERT_TRUE((data[0].get() & 0b1111'0000) == (3 << 4));            // clock_scale_slow
	ASSERT_TRUE((data[0].get() & 0b1111'0000'0000) == (7 << 8));       // clock_scale_fast
	ASSERT_TRUE((data[0].get() & 0b1111'0000'0000'0000) == (13 << 12)); // sample_pos_edge
	ASSERT_TRUE(
	    (data[0].get() & 0b1111'0000'0000'0000'0000) == (4 << 16)); // clock_scale_adapt_pulse
	ASSERT_TRUE(
	    (data[0].get() & 0b1111'0000'0000'0000'0000'0000) == (8 << 20)); // clock_scale_post_pulse
	ASSERT_TRUE(
	    (data[1].get() & 0xFF) == (unsigned int)(186 << 0));          // wait_global_post_pulse
	ASSERT_TRUE(
	    (data[1].get() & 0xFF00) == (unsigned int)(220 << 8));        // wait_spike_counter_reset
	ASSERT_TRUE(
	    (data[1].get() & 0xFF0000) == (unsigned int)(13 << 16));      // wait_spike_counter_read
	ASSERT_TRUE(
	    (data[1].get() & 0xFF000000) == (unsigned int)(237 << 24));   // wait_fire_neuron

	// Decode back
	CommonNeuronBackendConfig config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, backend_coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(CommonNeuronBackendConfig, CerealizeCoverage)
{
	CommonNeuronBackendConfig c1, c2;
	c1.set_enable_event_registers(true);
	c1.set_force_reset(true);
	c1.set_enable_clocks(true);
	c1.set_clock_scale_fast(CommonNeuronBackendConfig::ClockScale(7));
	c1.set_clock_scale_slow(CommonNeuronBackendConfig::ClockScale(7));
	c1.set_sample_positive_edge(EventOutputOnNeuronBackendBlock(0), true);
	c1.set_clock_scale_adaptation_pulse(CommonNeuronBackendConfig::ClockScale(10));
	c1.set_clock_scale_post_pulse(CommonNeuronBackendConfig::ClockScale(10));
	c1.set_wait_global_post_pulse(CommonNeuronBackendConfig::WaitGlobalPostPulse(186));
	c1.set_wait_spike_counter_reset(CommonNeuronBackendConfig::WaitSpikeCounterReset(220));
	c1.set_wait_spike_counter_read(CommonNeuronBackendConfig::WaitSpikeCounterRead(13));
	c1.set_wait_fire_neuron(CommonNeuronBackendConfig::WaitFireNeuron(237));
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

TEST(NeuronBackendConfig, General)
{
	NeuronBackendConfig config;

	auto addr = draw_ranged_non_default_value<NeuronBackendConfig::AddressOut>();
	config.set_address_out(addr);
	ASSERT_EQ(config.get_address_out(), addr);

	auto reset = draw_ranged_non_default_value<NeuronBackendConfig::ResetHoldoff>();
	config.set_reset_holdoff(reset);
	ASSERT_EQ(config.get_reset_holdoff(), reset);

	auto refr = draw_ranged_non_default_value<NeuronBackendConfig::RefractoryTime>();
	config.set_refractory_time(refr);
	ASSERT_EQ(config.get_refractory_time(), refr);

	auto clock = NeuronBackendConfig::InputClock(1);
	config.set_select_input_clock(clock);
	ASSERT_EQ(config.get_select_input_clock(), clock);

	auto value = !config.get_post_overwrite();
	config.set_post_overwrite(value);
	ASSERT_EQ(config.get_post_overwrite(), value);


	value = !config.get_enable_adaptation_pulse();
	config.set_enable_adaptation_pulse(value);
	ASSERT_EQ(config.get_enable_adaptation_pulse(), value);

	value = !config.get_enable_bayesian_extension();
	config.set_enable_bayesian_extension(value);
	ASSERT_EQ(config.get_enable_bayesian_extension(), value);

	value = !config.get_enable_neuron_slave();
	config.set_enable_neuron_slave(value);
	ASSERT_EQ(config.get_enable_neuron_slave(), value);

	value = !config.get_connect_fire_bottom();
	config.set_connect_fire_bottom(value);
	ASSERT_EQ(config.get_connect_fire_bottom(), value);

	value = !config.get_connect_fire_from_right();
	config.set_connect_fire_from_right(value);
	ASSERT_EQ(config.get_connect_fire_from_right(), value);

	value = !config.get_connect_fire_to_right();
	config.set_connect_fire_to_right(value);
	ASSERT_EQ(config.get_connect_fire_to_right(), value);

	value = !config.get_enable_spike_out();
	config.set_enable_spike_out(value);
	ASSERT_EQ(config.get_enable_spike_out(), value);

	value = !config.get_enable_neuron_master();
	config.set_enable_neuron_master(value);
	ASSERT_EQ(config.get_enable_neuron_master(), value);

	value = !config.get_enable_bayesian_0();
	config.set_enable_bayesian_0(value);
	ASSERT_EQ(config.get_enable_bayesian_0(), value);

	value = !config.get_enable_bayesian_1();
	config.set_enable_bayesian_1(value);
	ASSERT_EQ(config.get_enable_bayesian_1(), value);

	NeuronBackendConfig default_config;
	ASSERT_NE(config, default_config);
	ASSERT_TRUE(config != default_config);
	ASSERT_FALSE(config == default_config);
	NeuronBackendConfig copy_config = config;
	ASSERT_EQ(config, copy_config);
	ASSERT_TRUE(config == copy_config);
	ASSERT_FALSE(config != copy_config);
}

TEST(NeuronBackendConfig, EncodeDecode)
{
	NeuronBackendConfig config;
	config.set_connect_fire_to_right(true);
	config.set_enable_spike_out(true);
	config.set_address_out(NeuronBackendConfig::AddressOut(7));
	config.set_enable_adaptation_pulse(true);

	// neuron 23, south east block
	auto neuron_coord = NeuronBackendConfigOnDLS(
	    NeuronBackendConfigOnNeuronBackendConfigBlock(23 + 128), NeuronBackendConfigBlockOnDLS(1));

	std::array<OmnibusChipOverJTAGAddress, NeuronBackendConfig::config_size_in_words>
	    ref_addresses = {OmnibusChipOverJTAGAddress{0x1a9000 + 128 * 4 + 23 * 4 + 0},
	                     OmnibusChipOverJTAGAddress{0x1a9000 + 128 * 4 + 23 * 4 + 1},
	                     OmnibusChipOverJTAGAddress{0x1a9000 + 128 * 4 + 23 * 4 + 2},
	                     OmnibusChipOverJTAGAddress{0x1a9000 + 128 * 4 + 23 * 4 + 3}};

	{ // check if write addresses are correct
		addresses_type write_addresses;
		visit_preorder(
		    config, neuron_coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // check if read addresses are correct
		addresses_type read_addresses;
		visit_preorder(
		    config, neuron_coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// Encode
	words_type data;
	visit_preorder(config, neuron_coord, stadls::EncodeVisitor<words_type>{data});
	ASSERT_TRUE(data[3].get() & 0b1000);               // connect_fire_to_right
	ASSERT_TRUE(data[3].get() & 0b10000);              // en_spike_out
	ASSERT_EQ((data[0].get() & 0b00111111), 0b111110); // address_out 6 MSBs
	ASSERT_EQ((data[1].get() & 0b00110000), 0b00);     // address_out 2 LSBs
	ASSERT_TRUE(data[2].get() & 0b1000000);            // en_adapt_pulse

	// Decode back
	NeuronBackendConfig config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, neuron_coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(NeuronBackendConfig, CerealizeCoverage)
{
	NeuronBackendConfig c1, c2;
	c1.set_enable_bayesian_0(true);
	c1.set_enable_neuron_master(true);
	c1.set_connect_fire_from_right(true);
	c1.set_select_input_clock(NeuronBackendConfig::InputClock(1));
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


TEST(NeuronConfig, General)
{
	NeuronConfig config;

	auto value = !config.get_enable_divide_multicomp_conductance_bias();
	config.set_enable_divide_multicomp_conductance_bias(value);
	ASSERT_EQ(config.get_enable_divide_multicomp_conductance_bias(), value);

	value = !config.get_enable_multiply_multicomp_conductance_bias();
	config.set_enable_multiply_multicomp_conductance_bias(value);
	ASSERT_EQ(config.get_enable_multiply_multicomp_conductance_bias(), value);

	value = !config.get_connect_soma();
	config.set_connect_soma(value);
	ASSERT_EQ(config.get_connect_soma(), value);

	value = !config.get_connect_membrane_right();
	config.set_connect_membrane_right(value);
	ASSERT_EQ(config.get_connect_membrane_right(), value);

	value = !config.get_enable_multicomp_conductance();
	config.set_enable_multicomp_conductance(value);
	ASSERT_EQ(config.get_enable_multicomp_conductance(), value);

	value = !config.get_connect_bottom();
	config.set_connect_bottom(value);
	ASSERT_EQ(config.get_connect_bottom(), value);

	value = !config.get_connect_soma_right();
	config.set_connect_soma_right(value);
	ASSERT_EQ(config.get_connect_soma_right(), value);

	value = !config.get_enable_fire();
	config.set_enable_fire(value);
	ASSERT_EQ(config.get_enable_fire(), value);

	value = !config.get_enable_threshold_comparator();
	config.set_enable_threshold_comparator(value);
	ASSERT_EQ(config.get_enable_threshold_comparator(), value);

	value = !config.get_enable_synaptic_input_excitatory();
	config.set_enable_synaptic_input_excitatory(value);
	ASSERT_EQ(config.get_enable_synaptic_input_excitatory(), value);

	value = !config.get_enable_synaptic_input_inhibitory();
	config.set_enable_synaptic_input_inhibitory(value);
	ASSERT_EQ(config.get_enable_synaptic_input_inhibitory(), value);

	value = !config.get_enable_bypass_excitatory();
	config.set_enable_bypass_excitatory(value);
	ASSERT_EQ(config.get_enable_bypass_excitatory(), value);

	value = !config.get_enable_bypass_inhibitory();
	config.set_enable_bypass_inhibitory(value);
	ASSERT_EQ(config.get_enable_bypass_inhibitory(), value);

	value = !config.get_enable_membrane_offset();
	config.set_enable_membrane_offset(value);
	ASSERT_EQ(config.get_enable_membrane_offset(), value);

	value = !config.get_enable_capacitor_merge();
	config.set_enable_capacitor_merge(value);
	ASSERT_EQ(config.get_enable_capacitor_merge(), value);

	auto memcap = NeuronConfig::MembraneCapacitorSize(config.get_membrane_capacitor_size() + 1);
	config.set_membrane_capacitor_size(memcap);
	ASSERT_EQ(config.get_membrane_capacitor_size(), memcap);

	value = !config.get_invert_adaptation_a();
	config.set_invert_adaptation_a(value);
	ASSERT_EQ(config.get_invert_adaptation_a(), value);

	value = !config.get_invert_adaptation_b();
	config.set_invert_adaptation_b(value);
	ASSERT_EQ(config.get_invert_adaptation_b(), value);

	value = !config.get_enable_adaptation();
	config.set_enable_adaptation(value);
	ASSERT_EQ(config.get_enable_adaptation(), value);

	value = !config.get_enable_adaptation_capacitor();
	config.set_enable_adaptation_capacitor(value);
	ASSERT_EQ(config.get_enable_adaptation_capacitor(), value);

	auto strength =
	    NeuronConfig::ExponentialTermStrength(config.get_exponential_term_strength() + 1);
	config.set_exponential_term_strength(strength);
	ASSERT_EQ(config.get_exponential_term_strength(), strength);

	value = !config.get_enable_exponential();
	config.set_enable_exponential(value);
	ASSERT_EQ(config.get_enable_exponential(), value);

	value = !config.get_enable_adaptation_readout();
	config.set_enable_adaptation_readout(value);
	ASSERT_EQ(config.get_enable_adaptation_readout(), value);

	value = !config.get_enable_unbuffered_access();
	config.set_enable_unbuffered_access(value);
	ASSERT_EQ(config.get_enable_unbuffered_access(), value);

	value = !config.get_enable_readout_amplifier();
	config.set_enable_readout_amplifier(value);
	ASSERT_EQ(config.get_enable_readout_amplifier(), value);

	auto readoutsrc = NeuronConfig::ReadoutSource::adaptation;
	config.set_readout_source(readoutsrc);
	ASSERT_EQ(config.get_readout_source(), readoutsrc);

	value = !config.get_enable_readout();
	config.set_enable_readout(value);
	ASSERT_EQ(config.get_enable_readout(), value);

	value = !config.get_enable_reset_degeneration();
	config.set_enable_reset_degeneration(value);
	ASSERT_EQ(config.get_enable_reset_degeneration(), value);

	value = !config.get_enable_reset_division();
	config.set_enable_reset_division(value);
	ASSERT_EQ(config.get_enable_reset_division(), value);

	value = !config.get_enable_reset_multiplication();
	config.set_enable_reset_multiplication(value);
	ASSERT_EQ(config.get_enable_reset_multiplication(), value);

	value = !config.get_enable_leak_degeneration();
	config.set_enable_leak_degeneration(value);
	ASSERT_EQ(config.get_enable_leak_degeneration(), value);

	value = !config.get_enable_leak_division();
	config.set_enable_leak_division(value);
	ASSERT_EQ(config.get_enable_leak_division(), value);

	value = !config.get_enable_leak_multiplication();
	config.set_enable_leak_multiplication(value);
	ASSERT_EQ(config.get_enable_leak_multiplication(), value);

	NeuronConfig default_config;
	ASSERT_NE(config, default_config);
	ASSERT_TRUE(config != default_config);
	ASSERT_FALSE(config == default_config);
	default_config = config;
	ASSERT_EQ(config, default_config);
	ASSERT_TRUE(config == default_config);
	ASSERT_FALSE(config != default_config);
}

TEST(NeuronConfig, EncodeDecode)
{
	NeuronConfig config;
	config.set_enable_readout_amplifier(true);
	config.set_enable_synaptic_input_excitatory(false);
	config.set_connect_soma_right(true);
	config.set_readout_source(NeuronConfig::ReadoutSource::adaptation);

	auto neuron_coord =
	    NeuronConfigOnDLS(NeuronConfigOnNeuronConfigBlock(23), NeuronConfigBlockOnDLS(1));

	std::array<OmnibusChipOverJTAGAddress, NeuronConfig::config_size_in_words> ref_addresses = {
	    OmnibusChipOverJTAGAddress{0x168000 + 23 * 8 + 2},
	    OmnibusChipOverJTAGAddress{0x168000 + 23 * 8 + 3},
	    OmnibusChipOverJTAGAddress{0x168000 + 23 * 8 + 4},
	    OmnibusChipOverJTAGAddress{0x168000 + 23 * 8 + 5},
	    OmnibusChipOverJTAGAddress{0x168000 + 23 * 8 + 6},
	    OmnibusChipOverJTAGAddress{0x168000 + 23 * 8 + 7}};

	{ // check if write addresses are correct
		addresses_type write_addresses;
		visit_preorder(
		    config, neuron_coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // check if read addresses are correct
		addresses_type read_addresses;
		visit_preorder(
		    config, neuron_coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// Encode
	words_type data;
	visit_preorder(config, neuron_coord, stadls::EncodeVisitor<words_type>{data});
	ASSERT_TRUE(data[1].get() & 0b10000);            // en_readout_amp
	ASSERT_TRUE(data[5].get() & 0b1000000);          // connect_somata
	ASSERT_FALSE(data[4].get() & 0b100000);          // en_synin_exc
	ASSERT_EQ(data[1].get() & 0b1100000, 0b1100000); // readout_select

	// Decode back
	NeuronConfig config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, neuron_coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(NeuronConfig, CerealizeCoverage)
{
	NeuronConfig c1, c2;
	c1.set_enable_readout_amplifier(true);
	c1.set_enable_synaptic_input_excitatory(false);
	c1.set_connect_soma_right(true);
	c1.set_readout_source(NeuronConfig::ReadoutSource::adaptation);
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

TEST(NeuronReset, General)
{
	NeuronReset config;

	NeuronReset default_config;
	ASSERT_EQ(config, default_config);
	ASSERT_TRUE(config == default_config);
	ASSERT_FALSE(config != default_config);
}

TEST(NeuronReset, EncodeDecode)
{
	NeuronReset config;

	auto neuron_coord =
	    NeuronResetOnDLS(NeuronResetOnNeuronResetBlock(23), NeuronResetBlockOnDLS(0));

	std::array<OmnibusChipOverJTAGAddress, NeuronReset::write_config_size_in_words> ref_addresses =
	    {OmnibusChipOverJTAGAddress{0x12205c}};

	{ // check if write addresses are correct
		addresses_type write_addresses;
		visit_preorder(
		    config, neuron_coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// Encode
	words_type data;
	visit_preorder(config, neuron_coord, stadls::EncodeVisitor<words_type>{data});
	ASSERT_EQ(data[0].get(), 0x0);
}

TEST(NeuronReset, CerealizeCoverage)
{
	NeuronReset c1, c2;
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

TEST(NeuronResetQuad, General)
{
	NeuronResetQuad config;

	NeuronResetQuad default_config;
	ASSERT_EQ(config, default_config);
	ASSERT_TRUE(config == default_config);
	ASSERT_FALSE(config != default_config);
}

TEST(NeuronResetQuad, EncodeDecode)
{
	NeuronResetQuad config;

	auto neuron_coord = NeuronResetQuadOnDLS(SynapseQuadColumnOnDLS(23), SynramOnDLS(1));

	std::array<OmnibusChipOverJTAGAddress, NeuronResetQuad::write_config_size_in_words>
	    ref_addresses = {OmnibusChipOverJTAGAddress{0x03c94028}};

	{ // check if write addresses are correct
		addresses_type write_addresses;
		visit_preorder(
		    config, neuron_coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// Encode
	words_type data;
	visit_preorder(config, neuron_coord, stadls::EncodeVisitor<words_type>{data});
	ASSERT_EQ(data[0].get(), 0x0);
}

TEST(NeuronResetQuad, CerealizeCoverage)
{
	NeuronResetQuad c1, c2;
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

TEST(SpikeCounterRead, General)
{
	SpikeCounterRead config;
	config.set_count(SpikeCounterRead::Count(100));
	config.set_overflow(true);

	SpikeCounterRead default_config;
	ASSERT_NE(config, default_config);
	ASSERT_TRUE(config != default_config);
	ASSERT_FALSE(config == default_config);
	default_config = config;
	ASSERT_EQ(config, default_config);
	ASSERT_TRUE(config == default_config);
	ASSERT_FALSE(config != default_config);
}

TEST(SpikeCounterRead, EncodeDecode)
{
	SpikeCounterRead config;
	config.set_count(SpikeCounterRead::Count(100));
	config.set_overflow(true);

	auto neuron_coord = SpikeCounterReadOnDLS(
	    SpikeCounterReadOnSpikeCounterReadBlock(22), SpikeCounterReadBlockOnDLS(0));

	std::array<OmnibusChipOverJTAGAddress, SpikeCounterRead::read_config_size_in_words>
	    ref_addresses = {OmnibusChipOverJTAGAddress{0x122858}};

	{ // check if read addresses are correct
		addresses_type read_addresses;
		visit_preorder(
		    config, neuron_coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// Decode
	words_type data = {fisch::vx::OmnibusData(0x164)};
	SpikeCounterRead config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, neuron_coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(SpikeCounterRead, CerealizeCoverage)
{
	SpikeCounterRead c1, c2;
	c1.set_count(SpikeCounterRead::Count(100));
	c1.set_overflow(true);
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

TEST(SpikeCounterReset, General)
{
	SpikeCounterReset config;

	SpikeCounterReset default_config;
	ASSERT_EQ(config, default_config);
	ASSERT_TRUE(config == default_config);
	ASSERT_FALSE(config != default_config);
}

TEST(SpikeCounterReset, EncodeDecode)
{
	SpikeCounterReset config;

	auto neuron_coord = SpikeCounterResetOnDLS(
	    SpikeCounterResetOnSpikeCounterResetBlock(21), SpikeCounterResetBlockOnDLS(0));

	std::array<OmnibusChipOverJTAGAddress, SpikeCounterReset::write_config_size_in_words>
	    ref_addresses = {OmnibusChipOverJTAGAddress{0x122c54}};

	{ // check if write addresses are correct
		addresses_type write_addresses;
		visit_preorder(
		    config, neuron_coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// Encode
	words_type data;
	visit_preorder(config, neuron_coord, stadls::EncodeVisitor<words_type>{data});
	ASSERT_EQ(data[0].get(), 0x0);
}

TEST(SpikeCounterReset, CerealizeCoverage)
{
	SpikeCounterReset c1, c2;
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
