#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/vx/perftest.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;

typedef std::vector<halco::hicann_dls::vx::OmnibusFPGAAddress> addresses_type;
typedef std::vector<fisch::vx::OmnibusFPGA> words_type;

TEST(PerfTest, General)
{
	PerfTest config;

	// test getter/setter
	{
		auto value = !config.get_enable();
		config.set_enable(value);
		EXPECT_EQ(config.get_enable(), value);
	}

	PerfTest config_eq = config;
	PerfTest config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(PerfTest, EncodeDecode)
{
	PerfTest config;

	config.set_enable(true);

	PerfTestOnFPGA coord;

	OmnibusFPGAAddress ref_address(0x0800'0000);
	std::array<OmnibusFPGAAddress, PerfTest::config_size_in_words> ref_addresses = {ref_address};
	std::array<fisch::vx::OmnibusFPGA, PerfTest::config_size_in_words> ref_data = {
	    fisch::vx::OmnibusFPGA(fisch::vx::OmnibusData(0x1))};

	{
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
}

TEST(PerfTest, CerealizeCoverage)
{
	PerfTest obj1, obj2;
	obj1.set_enable(!obj1.get_enable());

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


TEST(PerfTestStatus, General)
{
	PerfTestStatus config;

	// test getter/setter
	{
		auto value = draw_ranged_non_default_value<PerfTestStatus::Sent>(config.get_sent());
		config.set_sent(value);
		EXPECT_EQ(config.get_sent(), value);
	}

	{
		auto value = draw_ranged_non_default_value<PerfTestStatus::Received>(config.get_received());
		config.set_received(value);
		EXPECT_EQ(config.get_received(), value);
	}

	{
		auto value = draw_ranged_non_default_value<PerfTestStatus::InOrder>(config.get_in_order());
		config.set_in_order(value);
		EXPECT_EQ(config.get_in_order(), value);
	}

	{
		auto value = draw_ranged_non_default_value<PerfTestStatus::ErrorWord>(config.get_error_word());
		config.set_error_word(value);
		EXPECT_EQ(config.get_error_word(), value);
	}

	PerfTestStatus config_eq = config;
	PerfTestStatus config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(PerfTestStatus, EncodeDecode)
{
	PerfTestStatus config;

	config.set_sent(PerfTestStatus::Sent(0x123));
	config.set_received(PerfTestStatus::Received(0x456));
	config.set_in_order(PerfTestStatus::InOrder(0x789));
	config.set_error_word(PerfTestStatus::ErrorWord(0x010));

	PerfTestStatusOnFPGA coord;

	OmnibusFPGAAddress base_address(0x0800'0000);
	std::array<OmnibusFPGAAddress, PerfTestStatus::read_config_size_in_words> ref_addresses = {
	    OmnibusFPGAAddress(base_address + 1), OmnibusFPGAAddress(base_address + 2),
	    OmnibusFPGAAddress(base_address + 3), OmnibusFPGAAddress(base_address + 4)};

	std::array<fisch::vx::OmnibusFPGA, PerfTestStatus::read_config_size_in_words> ref_data = {
	    fisch::vx::OmnibusFPGA(fisch::vx::OmnibusData(0x123)),
	    fisch::vx::OmnibusFPGA(fisch::vx::OmnibusData(0x456)),
	    fisch::vx::OmnibusFPGA(fisch::vx::OmnibusData(0x789)),
	    fisch::vx::OmnibusFPGA(fisch::vx::OmnibusData(0x010))};

	{
		addresses_type read_addresses;
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	PerfTestStatus decoded_config;
	words_type data;
	std::copy(ref_data.begin(), ref_data.end(), std::back_inserter(data));
	visit_preorder(decoded_config, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	EXPECT_EQ(decoded_config, config);
}

TEST(PerfTestStatus, CerealizeCoverage)
{
	PerfTestStatus obj1, obj2;
	obj1.set_sent(draw_ranged_non_default_value<PerfTestStatus::Sent>(obj1.get_sent()));
	obj1.set_received(draw_ranged_non_default_value<PerfTestStatus::Received>(obj1.get_received()));
	obj1.set_in_order(draw_ranged_non_default_value<PerfTestStatus::InOrder>(obj1.get_in_order()));
	obj1.set_error_word(draw_ranged_non_default_value<PerfTestStatus::ErrorWord>(obj1.get_error_word()));

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
