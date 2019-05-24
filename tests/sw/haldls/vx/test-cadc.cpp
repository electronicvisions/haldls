#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/jtag.h"
#include "haldls/vx/cadc.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;
using namespace fisch::vx;

typedef std::vector<OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<OmnibusChipOverJTAG> words_type;

TEST(CADCConfig, General)
{
	CADCConfig config;

	// test getter/setter
	{
		bool value = !config.get_enable();
		config.set_enable(value);
		EXPECT_EQ(config.get_enable(), value);
	}

	{
		auto value = draw_ranged_non_default_value<CADCConfig::ResetWait>(config.get_reset_wait());
		config.set_reset_wait(value);
		EXPECT_EQ(config.get_reset_wait(), value);
	}

	{
		auto value = draw_ranged_non_default_value<CADCConfig::DeadTime>(config.get_dead_time());
		config.set_dead_time(value);
		EXPECT_EQ(config.get_dead_time(), value);
	}

	CADCConfig config_eq = config;
	CADCConfig config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(CADCConfig, EncodeDecode)
{
	CADCConfig config;

	config.set_enable(true);
	config.set_reset_wait(CADCConfig::ResetWait(12));

	CADCConfigOnDLS coord(0);

	std::array<OmnibusChipOverJTAGAddress, CADCConfig::config_size_in_words> ref_addresses = {
	    OmnibusChipOverJTAGAddress{0x0240'1000ul}};
	std::array<OmnibusChipOverJTAG, CADCConfig::config_size_in_words> ref_data = {
	    OmnibusData{1 | 12 << 4}};

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

	CADCConfig config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(CADCConfig, CerealizeCoverage)
{
	CADCConfig obj1, obj2;
	obj1.set_enable(!obj1.get_enable());
	obj1.set_reset_wait(
	    draw_ranged_non_default_value<CADCConfig::ResetWait>(obj1.get_reset_wait()));
	obj1.set_dead_time(draw_ranged_non_default_value<CADCConfig::DeadTime>(obj1.get_dead_time()));

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
