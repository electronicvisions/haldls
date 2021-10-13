#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/vx/routing_crossbar.h"

#include "fisch/vx/jtag.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/omnibus_constants.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<fisch::vx::OmnibusChipOverJTAG> words_type;

TEST(CrossbarOutputConfig, General)
{
	CrossbarOutputConfig config;

	// test getter/setter
	{
		auto value = config.get_enable_slow();
		for (auto l2out : iter_all<CrossbarL2OutputOnDLS>()) {
			value[l2out] = !value[l2out];
		}
		config.set_enable_slow(value);
		EXPECT_EQ(config.get_enable_slow(), value);
	}

	{
		auto value = config.get_enable_event_counter();
		for (auto out : iter_all<CrossbarOutputOnDLS>()) {
			value[out] = !value[out];
		}
		config.set_enable_event_counter(value);
		EXPECT_EQ(config.get_enable_event_counter(), value);
	}

	CrossbarOutputConfig config_eq = config;
	CrossbarOutputConfig config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(CrossbarOutputConfig, CerealizeCoverage)
{
	CrossbarOutputConfig c1, c2;
	{
		auto value = c1.get_enable_slow();
		for (auto l2out : iter_all<CrossbarL2OutputOnDLS>()) {
			value[l2out] = !value[l2out];
		}
		c1.set_enable_slow(value);
		EXPECT_EQ(c1.get_enable_slow(), value);
	}

	{
		auto value = c1.get_enable_event_counter();
		for (auto out : iter_all<CrossbarOutputOnDLS>()) {
			value[out] = !value[out];
		}
		c1.set_enable_event_counter(value);
		EXPECT_EQ(c1.get_enable_event_counter(), value);
	}

	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(c1);
	}
	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(c2);
	}
	ASSERT_EQ(c1, c2);
}

TEST(CrossbarOutputConfig, EncodeDecode)
{
	CrossbarOutputConfig config;
	{
		auto v = config.get_enable_event_counter();
		v[CrossbarOutputOnDLS(7)] = true;
		config.set_enable_event_counter(v);
	}
	{
		auto v = config.get_enable_slow();
		v[CrossbarL2OutputOnDLS(2)] = true;
		config.set_enable_slow(v);
	}

	CrossbarOutputConfigOnDLS coord;

	std::array<
	    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
	    CrossbarOutputConfig::config_size_in_words>
	    ref_addresses = {
	        halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{crossbar_out_mux_base_address}};
	std::array<fisch::vx::OmnibusChipOverJTAG, CrossbarOutputConfig::config_size_in_words>
	    ref_data = {fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusChipOverJTAG::Value(
	        (1ul << 2) | (1ul << (7 + CrossbarL2OutputOnDLS::size))))};

	{ // write addresses
		addresses_type addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{addresses});
		EXPECT_THAT(addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	CrossbarOutputConfig copy;
	ASSERT_NE(config, copy);
	visit_preorder(copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, copy);
}

TEST(CrossbarInputDropCounter, General)
{
	test_generic_functionality_single_value<CrossbarInputDropCounter>();
}

TEST(CrossbarInputDropCounter, CerealizeCoverage)
{
	test_non_default_cerealization_single_value<CrossbarInputDropCounter>();
}

TEST(CrossbarInputDropCounter, EncodeDecode)
{
	CrossbarInputDropCounter config;
	typename CrossbarInputDropCounter::Value val =
	    draw_ranged_non_default_value<typename CrossbarInputDropCounter::Value>();

	config.set_value(val);

	CrossbarInputOnDLS coord(Enum(5));

	std::array<
	    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
	    CrossbarInputDropCounter::read_config_size_in_words>
	    ref_addresses = {halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{
	        crossbar_input_drop_counter_base_address + coord.toEnum()}};
	std::array<fisch::vx::OmnibusChipOverJTAG, CrossbarInputDropCounter::read_config_size_in_words>
	    ref_data = {fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusChipOverJTAG::Value(val))};

	{ // read addresses
		addresses_type addresses;
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{addresses});
		EXPECT_THAT(addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	data.push_back(ref_data.at(0));

	CrossbarInputDropCounter config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(CrossbarOutputEventCounter, General)
{
	test_generic_functionality_single_value<CrossbarOutputEventCounter>();
}

TEST(CrossbarOutputEventCounter, CerealizeCoverage)
{
	test_non_default_cerealization_single_value<CrossbarOutputEventCounter>();
}

TEST(CrossbarOutputEventCounter, EncodeDecode)
{
	CrossbarOutputEventCounter config;
	typename CrossbarOutputEventCounter::Value val =
	    draw_ranged_non_default_value<typename CrossbarOutputEventCounter::Value>();

	config.set_value(val);

	CrossbarOutputOnDLS coord(Enum(5));

	std::array<
	    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
	    CrossbarOutputEventCounter::read_config_size_in_words>
	    ref_addresses = {halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{
	        crossbar_output_event_counter_base_address + coord.toEnum()}};
	std::array<
	    fisch::vx::OmnibusChipOverJTAG, CrossbarOutputEventCounter::read_config_size_in_words>
	    ref_data = {fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusChipOverJTAG::Value(val))};

	{ // read addresses
		addresses_type addresses;
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{addresses});
		EXPECT_THAT(addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	data.push_back(ref_data.at(0));

	CrossbarOutputEventCounter config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(CrossbarNode, General)
{
	CrossbarNode config;

	// test getter/setter
	{
		bool value = !config.get_enable_drop_counter();
		config.set_enable_drop_counter(value);
		EXPECT_EQ(config.get_enable_drop_counter(), value);
	}

	{
		auto value = draw_ranged_non_default_value<decltype(config.get_mask())>(config.get_mask());
		config.set_mask(value);
		EXPECT_EQ(config.get_mask(), value);
	}

	{
		auto value =
		    draw_ranged_non_default_value<decltype(config.get_target())>(config.get_target());
		config.set_target(value);
		EXPECT_EQ(config.get_target(), value);
	}

	CrossbarNode config_eq = config;
	CrossbarNode config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(CrossbarNode, CerealizeCoverage)
{
	CrossbarNode c1, c2;
	c1.set_enable_drop_counter(!c1.get_enable_drop_counter());
	c1.set_mask(draw_ranged_non_default_value<decltype(c1.get_mask())>(c1.get_mask()));
	c1.set_target(draw_ranged_non_default_value<decltype(c1.get_target())>(c1.get_target()));

	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(c1);
	}
	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(c2);
	}
	ASSERT_EQ(c1, c2);
}

TEST(CrossbarNode, EncodeDecode)
{
	CrossbarNode config;
	config.set_enable_drop_counter(true);
	config.set_mask(NeuronLabel(8));
	config.set_target(NeuronLabel(9));

	CrossbarNodeOnDLS coord(CrossbarOutputOnDLS(4), CrossbarInputOnDLS(4));

	std::array<
	    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, CrossbarNode::config_size_in_words>
	    ref_addresses = {halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{
	        crossbar_node_base_address + coord.toEnum()}};
	std::array<fisch::vx::OmnibusChipOverJTAG, CrossbarNode::config_size_in_words> ref_data = {
	    fisch::vx::OmnibusChipOverJTAG(
	        fisch::vx::OmnibusChipOverJTAG::Value(8ul | (9ul << 16) | (1ul << 31)))};

	{ // write addresses
		addresses_type addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{addresses});
		EXPECT_THAT(addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	CrossbarNode copy;
	ASSERT_NE(config, copy);
	visit_preorder(copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, copy);
}
