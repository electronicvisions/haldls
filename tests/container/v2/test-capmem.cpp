#include <gtest/gtest.h>

#include "haldls/container/v2/capmem.h"

using namespace haldls::container::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;

TEST(CapMemCell, General)
{
	CapMemCell capmemcell;

	EXPECT_ANY_THROW(CapMemCell::Value(1024));
	EXPECT_NO_THROW(CapMemCell::Value(1023));

	capmemcell.set_value(CapMemCell::Value(145));
	ASSERT_EQ(capmemcell.get_value(), CapMemCell::Value(145));

	CapMemCell capmemcell_eq = capmemcell;
	CapMemCell capmemcell_ne(CapMemCell::Value(139));

	ASSERT_EQ(capmemcell, capmemcell_eq);
	ASSERT_FALSE(capmemcell == capmemcell_ne);

	ASSERT_NE(capmemcell, capmemcell_ne);
	ASSERT_FALSE(capmemcell != capmemcell_eq);
}

TEST(CapMem, General)
{
	CapMem capmem;
	// test default constructor
	// neuron capmem
	ASSERT_EQ(
		capmem.get(
			CapMemCellOnDLS(CapMemColumnOnDLS(Enum(0)), CapMemRowOnDLS(Enum(1)))),
		CapMemCell(CapMemCell::Value(600)));
	ASSERT_EQ(
		capmem.get(
			CapMemCellOnDLS(CapMemColumnOnDLS(Enum(4)), CapMemRowOnDLS(Enum(10)))),
		CapMemCell(CapMemCell::Value(130)));
	// global capmem
	ASSERT_EQ(
		capmem.get(
			CapMemCellOnDLS(CapMemColumnOnDLS(Enum(32)), CapMemRowOnDLS(Enum(0)))),
		CapMemCell(CapMemCell::Value(100)));
	ASSERT_EQ(
		capmem.get(
			CapMemCellOnDLS(CapMemColumnOnDLS(Enum(32)), CapMemRowOnDLS(Enum(1)))),
		CapMemCell(CapMemCell::Value(0)));

	// test getter/setter
	capmem.set(CapMemCellOnDLS(Enum(2)), CapMemCell(CapMemCell::Value(5)));
	ASSERT_EQ(capmem.get(CapMemCellOnDLS(Enum(2))), CapMemCell(CapMemCell::Value(5)));

	capmem.set(
		NeuronOnDLS(Enum(0)), NeuronParameter::v_leak, CapMemCell(CapMemCell::Value(123)));
	ASSERT_EQ(
		capmem.get(NeuronOnDLS(Enum(0)), NeuronParameter::v_leak),
		CapMemCell(CapMemCell::Value(123)));

	capmem.set(
		CommonNeuronParameter::e_reset, CapMemCell(CapMemCell::Value(234)));
	ASSERT_EQ(
		capmem.get(CommonNeuronParameter::e_reset),
		CapMemCell(CapMemCell::Value(234)));

	CapMem capmem_eq = capmem;
	CapMem capmem_ne(capmem);
	capmem_ne.set(CapMemCellOnDLS(Enum(67)), CapMemCell(CapMemCell::Value(123)));

	ASSERT_EQ(capmem, capmem_eq);
	ASSERT_FALSE(capmem == capmem_ne);

	ASSERT_NE(capmem, capmem_ne);
	ASSERT_FALSE(capmem != capmem_eq);
}

