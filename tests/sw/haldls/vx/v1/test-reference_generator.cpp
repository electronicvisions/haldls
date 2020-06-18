#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/jtag.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/v1/capmem.h"
#include "haldls/vx/v1/common.h"
#include "test-helper.h"

using namespace haldls::vx::v1;
using namespace halco::hicann_dls::vx::v1;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<fisch::vx::OmnibusChipOverJTAG> words_type;

TEST(ReferenceGeneratorConfig, General)
{
	ReferenceGeneratorConfig config;

	{
		auto member = config.get_enable_internal_reference();
		member = !member;
		config.set_enable_internal_reference(member);
		EXPECT_EQ(config.get_enable_internal_reference(), member);
	}

	{
		auto member = config.get_enable_reference_output();
		member = !member;
		config.set_enable_reference_output(member);
		EXPECT_EQ(config.get_enable_reference_output(), member);
	}

	{
		auto member = config.get_enable_reference_input();
		member = !member;
		config.set_enable_reference_input(member);
		EXPECT_EQ(config.get_enable_reference_input(), member);
	}

	{
		auto member = config.get_enable_reset();
		member = !member;
		config.set_enable_reset(member);
		EXPECT_EQ(config.get_enable_reset(), member);
	}

	{
		auto member = draw_ranged_non_default_value<ReferenceGeneratorConfig::CapMemAmplifier>(
		    config.get_capmem_amplifier());
		config.set_capmem_amplifier(member);
		EXPECT_EQ(config.get_capmem_amplifier(), member);
	}

	{
		auto member = draw_ranged_non_default_value<ReferenceGeneratorConfig::CapMemOffset>(
		    config.get_capmem_offset());
		config.set_capmem_offset(member);
		EXPECT_EQ(config.get_capmem_offset(), member);
	}

	{
		auto member = draw_ranged_non_default_value<ReferenceGeneratorConfig::CapMemSlope>(
		    config.get_capmem_slope());
		config.set_capmem_slope(member);
		EXPECT_EQ(config.get_capmem_slope(), member);
	}

	{
		auto member = draw_ranged_non_default_value<ReferenceGeneratorConfig::ResistorControl>(
		    config.get_resistor_control());
		config.set_resistor_control(member);
		EXPECT_EQ(config.get_resistor_control(), member);
	}

	{
		auto member = draw_ranged_non_default_value<ReferenceGeneratorConfig::ReferenceControl>(
		    config.get_reference_control());
		config.set_reference_control(member);
		EXPECT_EQ(config.get_reference_control(), member);
	}

	ReferenceGeneratorConfig default_config;
	ASSERT_NE(config, default_config);
	ASSERT_TRUE(config != default_config);
	ASSERT_FALSE(config == default_config);

	default_config = config;
	ASSERT_EQ(config, default_config);
	ASSERT_TRUE(config == default_config);
	ASSERT_FALSE(config != default_config);
}

TEST(ReferenceGeneratorConfig, EncodeDecode)
{
	ReferenceGeneratorConfig config;
	config.set_enable_reference_output(true);
	config.set_capmem_offset(ReferenceGeneratorConfig::CapMemOffset(43));

	auto coord = typename ReferenceGeneratorConfig::coordinate_type();

	std::array<OmnibusChipOverJTAGAddress, ReferenceGeneratorConfig::config_size_in_words>
	    ref_addresses = {OmnibusChipOverJTAGAddress{0x000c0012},
	                     OmnibusChipOverJTAGAddress{0x000c0013},
	                     OmnibusChipOverJTAGAddress{0x000c0014}};

	{ // check if write addresses are correct
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // check if read addresses are correct
		addresses_type read_addresses;
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// Encode
	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	ASSERT_EQ(data[0].get() & 0b0000'1111'1100'0000, 43 << 6); // CapMem offset
	ASSERT_EQ(data[0].get(), 0x3afc);
	ASSERT_EQ(data[1].get(), 0x285);
	ASSERT_EQ(data[2].get(), 0x28);

	// Decode
	ReferenceGeneratorConfig decoded;
	ASSERT_NE(config, decoded);
	visit_preorder(decoded, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, decoded);
}

TEST(ReferenceGeneratorConfig, CerealizeCoverage)
{
	ReferenceGeneratorConfig c1, c2;

	{
		auto member = c1.get_enable_internal_reference();
		member = !member;
		c1.set_enable_internal_reference(member);
	}

	{
		auto member = c1.get_enable_reference_output();
		member = !member;
		c1.set_enable_reference_output(member);
	}

	{
		auto member = c1.get_enable_reference_input();
		member = !member;
		c1.set_enable_reference_input(member);
	}

	{
		auto member = c1.get_enable_reset();
		member = !member;
		c1.set_enable_reset(member);
	}

	{
		auto member = draw_ranged_non_default_value<ReferenceGeneratorConfig::CapMemAmplifier>(
		    c1.get_capmem_amplifier());
		c1.set_capmem_amplifier(member);
	}

	{
		auto member = draw_ranged_non_default_value<ReferenceGeneratorConfig::CapMemOffset>(
		    c1.get_capmem_offset());
		c1.set_capmem_offset(member);
	}

	{
		auto member = draw_ranged_non_default_value<ReferenceGeneratorConfig::CapMemSlope>(
		    c1.get_capmem_slope());
		c1.set_capmem_slope(member);
	}

	{
		auto member = draw_ranged_non_default_value<ReferenceGeneratorConfig::ResistorControl>(
		    c1.get_resistor_control());
		c1.set_resistor_control(member);
	}

	{
		auto member = draw_ranged_non_default_value<ReferenceGeneratorConfig::ReferenceControl>(
		    c1.get_reference_control());
		c1.set_reference_control(member);
	}

	ASSERT_NE(c1, c2);

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
