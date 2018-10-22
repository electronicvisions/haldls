#include <random>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "halco/common/iter_all.h"
#include "haldls/v2/rate_counter.h"
#include "stadls/visitors.h"

using namespace haldls::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;

typedef std::vector<hardware_address_type> addresses_type;
typedef std::vector<hardware_word_type> words_type;

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

TEST(RateCounter, EncodeDecode)
{
	RateCounter config;

	for (auto neuron : iter_all<NeuronOnDLS>()) {
		config.set_neuron_enable(neuron, (neuron % 2) == 0);
	}
	config.set_fire_interrupt(false);
	config.set_clear_on_read(true);

	RateCounterOnDLS coord;

	// clang-format off
	std::array<hardware_address_type, 34> ref_addresses = {{0x1e000000, 0x1e000001, 0x1e000002, 0x1e000003, 0x1e000004, 0x1e000005, 0x1e000006, 0x1e000007, 0x1e000008, 0x1e000009, 0x1e00000a, 0x1e00000b, 0x1e00000c, 0x1e00000d, 0x1e00000e, 0x1e00000f, 0x1e000010, 0x1e000011, 0x1e000012, 0x1e000013, 0x1e000014, 0x1e000015, 0x1e000016, 0x1e000017, 0x1e000018, 0x1e000019, 0x1e00001a, 0x1e00001b, 0x1e00001c, 0x1e00001d, 0x1e00001e, 0x1e00001f, 0x1e000020, 0x1e000021}};
	// clang-format on
	std::array<hardware_word_type, 34> ref_data{{}};
	ref_data.at(32) = 0b0101'0101'0101'0101'0101'0101'0101'0101ul;
	ref_data.at(33) = 0b10ul;

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

	RateCounter config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);

	data = words_type(ref_data.begin(), ref_data.end());
	for (auto neuron : iter_all<NeuronOnDLS>()) {
		if ((neuron % 2) == 0)
			data.at(neuron.value()) = 100 + neuron.value();
	}

	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});

	for (auto neuron : iter_all<NeuronOnDLS>()) {
		EXPECT_EQ((neuron % 2) == 0 ? (100 + neuron.value()) : 0, config_copy.get_count(neuron).value());
	}
}

TEST(RateCounter, CerealizeCoverage)
{
	RateCounter obj1,obj2;
	std::array<hardware_word_type, RateCounter::config_size_in_words> data;
	for (auto& word: data) {
		word = static_cast<hardware_word_type>(rand());
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
