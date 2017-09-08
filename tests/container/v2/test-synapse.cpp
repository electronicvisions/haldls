#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/container/v2/synapse.h"
#include "haldls/io/visitors.h"


using namespace haldls::container::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;

TEST(CommonSynramConfig, General)
{
	CommonSynramConfig config;
	EXPECT_ANY_THROW(CommonSynramConfig::PCConf(16));
	EXPECT_NO_THROW(CommonSynramConfig::PCConf(15));
	EXPECT_NO_THROW(CommonSynramConfig::WConf(255));
	EXPECT_ANY_THROW(CommonSynramConfig::WaitCtrClear(8));
	EXPECT_NO_THROW(CommonSynramConfig::WaitCtrClear(7));

	config.set_pc_conf(CommonSynramConfig::PCConf(15));
	ASSERT_EQ(config.get_pc_conf(), CommonSynramConfig::PCConf(15));

	config.set_w_conf(CommonSynramConfig::WConf(15));
	ASSERT_EQ(config.get_w_conf(), CommonSynramConfig::WConf(15));

	config.set_wait_ctr_clear(CommonSynramConfig::WaitCtrClear(7));
	ASSERT_EQ(config.get_wait_ctr_clear(), CommonSynramConfig::WaitCtrClear(7));

	CommonSynramConfig config_eq = config;
	CommonSynramConfig config_ne(config);
	config_ne.set_w_conf(CommonSynramConfig::WConf(11));

	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_ne);

	ASSERT_NE(config, config_ne);
	ASSERT_FALSE(config != config_eq);
}

