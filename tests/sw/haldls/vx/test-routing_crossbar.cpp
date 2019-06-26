#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/vx/routing_crossbar.h"

#include "fisch/vx/jtag.h"
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
	for (auto l2out : iter_all<CrossbarL2OutputOnDLS>()) {
		bool value = !config.get_enable_slow(l2out);
		config.set_enable_slow(l2out, value);
		EXPECT_EQ(config.get_enable_slow(l2out), value);
	}

	for (auto out : iter_all<CrossbarOutputOnDLS>()) {
		bool value = !config.get_enable_event_counter(out);
		config.set_enable_event_counter(out, value);
		EXPECT_EQ(config.get_enable_event_counter(out), value);
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
	for (auto l2out : iter_all<CrossbarL2OutputOnDLS>()) {
		bool value = !c1.get_enable_slow(l2out);
		c1.set_enable_slow(l2out, value);
	}

	for (auto out : iter_all<CrossbarOutputOnDLS>()) {
		bool value = !c1.get_enable_event_counter(out);
		c1.set_enable_event_counter(out, value);
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
	config.set_enable_event_counter(CrossbarOutputOnDLS(7), true);
	config.set_enable_slow(CrossbarL2OutputOnDLS(2), true);

	CrossbarOutputConfigOnDLS coord;

	std::array<
	    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
	    CrossbarOutputConfig::write_config_size_in_words>
	    ref_addresses = {
	        halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{crossbar_out_mux_base_address}};
	std::array<fisch::vx::OmnibusChipOverJTAG, CrossbarOutputConfig::write_config_size_in_words>
	    ref_data = {static_cast<fisch::vx::OmnibusData>(
	        (1ul << 2) | (1ul << (7 + CrossbarL2OutputOnDLS::size)))};

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
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
	    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, CrossbarNode::write_config_size_in_words>
	    ref_addresses = {halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{
	        crossbar_node_base_address + coord.toEnum()}};
	std::array<fisch::vx::OmnibusChipOverJTAG, CrossbarNode::write_config_size_in_words> ref_data =
	    {static_cast<fisch::vx::OmnibusData>(8ul | (9ul << 16) | (1ul << 31))};

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
}
