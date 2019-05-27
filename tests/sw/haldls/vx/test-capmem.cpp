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
	for (auto column : halco::common::iter_all<CapMemColumnOnDLS>()) {
		for (auto row : halco::common::iter_all<CapMemRowOnDLS>()) {
			block.set_cell(CapMemCellOnCapMemBlock(column, row), CapMemCell::Value(i));
			ASSERT_EQ(block.get_cell(CapMemCellOnCapMemBlock(column, row)), i);
			i = (i + 1) % CapMemCell::Value::max;
		}
	}

	// test copy
	CapMemBlock copy = block;
	ASSERT_TRUE(copy == block);
	ASSERT_EQ(copy, block);
	ASSERT_FALSE(copy != block);
	copy.set_cell(
	    CapMemCellOnCapMemBlock(CapMemColumnOnDLS(), CapMemRowOnDLS()), CapMemCell::Value(0x23));
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
