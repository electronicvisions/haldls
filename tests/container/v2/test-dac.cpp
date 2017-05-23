#include <gtest/gtest.h>

#include "haldls/container/v2/dac.h"
#include "halco/common/iter_all.h"

using namespace haldls::container::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;

TEST(DAC, General)
{
	EXPECT_ANY_THROW(DAC::Value(4096));
	EXPECT_NO_THROW(DAC::Value(4095));
	EXPECT_ANY_THROW(DAC::Channel(8));
	EXPECT_NO_THROW(DAC::Channel(7));

	DAC dac;
	for (auto channel_it : halco::common::iter_all<DAC::Channel>()) {
		ASSERT_EQ(dac.get(channel_it), DAC::Value(0));
	}

	// test getter/setter
	dac.set(DAC::Channel(2), DAC::Value(42));
	ASSERT_EQ(dac.get(DAC::Channel(2)), DAC::Value(42));

	DAC dac2 = dac;
	DAC dac3;
	dac3.set(DAC::Channel(5), DAC::Value(2345));

	ASSERT_EQ(dac, dac2);
	ASSERT_EQ(dac == dac3, false);

	ASSERT_NE(dac, dac3);
	ASSERT_EQ(dac != dac2, false);
}
