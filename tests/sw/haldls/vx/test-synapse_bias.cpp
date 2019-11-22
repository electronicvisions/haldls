#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/jtag.h"
#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/synapse.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<fisch::vx::OmnibusChipOverJTAG> words_type;

TEST(SynapseBiasSelection, General)
{
	SynapseBiasSelection config;

	{
		auto member = config.get_enable_internal_dac_bias();
		for (auto coord : iter_all<CapMemBlockOnDLS>()) {
			member[coord] = !member[coord];
		}
		config.set_enable_internal_dac_bias(member);
		EXPECT_EQ(config.get_enable_internal_dac_bias(), member);
	}

	{
		auto member = config.get_enable_internal_ramp_bias();
		for (auto coord : iter_all<CapMemBlockOnDLS>()) {
			member[coord] = !member[coord];
		}
		config.set_enable_internal_ramp_bias(member);
		EXPECT_EQ(config.get_enable_internal_ramp_bias(), member);
	}

	{
		auto member = config.get_enable_internal_store_bias();
		for (auto coord : iter_all<CapMemBlockOnDLS>()) {
			member[coord] = !member[coord];
		}
		config.set_enable_internal_store_bias(member);
		EXPECT_EQ(config.get_enable_internal_store_bias(), member);
	}

	{
		auto member = config.get_enable_internal_output_bias();
		for (auto coord : iter_all<CapMemBlockOnDLS>()) {
			member[coord] = !member[coord];
		}
		config.set_enable_internal_output_bias(member);
		EXPECT_EQ(config.get_enable_internal_output_bias(), member);
	}

	SynapseBiasSelection default_config;

	ASSERT_NE(config, default_config);
	ASSERT_TRUE(config != default_config);
	ASSERT_FALSE(config == default_config);

	default_config = config;
	ASSERT_EQ(config, default_config);
	ASSERT_TRUE(config == default_config);
	ASSERT_FALSE(config != default_config);
}

TEST(SynapseBiasSelection, EncodeDecode)
{
	SynapseBiasSelection config;

	auto coord = typename SynapseBiasSelection::coordinate_type();

	std::array<OmnibusChipOverJTAGAddress, SynapseBiasSelection::write_config_size_in_words>
	    ref_addresses = {OmnibusChipOverJTAGAddress{0x100000}};

	{ // check if write addresses are correct
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// Encode
	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_EQ(data[0].get(), 0xffff);
}

TEST(SynapseBiasSelection, CerealizeCoverage)
{
	SynapseBiasSelection c1, c2;

	{
		auto member = c1.get_enable_internal_dac_bias();
		for (auto coord : iter_all<CapMemBlockOnDLS>()) {
			member[coord] = !member[coord];
		}
		c1.set_enable_internal_dac_bias(member);
	}

	{
		auto member = c1.get_enable_internal_ramp_bias();
		for (auto coord : iter_all<CapMemBlockOnDLS>()) {
			member[coord] = !member[coord];
		}
		c1.set_enable_internal_ramp_bias(member);
	}

	{
		auto member = c1.get_enable_internal_store_bias();
		for (auto coord : iter_all<CapMemBlockOnDLS>()) {
			member[coord] = !member[coord];
		}
		c1.set_enable_internal_store_bias(member);
	}

	{
		auto member = c1.get_enable_internal_output_bias();
		for (auto coord : iter_all<CapMemBlockOnDLS>()) {
			member[coord] = !member[coord];
		}
		c1.set_enable_internal_output_bias(member);
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
