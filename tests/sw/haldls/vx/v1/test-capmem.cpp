#include <cmath>
#include <string>
#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "fisch/vx/jtag.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "halco/hicann-dls/vx/v1/capmem.h"
#include "haldls/vx/v1/capmem.h"
#include "haldls/vx/v1/omnibus_constants.h"
#include "test-helper.h"

using namespace haldls::vx::v1;
using namespace halco::hicann_dls::vx::v1;
using namespace halco::common;
using namespace hate::math;

TEST(CapMemCell, General)
{
	typedef typename CapMemCell::Value value_type;
	// test range
	EXPECT_NO_THROW(value_type(CapMemCell::Value::max));
	EXPECT_NO_THROW(value_type(CapMemCell::Value::min));

	// test getter/setter
	CapMemCell o;
	auto val = draw_ranged_non_default_value<value_type>();
	o.set_value(val);
	ASSERT_EQ(std::get<CapMemCell::Value>(o.get_value()), val);

	o.set_value(CapMemCell::DisableRefresh());
	ASSERT_EQ(std::get<CapMemCell::DisableRefresh>(o.get_value()), CapMemCell::DisableRefresh());

	// test assign
	CapMemCell o_copy = o;
	auto other_val = draw_ranged_non_default_value<value_type>(val);
	CapMemCell o_other(other_val);

	// test comparison
	ASSERT_EQ(o_copy, o);
	ASSERT_FALSE(o_copy != o);
	ASSERT_FALSE(o_copy == o_other);
	ASSERT_TRUE(o_copy != o_other);
	ASSERT_NE(o, o_other);

	test_ostream_operator_single_value<CapMemCell>();
}

TEST(CapMemCell, CerealizeCoverage)
{
	test_non_default_cerealization_single_value<CapMemCell>();
}

TEST(CapMemCell, EncodeDecode)
{
	CapMemCell cell;
	typename CapMemCell::Value val = draw_ranged_non_default_value<typename CapMemCell::Value>();

	cell.set_value(val);

	CapMemCellOnDLS coord(Enum(0));

	std::array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, CapMemCell::config_size_in_words>
	    ref_addresses = {
	        halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{capmem_nw_sram_base_address}};
	std::array<fisch::vx::OmnibusChipOverJTAG, CapMemCell::config_size_in_words> ref_data = {
	    fisch::vx::OmnibusChipOverJTAG(static_cast<fisch::vx::OmnibusChipOverJTAG::Value>(val))};

	HALDLS_TEST_ENCODE_DECODE(cell, coord, ref_addresses, ref_data)
}

TEST(CapMemBlock, General)
{
	CapMemBlock block;
	size_t i = 0;
	for (auto cell : halco::common::iter_all<CapMemCellOnCapMemBlock>()) {
		block.set_cell(cell, CapMemCell::Value(i));
		ASSERT_EQ(std::get<CapMemCell::Value>(block.get_cell(cell)), CapMemCell::Value(i));
		i = (i + 1) % CapMemCell::Value::max;
	}

	// test copy
	CapMemBlock copy = block;
	ASSERT_TRUE(copy == block);
	ASSERT_EQ(copy, block);
	ASSERT_FALSE(copy != block);
	copy.set_cell(CapMemCellOnCapMemBlock(), CapMemCell::Value(0x23));
	ASSERT_NE(copy, block);
	ASSERT_FALSE(copy == block);
	ASSERT_TRUE(copy != block);

	// test ostream
	std::stringstream out;
	out << copy;
	ASSERT_TRUE(out.str().find("CapMemCell(Value(35))") != std::string::npos);
}

TEST(CapMemBlock, EncodeDecode)
{
	CapMemBlock block;
	CapMemBlockOnDLS block_coord(0);
	std::array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, CapMemCellOnCapMemBlock::size>
	    ref_addresses;
	std::array<fisch::vx::OmnibusChipOverJTAG, CapMemCellOnCapMemBlock::size> ref_data;

	// Fill block with random data
	for (auto column : halco::common::iter_all<CapMemColumnOnCapMemBlock>()) {
		for (auto row : halco::common::iter_all<CapMemRowOnCapMemBlock>()) {
			auto const val = draw_ranged_non_default_value<typename CapMemCell::Value>();
			block.set_cell(CapMemCellOnDLS(CapMemCellOnCapMemBlock(column, row), block_coord), val);
			ref_data[CapMemCellOnCapMemBlock(column, row).toEnum()] =
			    fisch::vx::OmnibusChipOverJTAG(
			        static_cast<fisch::vx::OmnibusChipOverJTAG::Value>(val));
		}
	}

	// Generate expected addresses
	for (auto column : iter_all<CapMemColumnOnCapMemBlock>()) {
		for (auto row : iter_all<CapMemRowOnCapMemBlock>()) {
			ref_addresses[CapMemCellOnCapMemBlock(column, row).toEnum()] =
			    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{capmem_nw_sram_base_address +
			                                                      row + column * 32};
		}
	}

	HALDLS_TEST_ENCODE_DECODE(block, block_coord, ref_addresses, ref_data)
}

TEST(CapMemBlock, CerealizeCoverage)
{
	CapMemBlock b1, b2;
	for (auto cell : iter_all<CapMemCellOnCapMemBlock>()) {
		b1.set_cell(cell, draw_ranged_non_default_value<typename CapMemCell::Value>());
	}
	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(b1);
	}
	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(b2);
	}
	ASSERT_EQ(b1, b2);
}

