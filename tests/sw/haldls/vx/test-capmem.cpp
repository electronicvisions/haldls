#include <cmath>
#include <string>
#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "fisch/vx/jtag.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/vx/capmem.h"
#include "haldls/vx/common.h"
#include "haldls/vx/omnibus_constants.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;
using namespace hate::math;

typedef std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<fisch::vx::OmnibusChipOverJTAG> words_type;

TEST(CapMemCell, General)
{
	test_generic_functionality_single_value<CapMemCell>();
	test_hex_ostream_operator_single_value<CapMemCell>();
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
	    static_cast<fisch::vx::OmnibusData>(val)};

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(cell, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		addresses_type read_addresses;
		visit_preorder(cell, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(cell, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	CapMemCell cell_copy;
	ASSERT_NE(cell, cell_copy);
	visit_preorder(cell_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(cell, cell_copy);
}

TEST(CapMemBlock, General)
{
	CapMemBlock block;
	size_t i = 0;
	for (auto cell : halco::common::iter_all<CapMemCellOnCapMemBlock>()) {
		block.set_cell(cell, CapMemCell::Value(i));
		ASSERT_EQ(block.get_cell(cell), CapMemCell::Value(i));
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
	std::stringstream expect_to_find;
	expect_to_find << std::showbase << std::internal << std::setfill('0')
	               << std::setw(round_up_integer_division(num_bits(CapMemCell::Value::max), 4))
	               << std::hex << 0x23;
	std::stringstream out;
	out << copy;
	ASSERT_TRUE(out.str().find(expect_to_find.str()) != std::string::npos);
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
			    static_cast<fisch::vx::OmnibusData>(val);
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

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(
		    block, block_coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		addresses_type read_addresses;
		visit_preorder(
		    block, block_coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;

	visit_preorder(block, block_coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	CapMemBlock other;
	ASSERT_NE(block, other);
	visit_preorder(other, block_coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(block, other);
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

TEST(CapMemBlockConfig, General)
{
	CapMemBlockConfig config;

	EXPECT_ANY_THROW(CapMemBlockConfig::OutAmpBias(16));
	EXPECT_NO_THROW(CapMemBlockConfig::OutAmpBias(15));
	EXPECT_ANY_THROW(CapMemBlockConfig::SourceFollowerBias(16));
	EXPECT_NO_THROW(CapMemBlockConfig::SourceFollowerBias(15));
	EXPECT_ANY_THROW(CapMemBlockConfig::LevelShifterBias(16));
	EXPECT_NO_THROW(CapMemBlockConfig::LevelShifterBias(15));
	EXPECT_ANY_THROW(CapMemBlockConfig::VGlobalBias(16));
	EXPECT_NO_THROW(CapMemBlockConfig::VGlobalBias(15));
	EXPECT_ANY_THROW(CapMemBlockConfig::CurrentCellRes(64));
	EXPECT_NO_THROW(CapMemBlockConfig::CurrentCellRes(63));
	EXPECT_ANY_THROW(CapMemBlockConfig::BoostFactor(16));
	EXPECT_NO_THROW(CapMemBlockConfig::BoostFactor(15));
	EXPECT_ANY_THROW(CapMemBlockConfig::PrescalePause(7));
	EXPECT_NO_THROW(CapMemBlockConfig::PrescalePause(6));
	EXPECT_ANY_THROW(CapMemBlockConfig::PrescaleRamp(7));
	EXPECT_NO_THROW(CapMemBlockConfig::PrescaleRamp(6));
	EXPECT_ANY_THROW(CapMemBlockConfig::SubCounter(65536));
	EXPECT_NO_THROW(CapMemBlockConfig::SubCounter(65535));
	EXPECT_ANY_THROW(CapMemBlockConfig::PulseA(65536));
	EXPECT_NO_THROW(CapMemBlockConfig::PulseA(65535));
	EXPECT_ANY_THROW(CapMemBlockConfig::PulseB(65536));
	EXPECT_NO_THROW(CapMemBlockConfig::PulseB(65535));
	EXPECT_ANY_THROW(CapMemBlockConfig::BoostA(65536));
	EXPECT_NO_THROW(CapMemBlockConfig::BoostA(65535));
	EXPECT_ANY_THROW(CapMemBlockConfig::BoostB(65536));
	EXPECT_NO_THROW(CapMemBlockConfig::BoostB(65535));

	// test default constructor
	ASSERT_EQ(config.get_pause_counter(), CapMemBlockConfig::PauseCounter(8096));
	ASSERT_EQ(config.get_debug_out_amp_bias(), CapMemBlockConfig::OutAmpBias(7));

	// test getter/setter
	config.set_enable_capmem(true);
	ASSERT_EQ(config.get_enable_capmem(), true);

	config.set_debug_readout_enable(true);
	ASSERT_EQ(config.get_debug_readout_enable(), true);

	config.set_debug_capmem_coord(CapMemCellOnDLS(Enum(10)));
	ASSERT_EQ(config.get_debug_capmem_coord(), CapMemCellOnDLS(Enum(10)));

	config.set_debug_v_ref_select(CapMemBlockConfig::VRefSelect::v_ref_i);
	ASSERT_EQ(config.get_debug_v_ref_select(), CapMemBlockConfig::VRefSelect::v_ref_i);

	config.set_debug_i_out_select(CapMemBlockConfig::IOutSelect::i_out_ramp);
	ASSERT_EQ(config.get_debug_i_out_select(), CapMemBlockConfig::IOutSelect::i_out_ramp);

	config.set_debug_out_amp_bias(CapMemBlockConfig::OutAmpBias(8));
	ASSERT_EQ(config.get_debug_out_amp_bias(), CapMemBlockConfig::OutAmpBias(8));

	config.set_debug_source_follower_bias(CapMemBlockConfig::SourceFollowerBias(9));
	ASSERT_EQ(config.get_debug_source_follower_bias(), CapMemBlockConfig::SourceFollowerBias(9));

	config.set_debug_level_shifter_bias(CapMemBlockConfig::LevelShifterBias(10));
	ASSERT_EQ(config.get_debug_level_shifter_bias(), CapMemBlockConfig::LevelShifterBias(10));

	config.set_v_global_bias(CapMemBlockConfig::VGlobalBias(9));
	ASSERT_EQ(config.get_v_global_bias(), CapMemBlockConfig::VGlobalBias(9));

	config.set_current_cell_res(CapMemBlockConfig::CurrentCellRes(23));
	ASSERT_EQ(config.get_current_cell_res(), CapMemBlockConfig::CurrentCellRes(23));

	config.set_boost_factor(CapMemBlockConfig::BoostFactor(6));
	ASSERT_EQ(config.get_boost_factor(), CapMemBlockConfig::BoostFactor(6));

	config.set_enable_boost(true);
	ASSERT_EQ(config.get_enable_boost(), true);

	config.set_enable_autoboost(true);
	ASSERT_EQ(config.get_enable_autoboost(), true);

	config.set_prescale_pause(CapMemBlockConfig::PrescalePause(6));
	ASSERT_EQ(config.get_prescale_pause(), CapMemBlockConfig::PrescalePause(6));

	config.set_prescale_ramp(CapMemBlockConfig::PrescaleRamp(2));
	ASSERT_EQ(config.get_prescale_ramp(), CapMemBlockConfig::PrescaleRamp(2));

	config.set_sub_counter(CapMemBlockConfig::SubCounter(16));
	ASSERT_EQ(config.get_sub_counter(), CapMemBlockConfig::SubCounter(16));

	config.set_pause_counter(CapMemBlockConfig::PauseCounter(18));
	ASSERT_EQ(config.get_pause_counter(), CapMemBlockConfig::PauseCounter(18));

	config.set_pulse_a(CapMemBlockConfig::PulseA(15));
	ASSERT_EQ(config.get_pulse_a(), CapMemBlockConfig::PulseA(15));

	config.set_pulse_b(CapMemBlockConfig::PulseB(123));
	ASSERT_EQ(config.get_pulse_b(), CapMemBlockConfig::PulseB(123));

	config.set_boost_a(CapMemBlockConfig::BoostA(13));
	ASSERT_EQ(config.get_boost_a(), CapMemBlockConfig::BoostA(13));

	config.set_boost_b(CapMemBlockConfig::BoostB(134));
	ASSERT_EQ(config.get_boost_b(), CapMemBlockConfig::BoostB(134));


	CapMemBlockConfig config_eq = config;
	CapMemBlockConfig config_ne(config);
	config_ne.set_boost_b(CapMemBlockConfig::BoostB(118));
	config_ne.set_enable_autoboost(false);

	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_ne);

	ASSERT_NE(config, config_ne);
	ASSERT_FALSE(config != config_eq);
}


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
	    {fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusData(0x800000)),
	     fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusData(0xa)),
	     fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusData(0x2)),
	     fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusData(0x000089A9)),
	     fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusData(0x17)),
	     fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusData(0x2)),
	     fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusData(0x66270010)),
	     fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusData(0x000F007B)),
	     fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusData(0x000D0086)),
	     fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusData(0x12))}};
	EXPECT_EQ(ref_data, config.encode<fisch::vx::OmnibusChipOverJTAG>());

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		addresses_type read_addresses;
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	CapMemBlockConfig config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(CapMemBlockConfig, CerealizeCoverage)
{
	CapMemBlockConfig obj1, obj2;
	obj1.set_enable_capmem(!obj1.get_enable_capmem());
	obj1.set_debug_readout_enable(!obj1.get_debug_readout_enable());
	obj1.set_enable_boost(!obj1.get_enable_boost());
	obj1.set_enable_autoboost(!obj1.get_enable_autoboost());
	obj1.set_debug_capmem_coord(halco::hicann_dls::vx::CapMemCellOnCapMemBlock(
	    draw_ranged_non_default_value<halco::hicann_dls::vx::CapMemRowOnCapMemBlock>(0),
	    draw_ranged_non_default_value<halco::hicann_dls::vx::CapMemColumnOnCapMemBlock>(0)));
	obj1.set_debug_v_ref_select(CapMemBlockConfig::VRefSelect::v_ref_v);
	obj1.set_debug_i_out_select(CapMemBlockConfig::IOutSelect::i_out_ramp);
#define RANDOMIZE(name, type)                                                                      \
	obj1.set_##name(draw_ranged_non_default_value<type>(obj1.get_##name()));
	RANDOMIZE(debug_out_amp_bias, CapMemBlockConfig::OutAmpBias)
	RANDOMIZE(debug_source_follower_bias, CapMemBlockConfig::SourceFollowerBias)
	RANDOMIZE(debug_level_shifter_bias, CapMemBlockConfig::LevelShifterBias)
	RANDOMIZE(v_global_bias, CapMemBlockConfig::VGlobalBias)
	RANDOMIZE(current_cell_res, CapMemBlockConfig::CurrentCellRes)
	RANDOMIZE(boost_factor, CapMemBlockConfig::BoostFactor)
	RANDOMIZE(prescale_pause, CapMemBlockConfig::PrescalePause)
	RANDOMIZE(prescale_ramp, CapMemBlockConfig::PrescaleRamp)
	RANDOMIZE(sub_counter, CapMemBlockConfig::SubCounter)
	RANDOMIZE(pause_counter, CapMemBlockConfig::PauseCounter)
	RANDOMIZE(pulse_a, CapMemBlockConfig::PulseA)
	RANDOMIZE(pulse_b, CapMemBlockConfig::PulseB)
	RANDOMIZE(boost_a, CapMemBlockConfig::BoostA)
	RANDOMIZE(boost_b, CapMemBlockConfig::BoostB)
#undef RANDOMIZE
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
