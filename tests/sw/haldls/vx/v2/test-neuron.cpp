#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "fisch/vx/jtag.h"
#include "halco/hicann-dls/vx/v2/omnibus.h"
#include "haldls/vx/common.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/v2/neuron.h"
#include "test-helper.h"

using namespace fisch::vx;
using namespace haldls::vx::v2;
using namespace halco::hicann_dls::vx::v2;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<fisch::vx::OmnibusChipOverJTAG> words_type;

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

	value = !config.get_enable_strong_fire();
	config.set_enable_strong_fire(value);
	ASSERT_EQ(config.get_enable_strong_fire(), value);

	value = !config.get_enable_threshold_comparator();
	config.set_enable_threshold_comparator(value);
	ASSERT_EQ(config.get_enable_threshold_comparator(), value);

	value = !config.get_enable_synaptic_input_excitatory();
	config.set_enable_synaptic_input_excitatory(value);
	ASSERT_EQ(config.get_enable_synaptic_input_excitatory(), value);

	value = !config.get_enable_synaptic_input_inhibitory();
	config.set_enable_synaptic_input_inhibitory(value);
	ASSERT_EQ(config.get_enable_synaptic_input_inhibitory(), value);

	value = !config.get_enable_synaptic_input_inhibitory_small_capacitor();
	config.set_enable_synaptic_input_inhibitory_small_capacitor(value);
	ASSERT_EQ(config.get_enable_synaptic_input_inhibitory_small_capacitor(), value);

	value = !config.get_enable_synaptic_input_inhibitory_small_capacitor();
	config.set_enable_synaptic_input_inhibitory_small_capacitor(value);
	ASSERT_EQ(config.get_enable_synaptic_input_inhibitory_small_capacitor(), value);

	value = !config.get_enable_synaptic_input_inhibitory_high_resistance();
	config.set_enable_synaptic_input_inhibitory_high_resistance(value);
	ASSERT_EQ(config.get_enable_synaptic_input_inhibitory_high_resistance(), value);

	value = !config.get_enable_synaptic_input_inhibitory_high_resistance();
	config.set_enable_synaptic_input_inhibitory_high_resistance(value);
	ASSERT_EQ(config.get_enable_synaptic_input_inhibitory_high_resistance(), value);

	value = !config.get_enable_bypass_excitatory();
	config.set_enable_bypass_excitatory(value);
	ASSERT_EQ(config.get_enable_bypass_excitatory(), value);

	value = !config.get_enable_bypass_inhibitory();
	config.set_enable_bypass_inhibitory(value);
	ASSERT_EQ(config.get_enable_bypass_inhibitory(), value);

	value = !config.get_enable_membrane_offset();
	config.set_enable_membrane_offset(value);
	ASSERT_EQ(config.get_enable_membrane_offset(), value);

	value = !config.get_invert_membrane_offset();
	config.set_invert_membrane_offset(value);
	ASSERT_EQ(config.get_invert_membrane_offset(), value);

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

	value = !config.get_enable_pause();
	config.set_enable_pause(value);
	ASSERT_EQ(config.get_enable_pause(), value);

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
	ASSERT_TRUE(data[5].get() & 0b0000001);          // connect_somata
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