template <>
struct random_value<CapMemBlockConfig::VRefSelect>
{
	CapMemBlockConfig::VRefSelect operator()(CapMemBlockConfig::VRefSelect const& exclude)
	{
		size_t rnd;
		do {
			rnd = rand() % 3;
		} while (rnd == static_cast<size_t>(exclude));
		return static_cast<CapMemBlockConfig::VRefSelect>(rnd);
	};
};

template <>
struct random_value<CapMemBlockConfig::IOutSelect>
{
	CapMemBlockConfig::IOutSelect operator()(CapMemBlockConfig::IOutSelect const& exclude)
	{
		size_t rnd;
		do {
			rnd = rand() % 3;
		} while (rnd == static_cast<size_t>(exclude));
		return static_cast<CapMemBlockConfig::IOutSelect>(rnd);
	};
};

template <>
struct random_value<CapMemCellOnCapMemBlock>
{
	CapMemCellOnCapMemBlock operator()(CapMemCellOnCapMemBlock const& exclude)
	{
		return CapMemCellOnCapMemBlock(
		    random_value<CapMemColumnOnCapMemBlock>{}(exclude.x()),
		    random_value<CapMemRowOnCapMemBlock>{}(exclude.y()));
	};
};

HALDLS_TEST(
    CapMemBlockConfig,
    (enable_capmem)(debug_readout_enable)(debug_capmem_coord)(debug_v_ref_select)(
        debug_i_out_select)(debug_out_amp_bias)(debug_source_follower_bias)(
        debug_level_shifter_bias)(v_global_bias)(current_cell_res)(boost_factor)(enable_boost)(
        enable_autoboost)(prescale_pause)(prescale_ramp)(sub_counter)(pause_counter)(pulse_a)(
        pulse_b)(boost_a)(boost_b))

TEST(CapMemBlockConfig, EncodeDecode)
{
	CapMemBlockConfig config;
	config.set_enable_capmem(true);
	config.set_debug_readout_enable(true);
	config.set_debug_capmem_coord(CapMemCellOnCapMemBlock(Enum(10)));
	config.set_debug_v_ref_select(CapMemBlockConfig::VRefSelect::v_ref_i);
	config.set_debug_i_out_select(CapMemBlockConfig::IOutSelect::i_out_ramp);
	config.set_debug_out_amp_bias(CapMemBlockConfig::OutAmpBias(8));
	config.set_debug_source_follower_bias(CapMemBlockConfig::SourceFollowerBias(9));
	config.set_debug_level_shifter_bias(CapMemBlockConfig::LevelShifterBias(10));
	config.set_v_global_bias(CapMemBlockConfig::VGlobalBias(9));
	config.set_current_cell_res(CapMemBlockConfig::CurrentCellRes(23));
	config.set_boost_factor(CapMemBlockConfig::BoostFactor(6));
	config.set_enable_boost(true);
	config.set_enable_autoboost(true);
	config.set_prescale_pause(CapMemBlockConfig::PrescalePause(6));
	config.set_prescale_ramp(CapMemBlockConfig::PrescaleRamp(2));
	config.set_sub_counter(CapMemBlockConfig::SubCounter(16));
	config.set_pause_counter(CapMemBlockConfig::PauseCounter(18));
	config.set_pulse_a(CapMemBlockConfig::PulseA(15));
	config.set_pulse_b(CapMemBlockConfig::PulseB(123));
	config.set_boost_a(CapMemBlockConfig::BoostA(13));
	config.set_boost_b(CapMemBlockConfig::BoostB(134));

	CapMemBlockConfigOnDLS coord;
	std::array<OmnibusChipOverJTAGAddress, 10> ref_addresses = {
	    {OmnibusChipOverJTAGAddress(0x0014'4000), OmnibusChipOverJTAGAddress(0x0014'4001),
	     OmnibusChipOverJTAGAddress(0x0014'4002), OmnibusChipOverJTAGAddress(0x0014'4003),
	     OmnibusChipOverJTAGAddress(0x0014'4004), OmnibusChipOverJTAGAddress(0x0014'4005),
	     OmnibusChipOverJTAGAddress(0x0014'4006), OmnibusChipOverJTAGAddress(0x0014'4007),
	     OmnibusChipOverJTAGAddress(0x0014'4008), OmnibusChipOverJTAGAddress(0x0014'4009)}};
	EXPECT_EQ(ref_addresses, config.addresses<OmnibusChipOverJTAGAddress>(coord));

	std::array<fisch::vx::OmnibusChipOverJTAG, 10> ref_data = {
	    {fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusChipOverJTAG::Value(0x800000)),
	     fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusChipOverJTAG::Value(0xa)),
	     fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusChipOverJTAG::Value(0x2)),
	     fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusChipOverJTAG::Value(0x000089A9)),
	     fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusChipOverJTAG::Value(0x17)),
	     fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusChipOverJTAG::Value(0x2)),
	     fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusChipOverJTAG::Value(0x66270010)),
	     fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusChipOverJTAG::Value(0x000F007B)),
	     fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusChipOverJTAG::Value(0x000D0086)),
	     fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusChipOverJTAG::Value(0x12))}};
	EXPECT_EQ(ref_data, config.encode<fisch::vx::OmnibusChipOverJTAG>());

	HALDLS_TEST_ENCODE_DECODE(config, coord, ref_addresses, ref_data)
}
