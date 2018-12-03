#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "test-helper.h"
#include "haldls/v2/synapse.h"
#include "stadls/visitors.h"


using namespace haldls::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;

TEST(CommonSynramConfig, General)
{
	CommonSynramConfig config;
	EXPECT_ANY_THROW(CommonSynramConfig::PCConf(16));
	EXPECT_NO_THROW(CommonSynramConfig::PCConf(15));
	EXPECT_ANY_THROW(CommonSynramConfig::WConf(256));
	EXPECT_NO_THROW(CommonSynramConfig::WConf(255));
	EXPECT_ANY_THROW(CommonSynramConfig::WaitCtrClear(8));
	EXPECT_NO_THROW(CommonSynramConfig::WaitCtrClear(7));

	config.set_pc_conf(CommonSynramConfig::PCConf(15));
	ASSERT_EQ(config.get_pc_conf(), CommonSynramConfig::PCConf(15));

	config.set_w_conf(CommonSynramConfig::WConf(15));
	ASSERT_EQ(config.get_w_conf(), CommonSynramConfig::WConf(15));

	config.set_wait_ctr_clear(CommonSynramConfig::WaitCtrClear(7));
	ASSERT_EQ(config.get_wait_ctr_clear(), CommonSynramConfig::WaitCtrClear(7));

	config.set_use_internal_i_bias_correlation_output(true);
	ASSERT_EQ(config.get_use_internal_i_bias_correlation_output(), true);

	config.set_use_internal_i_bias_vstore(true);
	ASSERT_EQ(config.get_use_internal_i_bias_vstore(), true);

	config.set_use_internal_i_bias_vramp(true);
	ASSERT_EQ(config.get_use_internal_i_bias_vramp(), true);

	config.set_use_internal_i_bias_vdac(true);
	ASSERT_EQ(config.get_use_internal_i_bias_vdac(), true);

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
	config.set_use_internal_i_bias_correlation_output(true);
	config.set_use_internal_i_bias_vstore(false);
	config.set_use_internal_i_bias_vramp(true);
	config.set_use_internal_i_bias_vdac(false);

	std::array<hardware_address_type, CommonSynramConfig::config_size_in_words> ref_addresses = {
	    {0x08000000, 0x08000001, 0x08000002, 0x08000003}};
	std::array<hardware_word_type, CommonSynramConfig::config_size_in_words> ref_data = {
	    {5, 50, 2, 0b0101}};

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

	CommonSynramConfig config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(
		config_copy, coord,
		stadls::DecodeVisitor<std::vector<hardware_word_type> >{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(CommonSynramConfig, CerealizeCoverage)
{
	CommonSynramConfig obj1,obj2;
	obj1.set_use_internal_i_bias_correlation_output(!obj1.get_use_internal_i_bias_correlation_output());
	obj1.set_use_internal_i_bias_vstore(!obj1.get_use_internal_i_bias_vstore());
	obj1.set_use_internal_i_bias_vramp(!obj1.get_use_internal_i_bias_vramp());
	obj1.set_use_internal_i_bias_vdac(!obj1.get_use_internal_i_bias_vdac());
	obj1.set_pc_conf(draw_ranged_non_default_value<CommonSynramConfig::PCConf>(obj1.get_pc_conf()));
	obj1.set_w_conf(draw_ranged_non_default_value<CommonSynramConfig::WConf>(obj1.get_w_conf()));
	obj1.set_wait_ctr_clear(draw_ranged_non_default_value<CommonSynramConfig::WaitCtrClear>(obj1.get_wait_ctr_clear()));

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

TEST(SynapseBlock_Synapse, CerealizeCoverage)
{
	SynapseBlock::Synapse obj1,obj2;
	obj1.set_weight(draw_ranged_non_default_value<SynapseBlock::Synapse::Weight>(obj1.get_weight()));
	obj1.set_address(draw_ranged_non_default_value<SynapseBlock::Synapse::Address>(obj1.get_address()));
	obj1.set_time_calib(draw_ranged_non_default_value<SynapseBlock::Synapse::TimeCalib>(obj1.get_time_calib()));
	obj1.set_amp_calib(draw_ranged_non_default_value<SynapseBlock::Synapse::AmpCalib>(obj1.get_amp_calib()));

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
			stadls::WriteAddressVisitor<std::vector<hardware_address_type> >{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		std::vector<hardware_address_type> read_addresses;
		visit_preorder(
			synapse_block, block_coord,
			stadls::ReadAddressVisitor<std::vector<hardware_address_type> >{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	std::vector<hardware_word_type> data;
	visit_preorder(
		synapse_block, block_coord,
		stadls::EncodeVisitor<std::vector<hardware_word_type> >{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	SynapseBlock block_copy;
	ASSERT_NE(synapse_block, block_copy);
	visit_preorder(
		block_copy, block_coord,
		stadls::DecodeVisitor<std::vector<hardware_word_type> >{std::move(data)});
	ASSERT_EQ(synapse_block, block_copy);
}

TEST(SynapseBlock, CerealizeCoverage)
{
	SynapseBlock obj1,obj2;
	for (auto syn: iter_all<halco::hicann_dls::v2::SynapseOnSynapseBlock>()) {
		SynapseBlock::Synapse synapse;
		synapse.set_weight(draw_ranged_non_default_value<SynapseBlock::Synapse::Weight>(synapse.get_weight()));
		synapse.set_address(draw_ranged_non_default_value<SynapseBlock::Synapse::Address>(synapse.get_address()));
		synapse.set_time_calib(draw_ranged_non_default_value<SynapseBlock::Synapse::TimeCalib>(synapse.get_time_calib()));
		synapse.set_amp_calib(draw_ranged_non_default_value<SynapseBlock::Synapse::AmpCalib>(synapse.get_amp_calib()));
		obj1.set_synapse(syn, synapse);
	}

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

TEST(ColumnCorrelationBlock_ColumnCorrelationSwitch, CerealizeCoverage)
{
	ColumnCorrelationBlock::ColumnCorrelationSwitch obj1,obj2;
	obj1.set_causal_config(ColumnCorrelationBlock::ColumnCorrelationSwitch::Config::readout);
	obj1.set_acausal_config(ColumnCorrelationBlock::ColumnCorrelationSwitch::Config::readout);

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
			stadls::WriteAddressVisitor<std::vector<hardware_address_type> >{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		std::vector<hardware_address_type> read_addresses;
		visit_preorder(
			column_block, block_coord,
			stadls::ReadAddressVisitor<std::vector<hardware_address_type> >{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	std::vector<hardware_word_type> data;
	visit_preorder(
		column_block, block_coord,
		stadls::EncodeVisitor<std::vector<hardware_word_type> >{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	ColumnCorrelationBlock block_copy;
	ASSERT_NE(column_block, block_copy);
	visit_preorder(
		block_copy, block_coord,
		stadls::DecodeVisitor<std::vector<hardware_word_type> >{std::move(data)});
	ASSERT_EQ(column_block, block_copy);
}

TEST(ColumnCorrelationBlock, CerealizeCoverage)
{
	ColumnCorrelationBlock obj1,obj2;
	for (auto sw: iter_all<ColumnCorrelationSwitchOnColumnBlock>()) {
		auto config = obj1.get_switch(sw);
		config.set_causal_config(ColumnCorrelationBlock::ColumnCorrelationSwitch::Config::readout);
		config.set_acausal_config(ColumnCorrelationBlock::ColumnCorrelationSwitch::Config::readout);

		obj1.set_switch(sw, config);
	}

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

TEST(ColumnCurrentBlock_ColumnCurrentSwitch, CerealizeCoverage)
{
	ColumnCurrentBlock::ColumnCurrentSwitch obj1,obj2;
	obj1.set_exc_config(ColumnCurrentBlock::ColumnCurrentSwitch::Config::readout);
	obj1.set_inh_config(ColumnCurrentBlock::ColumnCurrentSwitch::Config::readout);

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
			stadls::WriteAddressVisitor<std::vector<hardware_address_type> >{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		std::vector<hardware_address_type> read_addresses;
		visit_preorder(
			column_block, block_coord,
			stadls::ReadAddressVisitor<std::vector<hardware_address_type> >{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	std::vector<hardware_word_type> data;
	visit_preorder(
		column_block, block_coord,
		stadls::EncodeVisitor<std::vector<hardware_word_type> >{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	ColumnCurrentBlock block_copy;
	ASSERT_NE(column_block, block_copy);
	visit_preorder(
		block_copy, block_coord,
		stadls::DecodeVisitor<std::vector<hardware_word_type> >{std::move(data)});
	ASSERT_EQ(column_block, block_copy);
}

TEST(ColumnCurrentBlock, CerealizeCoverage)
{
	ColumnCurrentBlock obj1,obj2;
	for (auto sw: iter_all<ColumnCurrentSwitchOnColumnBlock>()) {
		auto config = obj1.get_switch(sw);
		config.set_exc_config(ColumnCurrentBlock::ColumnCurrentSwitch::Config::readout);
		config.set_inh_config(ColumnCurrentBlock::ColumnCurrentSwitch::Config::readout);
		obj1.set_switch(sw, config);
	}

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


