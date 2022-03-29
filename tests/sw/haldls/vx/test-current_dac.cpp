#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/word_access/type/jtag.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/current_dac.h"
#include "haldls/vx/omnibus_constants.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<fisch::vx::word_access_type::OmnibusChipOverJTAG> words_type;

TEST(CurrentDAC, General)
{
	CurrentDAC config;

	{
		auto const value = draw_non_default_value<CurrentDAC::Current>(config.get_current());
		config.set_current(value);
		EXPECT_EQ(config.get_current(), value);
	}

	{
		EXPECT_EQ(config.get_sign(), CurrentDAC::Sign::source);
		config.set_sign(CurrentDAC::Sign::sink);
		EXPECT_EQ(config.get_sign(), CurrentDAC::Sign::sink);
	}

	{
		auto member = config.get_enable_current();
		member = !member;
		config.set_enable_current(member);
		EXPECT_EQ(config.get_enable_current(), member);
	}

	{
		auto member = config.get_connect_neuron();
		for (auto& connect : member) {
			connect = !connect;
		}
		config.set_connect_neuron(member);
		EXPECT_EQ(config.get_connect_neuron(), member);
	}

	{
		auto member = config.get_connect_synapse();
		for (auto& connect : member) {
			connect = !connect;
		}
		config.set_connect_synapse(member);
		EXPECT_EQ(config.get_connect_synapse(), member);
	}


	CurrentDAC default_config;

	ASSERT_NE(config, default_config);
	ASSERT_TRUE(config != default_config);
	ASSERT_FALSE(config == default_config);

	default_config = config;
	ASSERT_EQ(config, default_config);
	ASSERT_TRUE(config == default_config);
	ASSERT_FALSE(config != default_config);
}

TEST(CurrentDAC, EncodeDecode)
{
	CurrentDAC config;

	auto coord = typename CurrentDAC::coordinate_type();

	std::array<OmnibusChipOverJTAGAddress, CurrentDAC::config_size_in_words> ref_addresses = {
	    OmnibusChipOverJTAGAddress{0xc0000 + 11}};

	{ // check if write addresses are correct
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// Encode default
	words_type default_data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{default_data});
	EXPECT_EQ(default_data[0], 0x0);

	config.set_sign(CurrentDAC::Sign::sink);
	config.set_current(CurrentDAC::Current(0xA7));

	config.set_connect_neuron({true, false});
	config.set_connect_synapse({false, true});

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_EQ(data[0], 0x4B67);

	// Decode
	CurrentDAC decoded;
	ASSERT_NE(config, decoded);
	visit_preorder(decoded, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, decoded);
}

TEST(CurrentDAC, CerealizeCoverage)
{
	CurrentDAC config, c2;

	{
		auto const value = draw_non_default_value<CurrentDAC::Current>(config.get_current());
		config.set_current(value);
	}

	{
		auto member = config.get_enable_current();
		member = !member;
		config.set_enable_current(member);
	}

	{
		config.set_sign(CurrentDAC::Sign::sink);
	}

	{
		auto member = config.get_connect_neuron();
		for (auto& connect : member) {
			connect = !connect;
		}
		config.set_connect_neuron(member);
	}

	{
		auto member = config.get_connect_synapse();
		for (auto& connect : member) {
			connect = !connect;
		}
		config.set_connect_synapse(member);
	}

	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(config);
	}

	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(c2);
	}
	ASSERT_EQ(config, c2);
}
