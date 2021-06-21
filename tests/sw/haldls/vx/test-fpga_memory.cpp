#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/vx/fpga.h"

#include "fisch/vx/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/common.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

TEST(ExternalPPUMemoryByte, General)
{
	// test range
	EXPECT_NO_THROW(ExternalPPUMemoryByte::Value(0xff));
	EXPECT_ANY_THROW(ExternalPPUMemoryByte::Value(0x100));

	// test getter/setter
	ExternalPPUMemoryByte word;
	word.set_value(ExternalPPUMemoryByte::Value(145));
	ASSERT_EQ(word.get_value(), ExternalPPUMemoryByte::Value(145));

	// test assign
	ExternalPPUMemoryByte word_eq = word;
	ExternalPPUMemoryByte word_ne(ExternalPPUMemoryByte::Value(139));

	// test comparison
	ASSERT_EQ(word, word_eq);
	ASSERT_FALSE(word == word_ne);

	ASSERT_NE(word, word_ne);
	ASSERT_FALSE(word != word_eq);

	// test ostream operator
	std::stringstream out;
	word.set_value(ExternalPPUMemoryByte::Value(12));
	out << word;
	EXPECT_EQ(out.str(), "ExternalPPUMemoryByte(0x0c)");
}

TEST(ExternalPPUMemoryByte, EncodeDecode)
{
	typedef std::vector<fisch::vx::Omnibus::coordinate_type> addresses_type;

	ExternalPPUMemoryByte config;

	config.set_value(ExternalPPUMemoryByte::Value(0x12));

	ExternalPPUMemoryByteOnFPGA coord(22);

	std::array<
	    typename fisch::vx::Omnibus::coordinate_type, ExternalPPUMemoryByte::config_size_in_words>
	    ref_addresses = {typename fisch::vx::Omnibus::coordinate_type{0x8e00'0000 + (22 / 4)}};
	std::array<fisch::vx::Omnibus, ExternalPPUMemoryByte::config_size_in_words> ref_data = {
	    fisch::vx::Omnibus(fisch::vx::Omnibus::Value(0x0000'1200), {false, true, false, false})};

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

	std::vector<fisch::vx::Omnibus> data;
	visit_preorder(config, coord, stadls::EncodeVisitor<std::vector<fisch::vx::Omnibus>>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
	ExternalPPUMemoryByte config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(
	    config_copy, coord,
	    stadls::DecodeVisitor<std::vector<fisch::vx::Omnibus>>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(ExternalPPUMemoryByte, CerealizeCoverage)
{
	ExternalPPUMemoryByte obj1, obj2;
	obj1.set_value(draw_ranged_non_default_value<ExternalPPUMemoryByte::Value>(0));

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
