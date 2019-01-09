#include <random>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "halco/common/iter_all.h"
#include "haldls/v2/rate_counter.h"
#include "stadls/visitors.h"

#include "test-helper.h"

using namespace haldls::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;

typedef std::vector<hardware_address_type> addresses_type;
typedef std::vector<hardware_word_type> words_type;

TEST(RateCounterConfig, General)
{
	RateCounterConfig config;

	// test default constructor
	for (auto neuron : iter_all<NeuronOnDLS>()) {
		EXPECT_FALSE(config.get_enable_neuron(neuron)) << neuron.value();
	}
	EXPECT_EQ(config.get_enable_fire_interrupt(), false);
	EXPECT_EQ(config.get_enable_clear_on_read(), false);

	ASSERT_FALSE(config.get_enable_neuron(NeuronOnDLS(5)));
	config.set_enable_neuron(NeuronOnDLS(5), true);
	ASSERT_TRUE(config.get_enable_neuron(NeuronOnDLS(5)));

	ASSERT_FALSE(config.get_enable_fire_interrupt());
	config.set_enable_fire_interrupt(true);
	ASSERT_TRUE(config.get_enable_fire_interrupt());

	ASSERT_FALSE(config.get_enable_clear_on_read());
	config.set_enable_clear_on_read(true);
	ASSERT_TRUE(config.get_enable_clear_on_read());

	RateCounterConfig config2 = config;
	RateCounterConfig config3;
	config3.set_enable_neuron(NeuronOnDLS(14), true);

	ASSERT_EQ(config, config2);
	ASSERT_FALSE(config == config3);

	ASSERT_NE(config, config3);
	ASSERT_FALSE(config != config2);
}

TEST(RateCounterConfig, EncodeDecode)
{
	RateCounterConfig config;

	for (auto neuron: iter_all<NeuronOnDLS>()) {
		config.set_enable_neuron(neuron, (neuron % 2) == 0);
	}
	config.set_enable_fire_interrupt(false);
	config.set_enable_clear_on_read(true);

	RateCounterConfigOnDLS coord;

	// clang-format off
	std::array<hardware_address_type, 2> ref_addresses = {{0x1e000020, 0x1e000021}};
	// clang-format on
	std::array<hardware_word_type, 2> ref_data{{}};
	ref_data.at(0) = 0b0101'0101'0101'0101'0101'0101'0101'0101ul;
	ref_data.at(1) = 0b10ul;

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

	RateCounterConfig config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(RateCounterConfig, CerealizeCoverage)
{
	RateCounterConfig obj1,obj2;
	for (auto neuron : iter_all<NeuronOnDLS>()) {
		obj1.set_enable_neuron(neuron, !obj1.get_enable_neuron(neuron));
	}
	obj1.set_enable_fire_interrupt(!obj1.get_enable_fire_interrupt());
	obj1.set_enable_clear_on_read(!obj1.get_enable_clear_on_read());

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

TEST(RateCounterEntry, General)
{
	RateCounterEntry entry;

	EXPECT_ANY_THROW(RateCounterEntry::Count(1024));
	EXPECT_NO_THROW(RateCounterEntry::Count(1023));

	// test default constructor
	EXPECT_EQ(RateCounterEntry::Count(0), entry.get_count());

	// test getter
	ASSERT_EQ(entry.get_count(), RateCounterEntry::Count(0));

	RateCounterEntry entry2 = entry;
	RateCounterEntry entry3;
	std::array<hardware_word_type, 1> data{1};
	entry3.decode(data);

	ASSERT_EQ(entry, entry2);
	ASSERT_FALSE(entry == entry3);

	ASSERT_NE(entry, entry3);
	ASSERT_FALSE(entry != entry2);
}

TEST(RateCounterEntry, EncodeDecode)
{

	for (auto coord: iter_all<NeuronOnDLS>()) {
		RateCounterEntry config;
		// clang-format off
		std::array<hardware_address_type, 1> ref_addresses = {{0x1e000000 + static_cast<uint32_t>(coord.value())}};
		// clang-format on

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
	}

	NeuronOnDLS coord;

	RateCounterEntry config;

	words_type write_data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{write_data});
	ASSERT_EQ(write_data, words_type{0});

	RateCounterEntry config_copy;
	words_type read_data{100};

	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(read_data)});

	EXPECT_EQ(100, config_copy.get_count().value());
}

TEST(RateCounterEntry, CerealizeCoverage)
{
	RateCounterEntry obj1,obj2;
	std::array<hardware_word_type, 1> data{1};
	obj1.decode(data);

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

TEST(RateCounter, General)
{
	RateCounter rate;
	EXPECT_ANY_THROW(RateCounter::Count(1024));
	EXPECT_NO_THROW(RateCounter::Count(1023));

	// test default constructor
	for (auto neuron : iter_all<NeuronOnDLS>()) {
		EXPECT_EQ(RateCounter::Count(0), rate.get_count(neuron)) << neuron.value();
	}

	// test getter/setter
	ASSERT_EQ(rate.get_count(NeuronOnDLS(2)), RateCounter::Count(0));

	RateCounter rate2 = rate;
	RateCounter rate3;

	words_type data;
	for (size_t i = 0; i < NeuronOnDLS::size; ++i) {
		data.push_back(draw_ranged_non_default_value<RateCounter::Count>(0).value());
	}
	visit_preorder(rate3, RateCounterOnDLS(), stadls::DecodeVisitor<words_type>{std::move(data)});

	ASSERT_EQ(rate, rate2);
	ASSERT_FALSE(rate == rate3);

	ASSERT_NE(rate, rate3);
	ASSERT_FALSE(rate != rate2);
}

TEST(RateCounter, EncodeDecode)
{
	RateCounter config;

	RateCounterOnDLS coord;

	std::array<hardware_address_type, NeuronOnDLS::size>
		ref_addresses{{}};
	for (size_t i = 0; i < NeuronOnDLS::size; ++i) {
		ref_addresses[i] = 0x1e000000 + i;
	}
	std::array<hardware_word_type, NeuronOnDLS::size> ref_read_data{{}};
	std::array<hardware_word_type, NeuronOnDLS::size> ref_write_data{{}};

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

	words_type write_data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{write_data});
	EXPECT_THAT(write_data, ::testing::ElementsAreArray(ref_write_data));

	words_type read_data(NeuronOnDLS::size);
	visit_preorder(config, coord, stadls::DecodeVisitor<words_type>{read_data});
	EXPECT_THAT(read_data, ::testing::ElementsAreArray(ref_read_data));
}

TEST(RateCounter, CerealizeCoverage)
{
	RateCounter obj1,obj2;

	words_type data;
	for (size_t i = 0; i < NeuronOnDLS::size; ++i) {
		data.push_back(draw_ranged_non_default_value<RateCounter::Count>(0).value());
	}
	visit_preorder(obj1, RateCounterOnDLS(), stadls::DecodeVisitor<words_type>{std::move(data)});

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
