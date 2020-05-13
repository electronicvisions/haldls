#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/jtag.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/v1/background.h"
#include "haldls/vx/v1/common.h"
#include "haldls/vx/v1/omnibus_constants.h"
#include "test-helper.h"

using namespace haldls::vx::v1;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<fisch::vx::OmnibusChipOverJTAG> words_type;

TEST(BackgroundSpikeSource, General)
{
	BackgroundSpikeSource config;

	{
		auto value = !config.get_enable();
		config.set_enable(value);
		EXPECT_EQ(config.get_enable(), value);
	}

	{
		auto value = !config.get_enable_random();
		config.set_enable_random(value);
		EXPECT_EQ(config.get_enable_random(), value);
	}

	{
		auto value =
		    draw_ranged_non_default_value<decltype(config.get_period())>(config.get_period());
		config.set_period(value);
		EXPECT_EQ(config.get_period(), value);
	}

	{
		auto value = draw_ranged_non_default_value<decltype(config.get_rate())>(config.get_rate());
		config.set_rate(value);
		EXPECT_EQ(config.get_rate(), value);
	}

	{
		auto value = draw_ranged_non_default_value<decltype(config.get_seed())>(config.get_seed());
		config.set_seed(value);
		EXPECT_EQ(config.get_seed(), value);
	}

	{
		auto value = draw_ranged_non_default_value<decltype(config.get_mask())>(config.get_mask());
		config.set_mask(value);
		EXPECT_EQ(config.get_mask(), value);
	}

	{
		auto value = draw_ranged_non_default_value<decltype(config.get_neuron_label())>(
		    config.get_neuron_label());
		config.set_neuron_label(value);
		EXPECT_EQ(config.get_neuron_label(), value);
	}

	BackgroundSpikeSource default_config;
	ASSERT_NE(config, default_config);
	ASSERT_TRUE(config != default_config);
	ASSERT_FALSE(config == default_config);
	default_config = config;
	ASSERT_EQ(config, default_config);
	ASSERT_TRUE(config == default_config);
	ASSERT_FALSE(config != default_config);
}

TEST(BackgroundSpikeSource, EncodeDecode)
{
	BackgroundSpikeSource config;
	config.set_enable(true);
	config.set_enable_random(false);
	config.set_period(BackgroundSpikeSource::Period(12));
	config.set_rate(BackgroundSpikeSource::Rate(13));
	config.set_seed(BackgroundSpikeSource::Seed(14));
	config.set_mask(BackgroundSpikeSource::Mask(15));
	config.set_neuron_label(halco::hicann_dls::vx::NeuronLabel(16));

	auto coord = typename BackgroundSpikeSource::coordinate_type(3);

	std::array<OmnibusChipOverJTAGAddress, BackgroundSpikeSource::config_size_in_words>
	    ref_addresses = {OmnibusChipOverJTAGAddress{0x130001 + coord.toEnum() * 3 + 1},
	                     OmnibusChipOverJTAGAddress{0x130001 + coord.toEnum() * 3 + 2},
	                     OmnibusChipOverJTAGAddress{0x130001 + coord.toEnum() * 3 + 0}};

	{ // check if write addresses are correct
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// Encode
	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_EQ(data[0].get(), 14ul);                              // seed
	EXPECT_EQ(data[1].get(), 15ul | (13ul << 8) | (16ul << 16)); // rate, mask, neuron_label
	EXPECT_EQ(data[2].get(), 1ul | (12ul << 16)); // period, enable, enable_random, written last

	BackgroundSpikeSource copy;
	ASSERT_NE(config, copy);
	visit_preorder(copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, copy);
}

TEST(BackgroundSpikeSource, CerealizeCoverage)
{
	BackgroundSpikeSource c1, c2;
	c1.set_enable(!c1.get_enable());
	c1.set_enable_random(!c1.get_enable_random());
	c1.set_period(draw_ranged_non_default_value<decltype(c1.get_period())>(c1.get_period()));
	c1.set_rate(draw_ranged_non_default_value<decltype(c1.get_rate())>(c1.get_rate()));
	c1.set_seed(draw_ranged_non_default_value<decltype(c1.get_seed())>(c1.get_seed()));
	c1.set_mask(draw_ranged_non_default_value<decltype(c1.get_mask())>(c1.get_mask()));
	c1.set_neuron_label(
	    draw_ranged_non_default_value<decltype(c1.get_neuron_label())>(c1.get_neuron_label()));

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
