#include <gtest/gtest.h>

#include "haldls/container/v2/correlation.h"
#include "halco/common/iter_all.h"

using namespace haldls::container::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;

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
