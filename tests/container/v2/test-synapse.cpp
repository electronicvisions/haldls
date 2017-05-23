#include <gtest/gtest.h>

#include "haldls/container/v2/synapse.h"

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
