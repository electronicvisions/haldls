#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/container_cast.h"
#include "fisch/vx/word_access/type/jtag.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "fisch/vx/word_access/type/systime.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/systime.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

TEST(SystimeSyncBase, General)
{
	// test range
	EXPECT_NO_THROW(SystimeSyncBase::Value(0x7ffffffffff));
	EXPECT_ANY_THROW(SystimeSyncBase::Value(0x80000000000));

	// test getter/setter
	SystimeSyncBase word;
	word.set_value(SystimeSyncBase::Value(145));
	ASSERT_EQ(word.get_value(), SystimeSyncBase::Value(145));

	// test assign
	SystimeSyncBase word_eq = word;
	SystimeSyncBase word_ne(SystimeSyncBase::Value(139));

	// test comparison
	ASSERT_EQ(word, word_eq);
	ASSERT_FALSE(word == word_ne);

	ASSERT_NE(word, word_ne);
	ASSERT_FALSE(word != word_eq);

	// test ostream operator
	std::stringstream out;
	word.set_value(SystimeSyncBase::Value(12345));
	out << word;
	ASSERT_TRUE(out.str().find("12345") != std::string::npos);
}

template <typename WordT>
void test_encode_decode()
{
	typedef std::vector<typename decltype(
	    fisch::vx::container_cast(std::declval<WordT>()))::coordinate_type>
	    addresses_type;

	SystimeSyncBase config;

	config.set_value(SystimeSyncBase::Value(0x1000000022b));

	SystimeSyncBaseOnDLS coord;

	std::array<typename addresses_type::value_type, SystimeSyncBase::config_size_in_words>
	    ref_addresses = {
	        typename addresses_type::value_type{0x0ul}, typename addresses_type::value_type{0x1ul}};
	std::array<WordT, SystimeSyncBase::config_size_in_words> ref_data = {
	    WordT(0x22b), WordT(0x100)};

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

	std::vector<WordT> data;
	visit_preorder(config, coord, stadls::EncodeVisitor<std::vector<WordT>>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	SystimeSyncBase config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<std::vector<WordT>>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(SystimeSyncBase, EncodeDecode_Omnibus)
{
	test_encode_decode<fisch::vx::word_access_type::Omnibus>();
}

TEST(SystimeSyncBase, EncodeDecode_OmnibusChipOverJTAG)
{
	test_encode_decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>();
}

TEST(SystimeSyncBase, CerealizeCoverage)
{
	SystimeSyncBase obj1, obj2;
	obj1.set_value(draw_ranged_non_default_value<SystimeSyncBase::Value>(0));

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
