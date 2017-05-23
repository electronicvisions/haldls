#include <gtest/gtest.h>

#include "haldls/container/v2/neuron.h"

using namespace haldls::container::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;

TEST(CommonNeuronConfig, General)
{
	CommonNeuronConfig config;

	// test default constructor
	ASSERT_EQ(config.get_enable_digital_out(), false);
	ASSERT_EQ(
	    config.get_post_correlation_signal_length(),
	    CommonNeuronConfig::PostCorrelationSignalLength(0));
	ASSERT_EQ(config.get_enable_external_post_correlation_signal(), false);
	ASSERT_EQ(config.get_inhibit_spike_comparator(), false);

	EXPECT_ANY_THROW(CommonNeuronConfig::PostCorrelationSignalLength(16));
	EXPECT_NO_THROW(CommonNeuronConfig::PostCorrelationSignalLength(15));

	config.set_enable_digital_out(true);
	ASSERT_EQ(config.get_enable_digital_out(), true);

	config.set_post_correlation_signal_length(CommonNeuronConfig::PostCorrelationSignalLength(15));
	ASSERT_EQ(
	    config.get_post_correlation_signal_length(),
	    CommonNeuronConfig::PostCorrelationSignalLength(15));

	config.set_enable_external_post_correlation_signal(true);
	ASSERT_EQ(config.get_enable_external_post_correlation_signal(), true);

	config.set_inhibit_spike_comparator(true);
	ASSERT_EQ(config.get_inhibit_spike_comparator(), true);

	CommonNeuronConfig config_eq = config;
	CommonNeuronConfig config_ne = config;
	config_ne.set_inhibit_spike_comparator(false);
	config_ne.set_post_correlation_signal_length(CommonNeuronConfig::PostCorrelationSignalLength(13));


	// test relation operator
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_ne);

	ASSERT_NE(config, config_ne);
	ASSERT_FALSE(config != config_eq);
}


TEST(NeuronDigitalConfig, General)
{
	NeuronDigitalConfig neuron;

	// test default constructor
	ASSERT_EQ(neuron.get_enable_synapse_input_excitatory(), false);
	ASSERT_EQ(neuron.get_enable_synapse_input_inhibitory(), false);
	ASSERT_EQ(neuron.get_enable_high_conductance_leak(), false);
	ASSERT_EQ(neuron.get_enable_leak(), true);
	ASSERT_EQ(neuron.get_enable_smallcap(), true);
	ASSERT_EQ(neuron.get_fire_out_mode(), NeuronDigitalConfig::FireOutMode::disabled);
	ASSERT_EQ(neuron.get_mux_readout_mode(), NeuronDigitalConfig::MuxReadoutMode::v_mem);

	neuron.set_enable_synapse_input_excitatory(true);
	ASSERT_EQ(neuron.get_enable_synapse_input_excitatory(), true);

	neuron.set_enable_synapse_input_inhibitory(true);
	ASSERT_EQ(neuron.get_enable_synapse_input_inhibitory(), true);

	neuron.set_enable_high_conductance_leak(true);
	ASSERT_EQ(neuron.get_enable_high_conductance_leak(), true);

	neuron.set_enable_leak(false);
	ASSERT_EQ(neuron.get_enable_leak(), false);

	neuron.set_enable_bigcap(false);
	ASSERT_EQ(neuron.get_enable_bigcap(), false);

	neuron.set_enable_smallcap(false);
	ASSERT_EQ(neuron.get_enable_smallcap(), false);

	neuron.set_fire_out_mode(NeuronDigitalConfig::FireOutMode::bypass_exc);
	ASSERT_EQ(neuron.get_fire_out_mode(), NeuronDigitalConfig::FireOutMode::bypass_exc);

	neuron.set_mux_readout_mode(NeuronDigitalConfig::MuxReadoutMode::i_psp);
	ASSERT_EQ(neuron.get_mux_readout_mode(), NeuronDigitalConfig::MuxReadoutMode::i_psp);

	NeuronDigitalConfig neuron2 = neuron;
	NeuronDigitalConfig neuron3 = neuron;
	neuron3.set_fire_out_mode(NeuronDigitalConfig::FireOutMode::bypass_inh);

	ASSERT_EQ(neuron, neuron2);
	ASSERT_FALSE(neuron == neuron3);

	ASSERT_NE(neuron, neuron3);
	ASSERT_FALSE(neuron != neuron2);
}
