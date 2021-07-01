#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/vx/v3/correlation.h"

#include "fisch/vx/word_access/type/jtag.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/v3/common.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx::v3;
using namespace halco::hicann_dls::vx;
using namespace halco::common;
using namespace fisch::vx::word_access_type;

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
		EXPECT_EQ(config.get_reset_mode(), CommonCorrelationConfig::ResetMode::normal);
		config.set_reset_mode(CommonCorrelationConfig::ResetMode::auto_calibrating);
		EXPECT_EQ(config.get_reset_mode(), CommonCorrelationConfig::ResetMode::auto_calibrating);
	}

	{
		auto value = config.get_cadc_v_offset_assignment();
		EXPECT_EQ(value[CapMemBlockOnHemisphere::left], CapMemBlockOnHemisphere::left);
		EXPECT_EQ(value[CapMemBlockOnHemisphere::right], CapMemBlockOnHemisphere::right);

		value[CapMemBlockOnHemisphere::left] = CapMemBlockOnHemisphere::right;
		value[CapMemBlockOnHemisphere::right] = CapMemBlockOnHemisphere::left;
		config.set_cadc_v_offset_assignment(value);

		EXPECT_EQ(
		    config.get_cadc_v_offset_assignment()[CapMemBlockOnHemisphere::left],
		    CapMemBlockOnHemisphere::right);
		EXPECT_EQ(
		    config.get_cadc_v_offset_assignment()[CapMemBlockOnHemisphere::right],
		    CapMemBlockOnHemisphere::left);
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
	config.set_reset_mode(CommonCorrelationConfig::ResetMode::auto_calibrating);
	auto value = config.get_cadc_v_offset_assignment();
	value[CapMemBlockOnHemisphere::left] = CapMemBlockOnHemisphere::right;
	config.set_cadc_v_offset_assignment(value);

	auto coord = CommonCorrelationConfigOnDLS::bottom;

	std::array<OmnibusChipOverJTAGAddress, CommonCorrelationConfig::config_size_in_words>
	    ref_addresses = {
	        OmnibusChipOverJTAGAddress{0x0320'0000ul}, OmnibusChipOverJTAGAddress{0x0320'0001ul},
	        OmnibusChipOverJTAGAddress{0x0320'0002ul}, OmnibusChipOverJTAGAddress{0x0320'0003ul}};
	std::array<OmnibusChipOverJTAG, CommonCorrelationConfig::config_size_in_words> ref_data = {
	    OmnibusChipOverJTAG(100), OmnibusChipOverJTAG(500), OmnibusChipOverJTAG(30),
	    OmnibusChipOverJTAG(0b101)};

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
		obj1.set_reset_mode(CommonCorrelationConfig::ResetMode::auto_calibrating);
	}

	{
		auto value = obj1.get_cadc_v_offset_assignment();
		value[CapMemBlockOnHemisphere::left] = CapMemBlockOnHemisphere::right;
		value[CapMemBlockOnHemisphere::right] = CapMemBlockOnHemisphere::left;
		obj1.set_cadc_v_offset_assignment(value);
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
