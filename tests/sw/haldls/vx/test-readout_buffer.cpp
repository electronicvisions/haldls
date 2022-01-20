#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/word_access/type/jtag.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/readout.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<fisch::vx::word_access_type::OmnibusChipOverJTAG> words_type;

TEST(ReadoutSourceSelection, General)
{
	ReadoutSourceSelection::SourceMultiplexer config;

	{
		auto member = config.get_debug_plus();
		member = !member;
		config.set_debug_plus(member);
		EXPECT_EQ(config.get_debug_plus(), member);
	}

	{
		auto member = config.get_debug_minus();
		member = !member;
		config.set_debug_minus(member);
		EXPECT_EQ(config.get_debug_minus(), member);
	}

	{
		auto member = config.get_current_dac();
		member = !member;
		config.set_current_dac(member);
		EXPECT_EQ(config.get_current_dac(), member);
	}

	{
		auto member = config.get_synin_debug_inhibitory();
		member = !member;
		config.set_synin_debug_inhibitory(member);
		EXPECT_EQ(config.get_synin_debug_inhibitory(), member);
	}

	{
		auto member = config.get_synin_debug_excitatory();
		member = !member;
		config.set_synin_debug_excitatory(member);
		EXPECT_EQ(config.get_synin_debug_excitatory(), member);
	}

	{
		auto member = config.get_cadc_debug_causal();
		member = !member;
		config.set_cadc_debug_causal(member);
		EXPECT_EQ(config.get_cadc_debug_causal(), member);
	}

	{
		auto member = config.get_cadc_debug_acausal();
		member = !member;
		config.set_cadc_debug_acausal(member);
		EXPECT_EQ(config.get_cadc_debug_acausal(), member);
	}

	{
		auto member = config.get_synapse_driver_debug();
		for (auto coord : iter_all<HemisphereOnDLS>()) {
			member[coord] = !member[coord];
		}
		config.set_synapse_driver_debug(member);
		EXPECT_EQ(config.get_synapse_driver_debug(), member);
	}

	{
		auto member = config.get_neuron_odd();
		for (auto coord : iter_all<HemisphereOnDLS>()) {
			member[coord] = !member[coord];
		}
		config.set_neuron_odd(member);
		EXPECT_EQ(config.get_neuron_odd(), member);
	}

	{
		auto member = config.get_neuron_even();
		for (auto coord : iter_all<HemisphereOnDLS>()) {
			member[coord] = !member[coord];
		}
		config.set_neuron_even(member);
		EXPECT_EQ(config.get_neuron_even(), member);
	}

	ReadoutSourceSelection block_config;
	for (auto coord : iter_all<SourceMultiplexerOnReadoutSourceSelection>()) {
		block_config.set_buffer(coord, config);
	}

	{
		auto member = block_config.get_enable_buffer_to_pad();
		for (auto coord : iter_all<SourceMultiplexerOnReadoutSourceSelection>()) {
			member[coord] = !member[coord];
		}
		block_config.set_enable_buffer_to_pad(member);
		EXPECT_EQ(block_config.get_enable_buffer_to_pad(), member);
	}

	ReadoutSourceSelection::SourceMultiplexer default_config;

	ASSERT_NE(config, default_config);
	ASSERT_TRUE(config != default_config);
	ASSERT_FALSE(config == default_config);

	default_config = config;
	ASSERT_EQ(config, default_config);
	ASSERT_TRUE(config == default_config);
	ASSERT_FALSE(config != default_config);

	ReadoutSourceSelection default_block_config;

	ASSERT_NE(block_config, default_block_config);
	ASSERT_TRUE(block_config != default_block_config);
	ASSERT_FALSE(block_config == default_block_config);

	default_block_config = block_config;
	ASSERT_EQ(block_config, default_block_config);
	ASSERT_TRUE(block_config == default_block_config);
	ASSERT_FALSE(block_config != default_block_config);
}

