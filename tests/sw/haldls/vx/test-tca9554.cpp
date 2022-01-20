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

TEST(TCA9554Config, General)
{
	// test getter/setter
	TCA9554Config config;

	// test getter/setter
	{
		auto channel_modes = config.get_channel_mode();
		auto channel_polarities = config.get_channel_polarity();
		auto channel_outputs = config.get_channel_output();

		TCA9554Config::ChannelsModeArray channel_modes_inverted{};
		TCA9554Config::ChannelsPolarityArray channel_polarities_inverted{};
		TCA9554Config::ChannelsBooleanArray channel_outputs_inverted{};

		for (auto coord : halco::common::iter_all<halco::hicann_dls::vx::TCA9554ChannelOnBoard>()) {
			channel_modes_inverted[coord] =
			    static_cast<TCA9554Config::ChannelMode>(!static_cast<bool>(channel_modes[coord]));
			channel_polarities_inverted[coord] = static_cast<TCA9554Config::ChannelPolarity>(
			    !static_cast<bool>(channel_polarities[coord]));
			channel_outputs_inverted[coord] = !channel_outputs[coord];
		}

		config.set_channel_mode(channel_modes_inverted);
		config.set_channel_polarity(channel_polarities_inverted);
		config.set_channel_output(channel_outputs_inverted);

		EXPECT_EQ(config.get_channel_mode(), channel_modes_inverted);
		EXPECT_EQ(config.get_channel_polarity(), channel_polarities_inverted);
		EXPECT_EQ(config.get_channel_output(), channel_outputs_inverted);
	}

	// test comparison operators
	{
		TCA9554Config config_equal = config;
		TCA9554Config config_default;

		ASSERT_EQ(config, config_equal);
		ASSERT_FALSE(config == config_default);

		ASSERT_NE(config, config_default);
		ASSERT_TRUE(config != config_default);
	}
}

