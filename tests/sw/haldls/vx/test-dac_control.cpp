#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/vx/spi.h"

#include "fisch/vx/spi.h"
#include "halco/hicann-dls/vx/spi.h"
#include "haldls/vx/common.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::SPIDACControlRegisterOnBoard> addresses_type;
typedef std::vector<fisch::vx::SPIDACControlRegister> words_type;

TEST(DACControl, General)
{
	DACControl config;

	// test getter/setter
	for (auto channel : iter_all<DACChannelOnDAC>()) {
		auto value = !config.get_enable_channel(channel);
		config.set_enable_channel(channel, value);
		EXPECT_EQ(config.get_enable_channel(channel), value);
	}

	DACControl config_eq = config;
	DACControl config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(DACControl, EncodeDecode)
{
	DACControl config;

	for (auto ch : iter_all<DACChannelOnDAC>()) {
		config.set_enable_channel(ch, false);
	}
	config.set_enable_channel(DACChannelOnDAC(2), true);

	auto coord = draw_ranged_non_default_value<DACOnBoard>(0);

	std::array<typename addresses_type::value_type, DACControl::config_size_in_words>
	    ref_addresses = {
	        typename addresses_type::value_type(
	            halco::hicann_dls::vx::SPIDACControlRegisterOnDAC::gain_reference, coord),
	        typename addresses_type::value_type(
	            halco::hicann_dls::vx::SPIDACControlRegisterOnDAC::power_down, coord)};
	std::array<fisch::vx::SPIDACControlRegister, DACControl::config_size_in_words> ref_data = {
	    fisch::vx::SPIDACControlRegister::Value(0b001100),
	    fisch::vx::SPIDACControlRegister::Value(0xff ^ (1u << 2))};

	{
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
}

TEST(DACControl, CerealizeCoverage)
{
	DACControl obj1, obj2;
	for (auto channel : iter_all<DACChannelOnDAC>()) {
		obj1.set_enable_channel(channel, !obj1.get_enable_channel(channel));
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
