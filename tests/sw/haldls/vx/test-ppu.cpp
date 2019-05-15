#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/jtag.h"
#include "haldls/vx/ppu.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;
using namespace fisch::vx;

typedef std::vector<OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<OmnibusChipOverJTAG> words_type;

TEST(PPUMemoryWord, General)
{
	test_generic_functionality_single_value<PPUMemoryWord>();
	test_hex_ostream_operator_single_value<PPUMemoryWord>();
}

TEST(PPUMemoryWord, EncodeDecode)
{
	PPUMemoryWord config;

	config.set_value(PPUMemoryWord::Value(555));

	PPUMemoryWordOnDLS coord(Enum(0x123));

	std::array<OmnibusChipOverJTAGAddress, PPUMemoryWord::config_size_in_words> ref_addresses = {
	    OmnibusChipOverJTAGAddress{0x02800123ul}};
	std::array<OmnibusChipOverJTAG, PPUMemoryWord::config_size_in_words> ref_data = {
	    OmnibusData{555ul}};

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
	obj1.set_value(draw_ranged_non_default_value<PPUMemoryWord::Value>(0));

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

TEST(PPUControlRegister, General)
{
	PPUControlRegister r1, r2;
	r1.set_inhibit_reset(true);
	ASSERT_TRUE(r1.get_inhibit_reset());
	r1.set_force_clock_on(true);
	ASSERT_TRUE(r1.get_force_clock_on());
	r1.set_force_clock_off(true);
	ASSERT_TRUE(r1.get_force_clock_off());
	r1.set_cache_controller_enable(true);
	ASSERT_TRUE(r1.get_cache_controller_enable());
	r1.set_inhibit_reset(false);
	ASSERT_FALSE(r1.get_inhibit_reset());
	r1.set_force_clock_on(false);
	ASSERT_FALSE(r1.get_force_clock_on());
	r1.set_force_clock_off(false);
	ASSERT_FALSE(r1.get_force_clock_off());
	r1.set_cache_controller_enable(false);
	ASSERT_FALSE(r1.get_cache_controller_enable());

	r2 = r1;
	ASSERT_EQ(r1, r2);
	ASSERT_TRUE(r1 == r2);
	ASSERT_FALSE(r1 != r1);
	r2.set_inhibit_reset(!r1.get_inhibit_reset());
	ASSERT_NE(r1, r2);
	ASSERT_TRUE(r1 != r2);
	ASSERT_FALSE(r1 == r2);

	r2 = r1;
	r2.set_force_clock_on(!r1.get_force_clock_on());
	ASSERT_NE(r1, r2);
	ASSERT_TRUE(r1 != r2);
	ASSERT_FALSE(r1 == r2);

	r2 = r1;
	r2.set_force_clock_off(!r1.get_force_clock_off());
	ASSERT_NE(r1, r2);
	ASSERT_TRUE(r1 != r2);
	ASSERT_FALSE(r1 == r2);

	r2 = r1;
	r2.set_cache_controller_enable(!r1.get_cache_controller_enable());
	ASSERT_NE(r1, r2);
	ASSERT_TRUE(r1 != r2);
	ASSERT_FALSE(r1 == r2);
}

TEST(PPUControlRegister, EncodeDecode)
{
	PPUControlRegister config;

	config.set_cache_controller_enable(true);
	config.set_inhibit_reset(true);
	config.set_force_clock_on(false);
	config.set_force_clock_off(true);

	PPUControlRegisterOnDLS coord(PPUControlRegisterOnPPU(), PPUOnDLS(0));

	std::array<OmnibusChipOverJTAGAddress, PPUControlRegister::config_size_in_words> ref_addresses =
	    {OmnibusChipOverJTAGAddress{0x2a00000}};
	std::array<OmnibusChipOverJTAG, PPUControlRegister::config_size_in_words> ref_data = {
	    OmnibusData{0b1101ul}};

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

	PPUControlRegister config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(PPUControlRegister, CerealizeCoverage)
{
	PPUControlRegister obj1, obj2;

	obj1.set_cache_controller_enable(!obj1.get_cache_controller_enable());
	obj1.set_inhibit_reset(!obj1.get_inhibit_reset());
	obj1.set_force_clock_on(!obj1.get_force_clock_on());
	obj1.set_force_clock_off(!obj1.get_force_clock_off());

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


TEST(PPUStatusRegister, EncodeDecode)
{
	PPUStatusRegister config;

	PPUStatusRegisterOnDLS coord(PPUStatusRegisterOnPPU(), PPUOnDLS(0));

	std::array<OmnibusChipOverJTAGAddress, PPUStatusRegister::config_size_in_words> ref_addresses =
	    {OmnibusChipOverJTAGAddress{0x2a00001ul}};
	std::array<OmnibusChipOverJTAG, PPUStatusRegister::config_size_in_words> ref_data = {
	    OmnibusData{0b0ul}};

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

	PPUStatusRegister config_copy;
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
	ASSERT_FALSE(config_copy.get_sleep());

	data.clear();
	data.push_back(OmnibusData{0b1ul});
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_TRUE(config_copy.get_sleep());
}

TEST(PPUStatusRegister, CerealizeCoverage)
{
	PPUStatusRegister obj1, obj2;
	std::array<OmnibusChipOverJTAG, PPUStatusRegister::config_size_in_words> data = {
	    {static_cast<OmnibusData>(rand())}};
	obj1.decode(data);

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
