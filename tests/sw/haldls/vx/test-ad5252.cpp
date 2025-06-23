#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "halco/common/iter_all.h"
#include "halco/hicann-dls/vx/jboa.h"
#include "haldls/vx/i2c.h"

#include "fisch/vx/word_access/type/i2c.h"
#include "halco/hicann-dls/vx/i2c.h"
#include "haldls/vx/common.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

TEST(AD5252ChannelConfig, General)
{
	// test getter/setter
	AD5252ChannelConfig config;

	// test getter/setter
	{
		auto value_changed = AD5252ChannelConfig::WiperSetting(config.get_value() + 0x10);
		config.set_value(value_changed);

		EXPECT_EQ(config.get_value(), value_changed);
	}

	// test comparison operators
	{
		AD5252ChannelConfig config_equal = config;
		AD5252ChannelConfig config_default;

		ASSERT_EQ(config, config_equal);
		ASSERT_FALSE(config == config_default);

		ASSERT_NE(config, config_default);
		ASSERT_TRUE(config != config_default);
	}
}

TEST(AD5252ChannelConfig, EncodeDecode)
{
	AD5252ChannelConfig config;

	config.set_value(AD5252ChannelConfig::WiperSetting(0x70));

	AD5252ChannelOnBoard coord;

	// Generate reference addresses and data
	std::array<I2CAD5252RwRegisterOnBoard, AD5252ChannelConfig::config_size_in_words>
	    ref_addresses = {
	        I2CAD5252RwRegisterOnBoard(I2CAD5252RwRegisterOnAD5252Channel::rdac_volatile, coord)};
	std::array<
	    fisch::vx::word_access_type::I2CAD5252RwRegister, AD5252ChannelConfig::config_size_in_words>
	    ref_data = {fisch::vx::word_access_type::I2CAD5252RwRegister(0x70)};

	{ // test write_addresses
		std::vector<I2CAD5252RwRegisterOnBoard> write_addresses;
		visit_preorder(
		    config, coord.toAD5252ChannelConfigOnBoard(),
		    stadls::WriteAddressVisitor<std::vector<I2CAD5252RwRegisterOnBoard>>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // test data encoding
		std::vector<fisch::vx::word_access_type::I2CAD5252RwRegister> data;
		visit_preorder(
		    config, coord.toAD5252ChannelConfigOnBoard(),
		    stadls::EncodeVisitor<std::vector<fisch::vx::word_access_type::I2CAD5252RwRegister>>{
		        data});
		EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
	}

	{ // test data decoding
		AD5252ChannelConfig config_decoded;
		visit_preorder(
		    config_decoded, coord.toAD5252ChannelConfigOnBoard(),
		    stadls::DecodeVisitor<std::array<
		        fisch::vx::word_access_type::I2CAD5252RwRegister,
		        AD5252ChannelConfig::config_size_in_words>>{ref_data});
		EXPECT_EQ(config, config_decoded);
	}
}

TEST(AD5252ChannelConfig, CerealizeCoverage)
{
	AD5252ChannelConfig conf1, conf2;

	// set test config values
	conf1.set_value(AD5252ChannelConfig::WiperSetting(0x70));

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

TEST(AD5252ChannelConfigPersistent, General)
{
	// test getter/setter
	AD5252ChannelConfigPersistent config;

	// test getter/setter
	{
		auto value_changed = AD5252ChannelConfigPersistent::WiperSetting(config.get_value() + 0x10);
		config.set_value(value_changed);

		EXPECT_EQ(config.get_value(), value_changed);
	}

	// test comparison operators
	{
		AD5252ChannelConfigPersistent config_equal = config;
		AD5252ChannelConfigPersistent config_default;

		ASSERT_EQ(config, config_equal);
		ASSERT_FALSE(config == config_default);

		ASSERT_NE(config, config_default);
		ASSERT_TRUE(config != config_default);
	}
}

TEST(AD5252ChannelConfigPersistent, EncodeDecode)
{
	AD5252ChannelConfigPersistent config;

	config.set_value(AD5252ChannelConfigPersistent::WiperSetting(0x70));

	AD5252ChannelOnBoard coord;

	// Generate reference addresses and data
	std::array<I2CAD5252RwRegisterOnBoard, AD5252ChannelConfigPersistent::config_size_in_words>
	    ref_addresses = {I2CAD5252RwRegisterOnBoard(
	        I2CAD5252RwRegisterOnAD5252Channel::eemem_persistent, coord)};
	std::array<
	    fisch::vx::word_access_type::I2CAD5252RwRegister,
	    AD5252ChannelConfigPersistent::config_size_in_words>
	    ref_data = {fisch::vx::word_access_type::I2CAD5252RwRegister(0x70)};

	{ // test write_addresses
		std::vector<I2CAD5252RwRegisterOnBoard> write_addresses;
		visit_preorder(
		    config, coord.toAD5252ChannelConfigPersistentOnBoard(),
		    stadls::WriteAddressVisitor<std::vector<I2CAD5252RwRegisterOnBoard>>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // test data encoding
		std::vector<fisch::vx::word_access_type::I2CAD5252RwRegister> data;
		visit_preorder(
		    config, coord.toAD5252ChannelConfigPersistentOnBoard(),
		    stadls::EncodeVisitor<std::vector<fisch::vx::word_access_type::I2CAD5252RwRegister>>{
		        data});
		EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
	}

	{ // test data decoding
		AD5252ChannelConfigPersistent config_decoded;
		visit_preorder(
		    config_decoded, coord.toAD5252ChannelConfigPersistentOnBoard(),
		    stadls::DecodeVisitor<std::array<
		        fisch::vx::word_access_type::I2CAD5252RwRegister,
		        AD5252ChannelConfigPersistent::config_size_in_words>>{ref_data});
		EXPECT_EQ(config, config_decoded);
	}
}

TEST(AD5252ChannelConfigPersistent, CerealizeCoverage)
{
	AD5252ChannelConfigPersistent conf1, conf2;

	// set test config values
	conf1.set_value(AD5252ChannelConfigPersistent::WiperSetting(0x70));

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