TEST(TCA9554Config, EncodeDecode)
{
	TCA9554Config config;

	// set test config values
	{
		auto channel_polarities = config.get_channel_polarity();
		auto channel_outputs = config.get_channel_output();

		for (auto coord : halco::common::iter_all<halco::hicann_dls::vx::TCA9554ChannelOnBoard>()) {
			channel_outputs[coord] = coord.toEnum() % 2 != 0;
			channel_polarities[coord] =
			    static_cast<TCA9554Config::ChannelPolarity>(coord.toEnum() % 2 == 0);
		}

		config.set_channel_polarity(channel_polarities);
		config.set_channel_output(channel_outputs);
	}

	TCA9554OnBoard coord;

	// Generate reference addresses and data
	std::array<I2CTCA9554RwRegisterOnBoard, TCA9554Config::config_size_in_words> ref_addresses = {
	    I2CTCA9554RwRegisterOnBoard(I2CTCA9554RwRegisterOnTCA9554::outputs, coord),
	    I2CTCA9554RwRegisterOnBoard(I2CTCA9554RwRegisterOnTCA9554::polarity, coord),
	    I2CTCA9554RwRegisterOnBoard(I2CTCA9554RwRegisterOnTCA9554::config, coord)};
	std::array<
	    fisch::vx::word_access_type::I2CTCA9554RwRegister,
	    TCA9554Config::config_size_in_words> const ref_data = {
	    fisch::vx::word_access_type::I2CTCA9554RwRegister(0b10101010),
	    fisch::vx::word_access_type::I2CTCA9554RwRegister(0b01010101),
	    fisch::vx::word_access_type::I2CTCA9554RwRegister(0b00000000)};

	{ // test write_addresses
		std::vector<I2CTCA9554RwRegisterOnBoard> write_addresses;
		visit_preorder(
		    config, coord.toTCA9554ConfigOnBoard(),
		    stadls::WriteAddressVisitor<std::vector<I2CTCA9554RwRegisterOnBoard>>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // test read_addresses
		std::vector<I2CTCA9554RwRegisterOnBoard> read_addresses;
		visit_preorder(
		    config, coord.toTCA9554ConfigOnBoard(),
		    stadls::ReadAddressVisitor<std::vector<I2CTCA9554RwRegisterOnBoard>>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // test data encoding
		std::vector<fisch::vx::word_access_type::I2CTCA9554RwRegister> data;
		visit_preorder(
		    config, coord.toTCA9554ConfigOnBoard(),
		    stadls::EncodeVisitor<std::vector<fisch::vx::word_access_type::I2CTCA9554RwRegister>>{
		        data});
		EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
	}

	{ // test data decoding
		TCA9554Config config_decoded;
		visit_preorder(
		    config_decoded, coord.toTCA9554ConfigOnBoard(),
		    stadls::DecodeVisitor<std::array<
		        fisch::vx::word_access_type::I2CTCA9554RwRegister,
		        TCA9554Config::config_size_in_words>>{ref_data});
		EXPECT_EQ(config, config_decoded);
	}
}

TEST(TCA9554Config, CerealizeCoverage)
{
	TCA9554Config conf1, conf2;

	// set test config values
	{
		TCA9554Config::ChannelsPolarityArray channel_polarities = conf1.get_channel_polarity();
		TCA9554Config::ChannelsBooleanArray channel_outputs = conf1.get_channel_output();

		for (auto coord : halco::common::iter_all<halco::hicann_dls::vx::TCA9554ChannelOnBoard>()) {
			channel_outputs[coord] = coord.toEnum() % 2 != 0;
			channel_polarities[coord] =
			    static_cast<TCA9554Config::ChannelPolarity>(coord.toEnum() % 2 == 0);
		}

		conf1.set_channel_polarity(channel_polarities);
		conf1.set_channel_output(channel_outputs);
	}

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


TEST(TCA9554Inputs, General)
{
	// test getter/setter
	TCA9554Inputs config;

	// test getter/setter
	{
		auto channel_inputs = config.get_channel_input();

		TCA9554Inputs::ChannelsBooleanArray channel_inputs_inverted{};

		for (auto coord : halco::common::iter_all<halco::hicann_dls::vx::TCA9554ChannelOnBoard>()) {
			channel_inputs_inverted[coord] = !channel_inputs[coord];
		}

		config.set_channel_input(channel_inputs_inverted);

		EXPECT_EQ(config.get_channel_input(), channel_inputs_inverted);
	}

	// test comparison operators
	{
		TCA9554Inputs config_equal = config;
		TCA9554Inputs config_default;

		ASSERT_EQ(config, config_equal);
		ASSERT_FALSE(config == config_default);

		ASSERT_NE(config, config_default);
		ASSERT_TRUE(config != config_default);
	}
}

TEST(TCA9554Inputs, EncodeDecode)
{
	TCA9554Inputs config;

	// set test config values
	{
		auto channel_inputs = config.get_channel_input();

		for (auto coord : halco::common::iter_all<halco::hicann_dls::vx::TCA9554ChannelOnBoard>()) {
			channel_inputs[coord] = coord.toEnum() % 2 != 0;
		}

		config.set_channel_input(channel_inputs);
	}

	TCA9554OnBoard coord;

	// Generate reference addresses and data
	std::array<I2CTCA9554RoRegisterOnBoard, TCA9554Inputs::config_size_in_words> ref_addresses = {
	    I2CTCA9554RoRegisterOnBoard(I2CTCA9554RoRegisterOnBoard::inputs, coord)};
	std::array<
	    fisch::vx::word_access_type::I2CTCA9554RoRegister, TCA9554Inputs::config_size_in_words>
	    ref_data = {fisch::vx::word_access_type::I2CTCA9554RoRegister(0b10101010)};

	{ // test write_addresses
		std::vector<I2CTCA9554RoRegisterOnBoard> read_addresses;
		visit_preorder(
		    config, coord.toTCA9554InputsOnBoard(),
		    stadls::ReadAddressVisitor<std::vector<I2CTCA9554RoRegisterOnBoard>>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // test data encoding
		std::vector<fisch::vx::word_access_type::I2CTCA9554RoRegister> data;
		visit_preorder(
		    config, coord.toTCA9554InputsOnBoard(),
		    stadls::EncodeVisitor<std::vector<fisch::vx::word_access_type::I2CTCA9554RoRegister>>{
		        data});
		EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
	}

	{ // test data decoding
		TCA9554Inputs config_decoded;
		visit_preorder(
		    config_decoded, coord.toTCA9554InputsOnBoard(),
		    stadls::DecodeVisitor<std::array<
		        fisch::vx::word_access_type::I2CTCA9554RoRegister,
		        TCA9554Inputs::config_size_in_words>>{ref_data});
		EXPECT_EQ(config, config_decoded);
	}
}

TEST(TCA9554Inputs, CerealizeCoverage)
{
	TCA9554Inputs conf1, conf2;

	// set test config values
	{
		TCA9554Inputs::ChannelsBooleanArray channel_inputs = conf1.get_channel_input();

		for (auto coord : halco::common::iter_all<halco::hicann_dls::vx::TCA9554ChannelOnBoard>()) {
			channel_inputs[coord] = coord.toEnum() % 2 != 0;
		}

		conf1.set_channel_input(channel_inputs);
	}

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