TEST(ReadoutSourceSelection, EncodeDecode)
{
	ReadoutSourceSelection config;

	auto coord = typename ReadoutSourceSelection::coordinate_type();

	std::array<OmnibusChipOverJTAGAddress, ReadoutSourceSelection::config_size_in_words>
	    ref_addresses = {OmnibusChipOverJTAGAddress{0xc0000 + 12},
	                     OmnibusChipOverJTAGAddress{0xc0000 + 13}};

	{ // check if write addresses are correct
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// Encode default
	words_type default_data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{default_data});
	EXPECT_EQ(default_data[0], 0x0);
	EXPECT_EQ(default_data[1], 0x0);

	// Encode some connections
	auto buffer_0 = config.get_buffer(SourceMultiplexerOnReadoutSourceSelection(0));
	auto buffer_1 = config.get_buffer(SourceMultiplexerOnReadoutSourceSelection(1));

	buffer_0.set_synin_debug_inhibitory(true);
	buffer_0.set_cadc_debug_causal(true);
	{
		auto member = buffer_0.get_neuron_odd();
		member[HemisphereOnDLS(0)] = true;
		buffer_0.set_neuron_odd(member);
	}

	buffer_1.set_current_dac(true);
	buffer_1.set_debug_plus(true);

	config.set_buffer(SourceMultiplexerOnReadoutSourceSelection(0), buffer_0);
	config.set_buffer(SourceMultiplexerOnReadoutSourceSelection(1), buffer_1);
	{
		auto member = config.get_enable_buffer_to_pad();
		member[SourceMultiplexerOnReadoutSourceSelection(1)] = true;
		config.set_enable_buffer_to_pad(member);
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_EQ(data[0], 0x228);
	EXPECT_EQ(data[1], 0x2005);

	// Decode
	ReadoutSourceSelection decoded;
	ASSERT_NE(config, decoded);
	visit_preorder(decoded, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, decoded);
}

TEST(ReadoutSourceSelection, CerealizeCoverage)
{
	ReadoutSourceSelection c1, c2;
	ReadoutSourceSelection::SourceMultiplexer config;

	{
		auto member = config.get_debug_plus();
		member = !member;
		config.set_debug_plus(member);
	}

	{
		auto member = config.get_debug_minus();
		member = !member;
		config.set_debug_minus(member);
	}

	{
		auto member = config.get_current_dac();
		member = !member;
		config.set_current_dac(member);
	}

	{
		auto member = config.get_synin_debug_inhibitory();
		member = !member;
		config.set_synin_debug_inhibitory(member);
	}

	{
		auto member = config.get_synin_debug_excitatory();
		member = !member;
		config.set_synin_debug_excitatory(member);
	}

	{
		auto member = config.get_cadc_debug_causal();
		member = !member;
		config.set_cadc_debug_causal(member);
	}

	{
		auto member = config.get_cadc_debug_acausal();
		member = !member;
		config.set_cadc_debug_acausal(member);
	}

	{
		auto member = config.get_synapse_driver_debug();
		for (auto coord : iter_all<HemisphereOnDLS>()) {
			member[coord] = !member[coord];
		}
		config.set_synapse_driver_debug(member);
	}

	{
		auto member = config.get_neuron_odd();
		for (auto coord : iter_all<HemisphereOnDLS>()) {
			member[coord] = !member[coord];
		}
		config.set_neuron_odd(member);
	}

	{
		auto member = config.get_neuron_even();
		for (auto coord : iter_all<HemisphereOnDLS>()) {
			member[coord] = !member[coord];
		}
		config.set_neuron_even(member);
	}

	for (auto coord : iter_all<SourceMultiplexerOnReadoutSourceSelection>()) {
		c1.set_buffer(coord, config);
	}

	{
		auto member = c1.get_enable_buffer_to_pad();
		for (auto coord : iter_all<SourceMultiplexerOnReadoutSourceSelection>()) {
			member[coord] = !member[coord];
		}
		c1.set_enable_buffer_to_pad(member);
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
