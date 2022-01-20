#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "halco/common/iter_all.h"
#include "halco/hicann-dls/vx/ultra96.h"
#include "haldls/vx/i2c.h"

#include "fisch/vx/word_access/type/i2c.h"
#include "halco/hicann-dls/vx/i2c.h"
#include "haldls/vx/common.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

TEST(DAC6573ChannelConfig, General)
{
	// test getter/setter
	DAC6573ChannelConfig config;

	// test getter/setter
	{
		auto value_changed = DAC6573ChannelConfig::Value(config.get_value() + 0x120);
		config.set_value(value_changed);

		EXPECT_EQ(config.get_value(), value_changed);
	}

	// test comparison operators
	{
		DAC6573ChannelConfig config_equal = config;
		DAC6573ChannelConfig config_default;

		ASSERT_EQ(config, config_equal);
		ASSERT_FALSE(config == config_default);

		ASSERT_NE(config, config_default);
		ASSERT_TRUE(config != config_default);
	}
}

TEST(DAC6573ChannelConfig, EncodeDecode)
{
	DAC6573ChannelConfig config;

	config.set_value(DAC6573ChannelConfig::Value(987));

	DAC6573ChannelOnBoard coord = DAC6573ChannelOnBoard::v_res_meas;

	// Generate reference addresses and data
	std::array<I2CDAC6573RwRegisterOnBoard, DAC6573ChannelConfig::config_size_in_words>
	    ref_addresses = {I2CDAC6573RwRegisterOnBoard(
	        coord.toDAC6573ChannelOnDAC6573(), coord.toDAC6573OnBoard())};
	std::array<
	    fisch::vx::word_access_type::I2CDAC6573RwRegister,
	    DAC6573ChannelConfig::config_size_in_words>
	    ref_data = {fisch::vx::word_access_type::I2CDAC6573RwRegister(987)};

	{ // test write_addresses
		std::vector<I2CDAC6573RwRegisterOnBoard> write_addresses;
		visit_preorder(
		    config, coord,
		    stadls::WriteAddressVisitor<std::vector<I2CDAC6573RwRegisterOnBoard>>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // test data encoding
		std::vector<fisch::vx::word_access_type::I2CDAC6573RwRegister> data;
		visit_preorder(
		    config, coord,
		    stadls::EncodeVisitor<std::vector<fisch::vx::word_access_type::I2CDAC6573RwRegister>>{
		        data});
		EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
	}

	{ // test data decoding
		DAC6573ChannelConfig config_decoded;
		visit_preorder(
		    config_decoded, coord,
		    stadls::DecodeVisitor<std::array<
		        fisch::vx::word_access_type::I2CDAC6573RwRegister,
		        DAC6573ChannelConfig::config_size_in_words>>{ref_data});
		EXPECT_EQ(config, config_decoded);
	}
}

TEST(DAC6573ChannelConfig, CerealizeCoverage)
{
	DAC6573ChannelConfig conf1, conf2;

	// set test config values
	conf1.set_value(DAC6573ChannelConfig::Value(987));

	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(conf1);
	}

	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(conf2);
	}
	ASSERT_EQ(conf1, conf2);
}