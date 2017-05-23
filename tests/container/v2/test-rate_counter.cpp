#include <gtest/gtest.h>

#include "haldls/container/v2/rate_counter.h"
#include "halco/common/iter_all.h"

using namespace haldls::container::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;

TEST(RateCounter, General)
{
	RateCounter rate;
	EXPECT_ANY_THROW(RateCounter::Count(1024));
	EXPECT_NO_THROW(RateCounter::Count(1023));

	// test default constructor
	for (auto neuron : iter_all<NeuronOnDLS>()) {
		EXPECT_EQ(RateCounter::Count(0), rate.get_count(neuron)) << neuron.value();
		EXPECT_FALSE(rate.get_neuron_enable(neuron)) << neuron.value();
	}
	EXPECT_EQ(rate.get_fire_interrupt(), false);
	EXPECT_EQ(rate.get_clear_on_read(), false);

	// test getter/setter
	ASSERT_EQ(rate.get_count(NeuronOnDLS(2)), RateCounter::Count(0));

	ASSERT_FALSE(rate.get_neuron_enable(NeuronOnDLS(5)));
	rate.set_neuron_enable(NeuronOnDLS(5), true);
	ASSERT_TRUE(rate.get_neuron_enable(NeuronOnDLS(5)));

	ASSERT_FALSE(rate.get_fire_interrupt());
	rate.set_fire_interrupt(true);
	ASSERT_TRUE(rate.get_fire_interrupt());

	ASSERT_FALSE(rate.get_clear_on_read());
	rate.set_clear_on_read(true);
	ASSERT_TRUE(rate.get_clear_on_read());

	RateCounter rate2 = rate;
	RateCounter rate3;
	rate3.set_neuron_enable(NeuronOnDLS(14), true);

	ASSERT_EQ(rate, rate2);
	ASSERT_FALSE(rate == rate3);

	ASSERT_NE(rate, rate3);
	ASSERT_FALSE(rate != rate2);
}
