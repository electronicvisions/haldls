#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/jtag.h"
#include "haldls/vx/ppu.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<omnibus_address_type> addresses_type;
typedef std::vector<fisch::vx::OmnibusOnChipOverJTAG> words_type;

TEST(PPUMemoryWord, General)
{
	// test range
	EXPECT_NO_THROW(PPUMemoryWord::Value(0xffffffff));

	// test getter/setter
	PPUMemoryWord word;
	word.set(PPUMemoryWord::Value(145));
	ASSERT_EQ(word.get(), PPUMemoryWord::Value(145));

	// test assign
	PPUMemoryWord word_eq = word;
	PPUMemoryWord word_ne(PPUMemoryWord::Value(139));

	// test comparison
	ASSERT_EQ(word, word_eq);
	ASSERT_FALSE(word == word_ne);

	ASSERT_NE(word, word_ne);
	ASSERT_FALSE(word != word_eq);

	// test ostream operator
	std::stringstream out;
	word.set(PPUMemoryWord::Value(0xdeadbeef));
	out << word;
	ASSERT_TRUE(out.str().find("0xdeadbeef") != std::string::npos);
}

TEST(PPUMemoryWord, EncodeDecode)
{
	PPUMemoryWord config;

	config.set(PPUMemoryWord::Value(555));

	PPUMemoryWordOnDLS coord(Enum(0x123));

	std::array<omnibus_address_type, PPUMemoryWord::config_size_in_words> ref_addresses = {
	    omnibus_address_type{0x02800123ul}};
	std::array<fisch::vx::OmnibusOnChipOverJTAG, PPUMemoryWord::config_size_in_words> ref_data = {
	    omnibus_word_type{555ul}};

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

	PPUMemoryWord config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(PPUMemoryWord, CerealizeCoverage)
{
	PPUMemoryWord obj1, obj2;
	obj1.set(draw_ranged_non_default_value<PPUMemoryWord::Value>(0));

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