TEST(CommonSynramConfig, EncodeDecode)
{
	CommonSynramConfig config;
	config.set_pc_conf(CommonSynramConfig::PCConf(5));
	config.set_w_conf(CommonSynramConfig::WConf(50));
	config.set_wait_ctr_clear(CommonSynramConfig::WaitCtrClear(2));

	std::array<hardware_address_type, CommonSynramConfig::config_size_in_words> ref_addresses = {
		{0x08000000, 0x08000001, 0x08000002}};
	std::array<hardware_word_type, CommonSynramConfig::config_size_in_words> ref_data = {
		{5, 50, 2}};

	halco::common::Unique coord;

	{ // write addresses
		std::vector<hardware_address_type> write_addresses;
		visit_preorder(
			config, coord,
			haldls::io::WriteAddressVisitor<std::vector<hardware_address_type> >{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		std::vector<hardware_address_type> read_addresses;
		visit_preorder(
			config, coord,
			haldls::io::ReadAddressVisitor<std::vector<hardware_address_type> >{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	std::vector<hardware_word_type> data;
	visit_preorder(
		config, coord, haldls::io::EncodeVisitor<std::vector<hardware_word_type> >{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	CommonSynramConfig config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(
		config_copy, coord,
		haldls::io::DecodeVisitor<std::vector<hardware_word_type> >{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(SynapseBlock_Synapse, General)
{
	SynapseBlock::Synapse synapse;

	EXPECT_ANY_THROW(SynapseBlock::Synapse::Weight(64));
	EXPECT_NO_THROW(SynapseBlock::Synapse::Weight(63));
	EXPECT_ANY_THROW(SynapseBlock::Synapse::Address(64));
	EXPECT_NO_THROW(SynapseBlock::Synapse::Address(63));

	synapse.set_weight(SynapseBlock::Synapse::Weight(63));
	ASSERT_EQ(synapse.get_weight(), SynapseBlock::Synapse::Weight(63));

	synapse.set_address(SynapseBlock::Synapse::Address(63));
	ASSERT_EQ(synapse.get_address(), SynapseBlock::Synapse::Address(63));

	synapse.set_time_calib(SynapseBlock::Synapse::TimeCalib(0x2));
	ASSERT_EQ(synapse.get_time_calib(), SynapseBlock::Synapse::TimeCalib(0x2));

	synapse.set_amp_calib(SynapseBlock::Synapse::AmpCalib(0x2));
	ASSERT_EQ(synapse.get_amp_calib(), SynapseBlock::Synapse::AmpCalib(0x2));

	SynapseBlock::Synapse synapse_eq = synapse;
	SynapseBlock::Synapse synapse_ne(synapse);
	synapse_ne.set_address(SynapseBlock::Synapse::Address(58));

	ASSERT_EQ(synapse, synapse_eq);
	ASSERT_EQ(synapse == synapse_ne, false);

	ASSERT_NE(synapse, synapse_ne);
	ASSERT_EQ(synapse != synapse_eq, false);
}

TEST(SynapseBlock, General)
{
	SynapseBlock block;
	SynapseBlock::Synapse synapse;
	synapse.set_weight(SynapseBlock::Synapse::Weight(1));
	synapse.set_address(SynapseBlock::Synapse::Address(1));

	block.set_synapse(SynapseOnSynapseBlock(3), synapse);
	ASSERT_EQ(block.get_synapse(SynapseOnSynapseBlock(3)), synapse);

	SynapseBlock block_eq = block;
	SynapseBlock block_ne(block);
	block_ne.set_synapse(SynapseOnSynapseBlock(2), synapse);

	ASSERT_EQ(block, block_eq);
	ASSERT_FALSE(block == block_ne);

	ASSERT_NE(block, block_ne);
	ASSERT_FALSE(block != block_eq);
}

TEST(SynapseBlock, EncodeDecode)
{
	SynapseOnSynapseBlock synapse_coord(2);
	SynapseBlockOnDLS block_coord(X(3), Y(1));
	SynapseBlock synapse_block;
	SynapseBlock::Synapse synapse;
	synapse.set_weight(SynapseBlock::Synapse::Weight(61));
	synapse.set_address(SynapseBlock::Synapse::Address(63));
	synapse.set_time_calib(SynapseBlock::Synapse::TimeCalib(0x2));
	synapse.set_amp_calib(SynapseBlock::Synapse::AmpCalib(0x1));
	synapse_block.set_synapse(synapse_coord, synapse);

	std::array<hardware_address_type, SynapseBlock::config_size_in_words> ref_addresses = {
		{0x0001F013, 0x0001F01B}};
	std::array<hardware_word_type, SynapseBlock::config_size_in_words> ref_data = {
		{0x0000B700, 0x00007F00}};

	{ // write addresses
		std::vector<hardware_address_type> write_addresses;
		visit_preorder(
			synapse_block, block_coord,
			haldls::io::WriteAddressVisitor<std::vector<hardware_address_type> >{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		std::vector<hardware_address_type> read_addresses;
		visit_preorder(
			synapse_block, block_coord,
			haldls::io::ReadAddressVisitor<std::vector<hardware_address_type> >{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	std::vector<hardware_word_type> data;
	visit_preorder(
		synapse_block, block_coord,
		haldls::io::EncodeVisitor<std::vector<hardware_word_type> >{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	SynapseBlock block_copy;
	ASSERT_NE(synapse_block, block_copy);
	visit_preorder(
		block_copy, block_coord,
		haldls::io::DecodeVisitor<std::vector<hardware_word_type> >{std::move(data)});
	ASSERT_EQ(synapse_block, block_copy);
}

TEST(ColumnCorrelationBlock_ColumnCorrelationSwitch, General)
{
	ColumnCorrelationBlock::ColumnCorrelationSwitch corrswitch;

	corrswitch.set_causal_config(ColumnCorrelationBlock::ColumnCorrelationSwitch::Config::external);
	ASSERT_EQ(
		corrswitch.get_causal_config(),
		ColumnCorrelationBlock::ColumnCorrelationSwitch::Config::external);

	corrswitch.set_acausal_config(
		ColumnCorrelationBlock::ColumnCorrelationSwitch::Config::internal);
	ASSERT_EQ(
		corrswitch.get_acausal_config(),
		ColumnCorrelationBlock::ColumnCorrelationSwitch::Config::internal);

	ColumnCorrelationBlock::ColumnCorrelationSwitch corrswitch_eq = corrswitch;
	ColumnCorrelationBlock::ColumnCorrelationSwitch corrswitch_ne(corrswitch);
	corrswitch_ne.set_causal_config(
		ColumnCorrelationBlock::ColumnCorrelationSwitch::Config::internal);

	ASSERT_EQ(corrswitch, corrswitch_eq);
	ASSERT_EQ(corrswitch_ne == corrswitch, false);

	ASSERT_NE(corrswitch, corrswitch_ne);
	ASSERT_EQ(corrswitch_eq != corrswitch, false);
}

TEST(ColumnCorrelationBlock, General)
{
	ColumnCorrelationBlock block;
	ColumnCorrelationBlock::ColumnCorrelationSwitch corr_switch;
	corr_switch.set_causal_config(
		ColumnCorrelationBlock::ColumnCorrelationSwitch::Config::internal);

	block.set_switch(ColumnCorrelationSwitchOnColumnBlock(3), corr_switch);
	ASSERT_EQ(block.get_switch(ColumnCorrelationSwitchOnColumnBlock(3)), corr_switch);

	ColumnCorrelationBlock block_eq = block;
	ColumnCorrelationBlock block_ne;
	block_ne.set_switch(ColumnCorrelationSwitchOnColumnBlock(2), corr_switch);

	ASSERT_EQ(block, block_eq);
	ASSERT_FALSE(block == block_ne);

	ASSERT_NE(block, block_ne);
	ASSERT_FALSE(block != block_eq);
}

TEST(ColumnCorrelationBlock, EncodeDecode)
{
	ColumnCorrelationSwitchOnColumnBlock switch_coord_1(2);
	ColumnCorrelationSwitchOnColumnBlock switch_coord_2(3);
	ColumnBlockOnDLS block_coord(1);
	ColumnCorrelationBlock column_block;
	ColumnCorrelationBlock::ColumnCorrelationSwitch col_switch_1;
	ColumnCorrelationBlock::ColumnCorrelationSwitch col_switch_2;
	col_switch_1.set_causal_config(
		ColumnCorrelationBlock::ColumnCorrelationSwitch::Config::external);
	col_switch_1.set_acausal_config(
		ColumnCorrelationBlock::ColumnCorrelationSwitch::Config::internal);
	col_switch_2.set_causal_config(
		ColumnCorrelationBlock::ColumnCorrelationSwitch::Config::readout);
	column_block.set_switch(switch_coord_1, col_switch_1);
	column_block.set_switch(switch_coord_2, col_switch_2);

	std::array<hardware_address_type, ColumnCurrentBlock::config_size_in_words> ref_addresses = {
		{0x0001F211, 0x0001F219}};
	std::array<hardware_word_type, ColumnCorrelationBlock::config_size_in_words> ref_data = {
		{0x00008040, 0x00004040}};

	{ // write addresses
		std::vector<hardware_address_type> write_addresses;
		visit_preorder(
			column_block, block_coord,
			haldls::io::WriteAddressVisitor<std::vector<hardware_address_type> >{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		std::vector<hardware_address_type> read_addresses;
		visit_preorder(
			column_block, block_coord,
			haldls::io::ReadAddressVisitor<std::vector<hardware_address_type> >{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	std::vector<hardware_word_type> data;
	visit_preorder(
		column_block, block_coord,
		haldls::io::EncodeVisitor<std::vector<hardware_word_type> >{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	ColumnCorrelationBlock block_copy;
	ASSERT_NE(column_block, block_copy);
	visit_preorder(
		block_copy, block_coord,
		haldls::io::DecodeVisitor<std::vector<hardware_word_type> >{std::move(data)});
	ASSERT_EQ(column_block, block_copy);
}

TEST(ColumnCurrentBlock_ColumnCurrentSwitch, General)
{
	ColumnCurrentBlock::ColumnCurrentSwitch currswitch;

	currswitch.set_exc_config(ColumnCurrentBlock::ColumnCurrentSwitch::Config::external);
	ASSERT_EQ(
		currswitch.get_exc_config(), ColumnCurrentBlock::ColumnCurrentSwitch::Config::external);

	currswitch.set_inh_config(ColumnCurrentBlock::ColumnCurrentSwitch::Config::internal);
	ASSERT_EQ(
		currswitch.get_inh_config(), ColumnCurrentBlock::ColumnCurrentSwitch::Config::internal);

	ColumnCurrentBlock::ColumnCurrentSwitch currswitch_eq = currswitch;
	ColumnCurrentBlock::ColumnCurrentSwitch currswitch_ne(currswitch);
	currswitch_ne.set_exc_config(ColumnCurrentBlock::ColumnCurrentSwitch::Config::internal);

	ASSERT_EQ(currswitch, currswitch_eq);
	ASSERT_EQ(currswitch_ne == currswitch, false);

	ASSERT_NE(currswitch, currswitch_ne);
	ASSERT_EQ(currswitch_eq != currswitch, false);
}

TEST(ColumnCurrentBlock, General)
{
	ColumnCurrentBlock block;
	ColumnCurrentBlock::ColumnCurrentSwitch curr_switch;
	curr_switch.set_exc_config(ColumnCurrentBlock::ColumnCurrentSwitch::Config::internal);

	block.set_switch(ColumnCurrentSwitchOnColumnBlock(3), curr_switch);
	ASSERT_EQ(block.get_switch(ColumnCurrentSwitchOnColumnBlock(3)), curr_switch);

	ColumnCurrentBlock block_eq = block;
	ColumnCurrentBlock block_ne;
	block_ne.set_switch(ColumnCurrentSwitchOnColumnBlock(2), curr_switch);

	ASSERT_EQ(block, block_eq);
	ASSERT_FALSE(block == block_ne);

	ASSERT_NE(block, block_ne);
	ASSERT_FALSE(block != block_eq);
}

TEST(ColumnCurrentBlock, EncodeDecode)
{
	ColumnCurrentSwitchOnColumnBlock switch_coord_1(2);
	ColumnCurrentSwitchOnColumnBlock switch_coord_2(3);
	ColumnBlockOnDLS block_coord(1);
	ColumnCurrentBlock column_block;
	ColumnCurrentBlock::ColumnCurrentSwitch cur_switch_1;
	ColumnCurrentBlock::ColumnCurrentSwitch cur_switch_2;
	cur_switch_1.set_exc_config(ColumnCurrentBlock::ColumnCurrentSwitch::Config::external);
	cur_switch_1.set_inh_config(ColumnCurrentBlock::ColumnCurrentSwitch::Config::internal);
	cur_switch_2.set_inh_config(ColumnCurrentBlock::ColumnCurrentSwitch::Config::readout);
	column_block.set_switch(switch_coord_1, cur_switch_1);
	column_block.set_switch(switch_coord_2, cur_switch_2);

	std::array<hardware_address_type, ColumnCorrelationBlock::config_size_in_words> ref_addresses =
		{{0x0001F201, 0x0001F209}};
	std::array<hardware_word_type, ColumnCurrentBlock::config_size_in_words> ref_data = {
		{0x00008080, 0x00004080}};

	{ // write addresses
		std::vector<hardware_address_type> write_addresses;
		visit_preorder(
			column_block, block_coord,
			haldls::io::WriteAddressVisitor<std::vector<hardware_address_type> >{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		std::vector<hardware_address_type> read_addresses;
		visit_preorder(
			column_block, block_coord,
			haldls::io::ReadAddressVisitor<std::vector<hardware_address_type> >{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	std::vector<hardware_word_type> data;
	visit_preorder(
		column_block, block_coord,
		haldls::io::EncodeVisitor<std::vector<hardware_word_type> >{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	ColumnCurrentBlock block_copy;
	ASSERT_NE(column_block, block_copy);
	visit_preorder(
		block_copy, block_coord,
		haldls::io::DecodeVisitor<std::vector<hardware_word_type> >{std::move(data)});
	ASSERT_EQ(column_block, block_copy);
}

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
			haldls::io::WriteAddressVisitor<std::vector<hardware_address_type> >{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		std::vector<hardware_address_type> read_addresses;
		visit_preorder(
			config, coord,
			haldls::io::ReadAddressVisitor<std::vector<hardware_address_type> >{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	std::vector<hardware_word_type> data;
	visit_preorder(
		config, coord, haldls::io::EncodeVisitor<std::vector<hardware_word_type> >{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	SynapseDrivers config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(
		config_copy, coord,
		haldls::io::DecodeVisitor<std::vector<hardware_word_type> >{std::move(data)});
	ASSERT_EQ(config, config_copy);
}