TEST(CommonCapMemConfig, General)
{
	CommonCapMemConfig config;

	EXPECT_ANY_THROW(CommonCapMemConfig::OutAmpBias(16));
	EXPECT_NO_THROW(CommonCapMemConfig::OutAmpBias(15));
	EXPECT_ANY_THROW(CommonCapMemConfig::SourceFollowerBias(16));
	EXPECT_NO_THROW(CommonCapMemConfig::SourceFollowerBias(15));
	EXPECT_ANY_THROW(CommonCapMemConfig::LevelShifterBias(16));
	EXPECT_NO_THROW(CommonCapMemConfig::LevelShifterBias(15));
	EXPECT_ANY_THROW(CommonCapMemConfig::VGlobalBias(16));
	EXPECT_NO_THROW(CommonCapMemConfig::VGlobalBias(15));
	EXPECT_ANY_THROW(CommonCapMemConfig::CurrentCellRes(64));
	EXPECT_NO_THROW(CommonCapMemConfig::CurrentCellRes(63));
	EXPECT_ANY_THROW(CommonCapMemConfig::BoostFactor(16));
	EXPECT_NO_THROW(CommonCapMemConfig::BoostFactor(15));
	EXPECT_ANY_THROW(CommonCapMemConfig::PrescalePause(7));
	EXPECT_NO_THROW(CommonCapMemConfig::PrescalePause(6));
	EXPECT_ANY_THROW(CommonCapMemConfig::PrescaleRamp(7));
	EXPECT_NO_THROW(CommonCapMemConfig::PrescaleRamp(6));
	EXPECT_ANY_THROW(CommonCapMemConfig::SubCounter(65536));
	EXPECT_NO_THROW(CommonCapMemConfig::SubCounter(65535));
	EXPECT_ANY_THROW(CommonCapMemConfig::PulseA(65536));
	EXPECT_NO_THROW(CommonCapMemConfig::PulseA(65535));
	EXPECT_ANY_THROW(CommonCapMemConfig::PulseB(65536));
	EXPECT_NO_THROW(CommonCapMemConfig::PulseB(65535));
	EXPECT_ANY_THROW(CommonCapMemConfig::BoostA(65536));
	EXPECT_NO_THROW(CommonCapMemConfig::BoostA(65535));
	EXPECT_ANY_THROW(CommonCapMemConfig::BoostB(65536));
	EXPECT_NO_THROW(CommonCapMemConfig::BoostB(65535));

	// test default constructor
	ASSERT_EQ(config.get_pause_counter(), CommonCapMemConfig::PauseCounter(8096));
	ASSERT_EQ(config.get_debug_out_amp_bias(), CommonCapMemConfig::OutAmpBias(7));

	// test getter/setter
	config.set_enable_capmem(true);
	ASSERT_EQ(config.get_enable_capmem(), true);

	config.set_debug_readout_enable(true);
	ASSERT_EQ(config.get_debug_readout_enable(), true);

	config.set_debug_capmem_coord(CapMemCellOnDLS(Enum(10)));
	ASSERT_EQ(config.get_debug_capmem_coord(), CapMemCellOnDLS(Enum(10)));

	config.set_debug_v_ref_select(CommonCapMemConfig::VRefSelect::v_ref_i);
	ASSERT_EQ(config.get_debug_v_ref_select(), CommonCapMemConfig::VRefSelect::v_ref_i);

	config.set_debug_i_out_select(CommonCapMemConfig::IOutSelect::i_out_ramp);
	ASSERT_EQ(config.get_debug_i_out_select(), CommonCapMemConfig::IOutSelect::i_out_ramp);

	config.set_debug_out_amp_bias(CommonCapMemConfig::OutAmpBias(8));
	ASSERT_EQ(config.get_debug_out_amp_bias(), CommonCapMemConfig::OutAmpBias(8));

	config.set_debug_source_follower_bias(CommonCapMemConfig::SourceFollowerBias(9));
	ASSERT_EQ(config.get_debug_source_follower_bias(), CommonCapMemConfig::SourceFollowerBias(9));

	config.set_debug_level_shifter_bias(CommonCapMemConfig::LevelShifterBias(10));
	ASSERT_EQ(config.get_debug_level_shifter_bias(), CommonCapMemConfig::LevelShifterBias(10));

	config.set_v_global_bias(CommonCapMemConfig::VGlobalBias(9));
	ASSERT_EQ(config.get_v_global_bias(), CommonCapMemConfig::VGlobalBias(9));

	config.set_current_cell_res(CommonCapMemConfig::CurrentCellRes(23));
	ASSERT_EQ(config.get_current_cell_res(), CommonCapMemConfig::CurrentCellRes(23));

	config.set_boost_factor(CommonCapMemConfig::BoostFactor(6));
	ASSERT_EQ(config.get_boost_factor(), CommonCapMemConfig::BoostFactor(6));

	config.set_enable_boost(true);
	ASSERT_EQ(config.get_enable_boost(), true);

	config.set_enable_autoboost(true);
	ASSERT_EQ(config.get_enable_autoboost(), true);

	config.set_prescale_pause(CommonCapMemConfig::PrescalePause(6));
	ASSERT_EQ(config.get_prescale_pause(), CommonCapMemConfig::PrescalePause(6));

	config.set_prescale_ramp(CommonCapMemConfig::PrescaleRamp(2));
	ASSERT_EQ(config.get_prescale_ramp(), CommonCapMemConfig::PrescaleRamp(2));

	config.set_sub_counter(CommonCapMemConfig::SubCounter(16));
	ASSERT_EQ(config.get_sub_counter(), CommonCapMemConfig::SubCounter(16));

	config.set_pause_counter(CommonCapMemConfig::PauseCounter(18));
	ASSERT_EQ(config.get_pause_counter(), CommonCapMemConfig::PauseCounter(18));

	config.set_pulse_a(CommonCapMemConfig::PulseA(15));
	ASSERT_EQ(config.get_pulse_a(), CommonCapMemConfig::PulseA(15));

	config.set_pulse_b(CommonCapMemConfig::PulseB(123));
	ASSERT_EQ(config.get_pulse_b(), CommonCapMemConfig::PulseB(123));

	config.set_boost_a(CommonCapMemConfig::BoostA(13));
	ASSERT_EQ(config.get_boost_a(), CommonCapMemConfig::BoostA(13));

	config.set_boost_b(CommonCapMemConfig::BoostB(134));
	ASSERT_EQ(config.get_boost_b(), CommonCapMemConfig::BoostB(134));


	CommonCapMemConfig config_eq = config;
	CommonCapMemConfig config_ne(config);
	config_ne.set_boost_b(CommonCapMemConfig::BoostB(118));
	config_ne.set_enable_autoboost(false);

	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_ne);

	ASSERT_NE(config, config_ne);
	ASSERT_FALSE(config != config_eq);
}
