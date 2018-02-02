#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "halco/common/iter_all.h"
#include "haldls/container/v2/correlation.h"
#include "stadls/visitors.h"

using namespace haldls::container::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;

typedef std::vector<hardware_address_type> addresses_type;
typedef std::vector<hardware_word_type> words_type;

TEST(CorrelationConfig, General)
{
	EXPECT_ANY_THROW(CorrelationConfig::Delay(4096));
	EXPECT_NO_THROW(CorrelationConfig::Delay(4095));

	CorrelationConfig config;
	ASSERT_EQ(config.get_sense_delay(), CorrelationConfig::Delay(0));
	ASSERT_EQ(config.get_reset_delay_1(), CorrelationConfig::Delay(0));
	ASSERT_EQ(config.get_reset_delay_2(), CorrelationConfig::Delay(0));

	// test getter/setter
	config.set_sense_delay(CorrelationConfig::Delay(12));
	ASSERT_EQ(config.get_sense_delay(), CorrelationConfig::Delay(12));

	config.set_reset_delay_1(CorrelationConfig::Delay(42));
	ASSERT_EQ(config.get_reset_delay_1(), CorrelationConfig::Delay(42));

	config.set_reset_delay_2(CorrelationConfig::Delay(69));
	ASSERT_EQ(config.get_reset_delay_2(), CorrelationConfig::Delay(69));


	CorrelationConfig config2 = config;
	CorrelationConfig config3 = config;
	config3.set_reset_delay_2(CorrelationConfig::Delay(78));


	ASSERT_EQ(config, config2);
	ASSERT_EQ(config == config3, false);

	ASSERT_NE(config, config3);
	ASSERT_EQ(config != config2, false);
}

TEST(CorrelationConfig, EncodeDecode)
{
	CorrelationConfig config;

	config.set_sense_delay(CorrelationConfig::Delay(12));
	config.set_reset_delay_1(CorrelationConfig::Delay(42));
	config.set_reset_delay_2(CorrelationConfig::Delay(69));

	Unique coord;

	std::array<hardware_address_type, 3> ref_addresses = {{0x0c000000, 0x0C000001, 0x0C000002}};
	std::array<hardware_word_type, 3> ref_data{{12, 42, 69}};

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(
			config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		addresses_type read_addresses;
		visit_preorder(
			config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	CorrelationConfig config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}
