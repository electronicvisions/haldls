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

TEST(FPGADeviceDNA, General)
{
	// test range
	EXPECT_NO_THROW(FPGADeviceDNA::Value(0x1ff'ffff'ffff'ffffull));
	EXPECT_ANY_THROW(FPGADeviceDNA::Value(0x200'0000'0000'0000ull));

	// test getter/setter
	FPGADeviceDNA word;
	word.set_value(FPGADeviceDNA::Value(145));
	ASSERT_EQ(word.get_value(), FPGADeviceDNA::Value(145));

	// test assign
	FPGADeviceDNA word_eq = word;
	FPGADeviceDNA word_ne(FPGADeviceDNA::Value(139));

	// test comparison
	ASSERT_EQ(word, word_eq);
	ASSERT_FALSE(word == word_ne);

	ASSERT_NE(word, word_ne);
	ASSERT_FALSE(word != word_eq);

	// test ostream operator
	std::stringstream out;
	word.set_value(FPGADeviceDNA::Value(1234));
	out << word;
	EXPECT_NE(out.str().find("Value(1234)"), std::string::npos);
}

TEST(FPGADeviceDNA, EncodeDecode)
{
	typedef std::vector<fisch::vx::Omnibus::coordinate_type> addresses_type;

	FPGADeviceDNA config;

	config.set_value(FPGADeviceDNA::Value(0x100'0000'022b));

	FPGADeviceDNAOnFPGA coord;

	std::array<
	    typename fisch::vx::Omnibus::coordinate_type, FPGADeviceDNA::read_config_size_in_words>
	    ref_read_addresses = {typename fisch::vx::Omnibus::coordinate_type{0x8000'0003ul},
	                          typename fisch::vx::Omnibus::coordinate_type{0x8000'0004ul}};
	std::array<
	    typename fisch::vx::Omnibus::coordinate_type, FPGADeviceDNA::write_config_size_in_words>
	    ref_write_addresses = {};
	std::array<fisch::vx::Omnibus, FPGADeviceDNA::read_config_size_in_words> ref_data = {
	    fisch::vx::Omnibus(fisch::vx::Omnibus::Value(0x100)),
	    fisch::vx::Omnibus(fisch::vx::Omnibus::Value(0x22b))};

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_write_addresses));
	}

	{ // read addresses
		addresses_type read_addresses;
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_read_addresses));
	}

	FPGADeviceDNA config_copy;
	ASSERT_NE(config, config_copy);
	std::vector<fisch::vx::Omnibus> data(FPGADeviceDNA::read_config_size_in_words);
	std::copy(ref_data.cbegin(), ref_data.cend(), data.begin());
	visit_preorder(
	    config_copy, coord,
	    stadls::DecodeVisitor<std::vector<fisch::vx::Omnibus>>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(FPGADeviceDNA, CerealizeCoverage)
{
	FPGADeviceDNA obj1, obj2;
	obj1.set_value(draw_ranged_non_default_value<FPGADeviceDNA::Value>(0));

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
