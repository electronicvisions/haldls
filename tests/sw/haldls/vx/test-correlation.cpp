#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/jtag.h"
#include "haldls/vx/correlation.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;
using namespace fisch::vx;

typedef std::vector<OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<OmnibusChipOverJTAG> words_type;

TEST(CommonCorrelationConfig, General)
{
	CommonCorrelationConfig config;

	// test getter/setter
	{
		auto value = draw_ranged_non_default_value<CommonCorrelationConfig::SenseDelay>(
		    config.get_sense_delay());
		config.set_sense_delay(value);
		EXPECT_EQ(config.get_sense_delay(), value);
	}

	{
		auto value = draw_ranged_non_default_value<CommonCorrelationConfig::ResetDuration>(
		    config.get_reset_duration());
		config.set_reset_duration(value);
		EXPECT_EQ(config.get_reset_duration(), value);
	}

	{
		auto value = draw_ranged_non_default_value<CommonCorrelationConfig::ResetFallTime>(
		    config.get_reset_fall_time());
		config.set_reset_fall_time(value);
		EXPECT_EQ(config.get_reset_fall_time(), value);
	}

	{
		bool value = !config.get_reset_mode();
		config.set_reset_mode(value);
		EXPECT_EQ(config.get_reset_mode(), value);
	}

	CommonCorrelationConfig config_eq = config;
	CommonCorrelationConfig config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(CommonCorrelationConfig, EncodeDecode)
{
	CommonCorrelationConfig config;

	config.set_sense_delay(CommonCorrelationConfig::SenseDelay(100));
	config.set_reset_duration(CommonCorrelationConfig::ResetDuration(500));
	config.set_reset_fall_time(CommonCorrelationConfig::ResetFallTime(30));
	config.set_reset_mode(true);

	auto coord = CommonCorrelationConfigOnDLS::bottom;

	std::array<OmnibusChipOverJTAGAddress, CommonCorrelationConfig::config_size_in_words>
	    ref_addresses = {
	        OmnibusChipOverJTAGAddress{0x0320'0000ul}, OmnibusChipOverJTAGAddress{0x0320'0001ul},
	        OmnibusChipOverJTAGAddress{0x0320'0002ul}, OmnibusChipOverJTAGAddress{0x0320'0003ul}};
	std::array<OmnibusChipOverJTAG, CommonCorrelationConfig::config_size_in_words> ref_data = {
	    OmnibusData{100}, OmnibusData{500}, OmnibusData{30}, OmnibusData{1}};

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

	CommonCorrelationConfig config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(CommonCorrelationConfig, CerealizeCoverage)
{
	CommonCorrelationConfig obj1, obj2;
	{
		auto value = draw_ranged_non_default_value<CommonCorrelationConfig::SenseDelay>(
		    obj1.get_sense_delay());
		obj1.set_sense_delay(value);
	}

	{
		auto value = draw_ranged_non_default_value<CommonCorrelationConfig::ResetDuration>(
		    obj1.get_reset_duration());
		obj1.set_reset_duration(value);
	}

	{
		auto value = draw_ranged_non_default_value<CommonCorrelationConfig::ResetFallTime>(
		    obj1.get_reset_fall_time());
		obj1.set_reset_fall_time(value);
	}

	{
		bool value = !obj1.get_reset_mode();
		obj1.set_reset_mode(value);
	}

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
