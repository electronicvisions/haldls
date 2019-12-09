#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/jtag.h"
#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/readout.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<fisch::vx::OmnibusChipOverJTAG> words_type;

TEST(ReadoutBufferConfigBlock, General)
{
	ReadoutBufferConfigBlock::ReadoutBufferConfig config;

	{
		auto member = config.get_differential_signal();
		member = !member;
		config.set_differential_signal(member);
		EXPECT_EQ(config.get_differential_signal(), member);
	}

	{
		auto member = config.get_differential_reference();
		member = !member;
		config.set_differential_reference(member);
		EXPECT_EQ(config.get_differential_reference(), member);
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

	{
		auto member = config.get_enable_buffer();
		member = !member;
		config.set_enable_buffer(member);
		EXPECT_EQ(config.get_enable_buffer(), member);
	}

	ReadoutBufferConfigBlock block_config;
	for (auto coord : iter_all<ReadoutBufferConfigOnReadoutBufferConfigBlock>()) {
		block_config.set_buffer(coord, config);
	}

	ReadoutBufferConfigBlock::ReadoutBufferConfig default_config;

	ASSERT_NE(config, default_config);
	ASSERT_TRUE(config != default_config);
	ASSERT_FALSE(config == default_config);

	default_config = config;
	ASSERT_EQ(config, default_config);
	ASSERT_TRUE(config == default_config);
	ASSERT_FALSE(config != default_config);

	ReadoutBufferConfigBlock default_block_config;

	ASSERT_NE(block_config, default_block_config);
	ASSERT_TRUE(block_config != default_block_config);
	ASSERT_FALSE(block_config == default_block_config);

	default_block_config = block_config;
	ASSERT_EQ(block_config, default_block_config);
	ASSERT_TRUE(block_config == default_block_config);
	ASSERT_FALSE(block_config != default_block_config);
}

TEST(ReadoutBufferConfigBlock, EncodeDecode)
{
	ReadoutBufferConfigBlock config;

	auto coord = typename ReadoutBufferConfigBlock::coordinate_type();

	std::array<OmnibusChipOverJTAGAddress, ReadoutBufferConfigBlock::config_size_in_words>
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
	EXPECT_EQ(default_data[0].get(), 0x0);
	EXPECT_EQ(default_data[1].get(), 0x0);

	// Encode some connections
	auto buffer_0 = config.get_buffer(ReadoutBufferConfigOnReadoutBufferConfigBlock(0));
	auto buffer_1 = config.get_buffer(ReadoutBufferConfigOnReadoutBufferConfigBlock(1));

	buffer_0.set_synin_debug_inhibitory(true);
	buffer_0.set_cadc_debug_causal(true);
	{
		auto member = buffer_0.get_neuron_odd();
		member[HemisphereOnDLS(0)] = true;
		buffer_0.set_neuron_odd(member);
	}

	buffer_1.set_current_dac(true);
	buffer_1.set_differential_signal(true);
	buffer_1.set_enable_buffer(true);

	config.set_buffer(ReadoutBufferConfigOnReadoutBufferConfigBlock(0), buffer_0);
	config.set_buffer(ReadoutBufferConfigOnReadoutBufferConfigBlock(1), buffer_1);

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_EQ(data[0].get(), 0x228);
	EXPECT_EQ(data[1].get(), 0x2005);

	// Decode
	ReadoutBufferConfigBlock decoded;
	ASSERT_NE(config, decoded);
	visit_preorder(decoded, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, decoded);
}

TEST(ReadoutBufferConfigBlock, CerealizeCoverage)
{
	ReadoutBufferConfigBlock c1, c2;
	ReadoutBufferConfigBlock::ReadoutBufferConfig config;

	{
		auto member = config.get_differential_signal();
		member = !member;
		config.set_differential_signal(member);
	}

	{
		auto member = config.get_differential_reference();
		member = !member;
		config.set_differential_reference(member);
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

	{
		auto member = config.get_enable_buffer();
		member = !member;
		config.set_enable_buffer(member);
	}

	for (auto coord : iter_all<ReadoutBufferConfigOnReadoutBufferConfigBlock>()) {
		c1.set_buffer(coord, config);
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
