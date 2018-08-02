#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/v2/synapsedriver.h"
#include "stadls/visitors.h"


using namespace haldls::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;

TEST(SynapseDriverConfig, General)
{
	SynapseDriverBlock config;

	EXPECT_ANY_THROW(SynapseDriverBlock::PulseLength(32));
	EXPECT_NO_THROW(SynapseDriverBlock::PulseLength(31));

	config.set_state(SynapseDriverOnDLS(5), SynapseDriverBlock::State::excitatory);
	ASSERT_EQ(
		config.get_state(SynapseDriverOnDLS(5)), SynapseDriverBlock::State::excitatory);
	config.set_state(SynapseDriverOnDLS(8), SynapseDriverBlock::State::inhibitory);
	ASSERT_EQ(
		config.get_state(SynapseDriverOnDLS(8)), SynapseDriverBlock::State::inhibitory);

	config.set_pulse_length(SynapseDriverBlock::PulseLength(21));
	ASSERT_EQ(config.get_pulse_length(), SynapseDriverBlock::PulseLength(21));

	SynapseDriverBlock config_eq = config;
	SynapseDriverBlock config_ne(config);
	config_ne.set_state(SynapseDriverOnDLS(5), SynapseDriverBlock::State::inhibitory);

	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config_ne == config);

	ASSERT_NE(config, config_ne);
	ASSERT_FALSE(config_eq != config);
}

TEST(SynapseDriverConfig, EncodeDecode)
{
	SynapseDriverBlock config;
	SynapseDriverBlock::states_type states;
	for (size_t index = 0; index < states.size(); index++) {
		if (index % 2)
			states.at(index) = SynapseDriverBlock::State::excitatory;
		else
			states.at(index) = SynapseDriverBlock::State::inhibitory;
	}

	config.set_pulse_length(SynapseDriverBlock::PulseLength(5));
	config.set_states(states);

	std::array<hardware_address_type, SynapseDriverBlock::config_size_in_words> ref_addresses = {
		{0x1c000000, 0x1c000001, 0x1c000002}};
	std::array<hardware_word_type, SynapseDriverBlock::config_size_in_words> ref_data = {
		{0x55555555, 0xAAAAAAAA, 5}};

	halco::hicann_dls::v2::SynapseDriverBlockOnDLS coord;

	{ // write addresses
		std::vector<hardware_address_type> write_addresses;
		visit_preorder(
			config, coord,
			stadls::WriteAddressVisitor<std::vector<hardware_address_type> >{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		std::vector<hardware_address_type> read_addresses;
		visit_preorder(
			config, coord,
			stadls::ReadAddressVisitor<std::vector<hardware_address_type> >{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	std::vector<hardware_word_type> data;
	visit_preorder(
		config, coord, stadls::EncodeVisitor<std::vector<hardware_word_type> >{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	SynapseDriverBlock config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(
		config_copy, coord,
		stadls::DecodeVisitor<std::vector<hardware_word_type> >{std::move(data)});
	ASSERT_EQ(config, config_copy);
}
