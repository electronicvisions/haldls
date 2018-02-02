#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/container/v2/synapsedriver.h"
#include "stadls/visitors.h"


using namespace haldls::container::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;

TEST(SynapseDriverConfig, General)
{
	SynapseDrivers config;

	EXPECT_ANY_THROW(SynapseDrivers::PulseLength(32));
	EXPECT_NO_THROW(SynapseDrivers::PulseLength(31));

	config.set_state(SynapseDriverOnDLS(5), SynapseDrivers::State::excitatory);
	ASSERT_EQ(
		config.get_state(SynapseDriverOnDLS(5)), SynapseDrivers::State::excitatory);
	config.set_state(SynapseDriverOnDLS(8), SynapseDrivers::State::inhibitory);
	ASSERT_EQ(
		config.get_state(SynapseDriverOnDLS(8)), SynapseDrivers::State::inhibitory);

	config.set_pulse_length(SynapseDrivers::PulseLength(21));
	ASSERT_EQ(config.get_pulse_length(), SynapseDrivers::PulseLength(21));

	SynapseDrivers config_eq = config;
	SynapseDrivers config_ne(config);
	config_ne.set_state(SynapseDriverOnDLS(5), SynapseDrivers::State::inhibitory);

	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config_ne == config);

	ASSERT_NE(config, config_ne);
	ASSERT_FALSE(config_eq != config);
}

TEST(SynapseDriverConfig, EncodeDecode)
{
	SynapseDrivers config;
	SynapseDrivers::states_type states;
	for (size_t index = 0; index < states.size(); index++) {
		if (index % 2)
			states.at(index) = SynapseDrivers::State::excitatory;
		else
			states.at(index) = SynapseDrivers::State::inhibitory;
	}

	config.set_pulse_length(SynapseDrivers::PulseLength(5));
	config.set_states(states);

	std::array<hardware_address_type, SynapseDrivers::config_size_in_words> ref_addresses = {
		{0x1c000000, 0x1c000001, 0x1c000002}};
	std::array<hardware_word_type, SynapseDrivers::config_size_in_words> ref_data = {
		{0x55555555, 0xAAAAAAAA, 5}};

	halco::common::Unique coord;

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

	SynapseDrivers config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(
		config_copy, coord,
		stadls::DecodeVisitor<std::vector<hardware_word_type> >{std::move(data)});
	ASSERT_EQ(config, config_copy);
}
