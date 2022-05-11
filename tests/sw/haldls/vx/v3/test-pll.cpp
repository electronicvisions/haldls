#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/word_access/type/jtag.h"
#include "halco/hicann-dls/vx/jtag.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/common.h"
#include "haldls/vx/v3/pll.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

TEST(PLLClockOutputBlock, General)
{
	haldls::vx::v3::PLLClockOutputBlock config;

	// test getter/setter
	for (auto output : iter_all<PLLClockOutputOnDLS>()) {
		auto value = config.get_clock_output(output);

		value.set_enable_output(!value.get_enable_output());
		value.set_enable_bypass(!value.get_enable_bypass());
		value.set_select_adpll(draw_ranged_non_default_value<ADPLLOnDLS>(value.get_select_adpll()));
		value.set_select_adpll_output(ADPLL::Output::dco); // != default

		config.set_clock_output(output, value);
		EXPECT_EQ(config.get_clock_output(output), value);
	}

	haldls::vx::v3::PLLClockOutputBlock config_eq = config;
	haldls::vx::v3::PLLClockOutputBlock config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(PLLClockOutputBlock, EncodeDecode)
{
	haldls::vx::v3::PLLClockOutputBlock config;

	PLLClockOutputBlockOnDLS coord;

	// JTAG write access
	{
		std::array<JTAGPLLRegisterOnDLS, haldls::vx::v3::PLLClockOutputBlock::config_size_in_words>
		    ref_addresses = {JTAGPLLRegisterOnDLS(4)};
		std::array<
		    fisch::vx::word_access_type::JTAGPLLRegister,
		    haldls::vx::v3::PLLClockOutputBlock::config_size_in_words>
		    ref_data = {fisch::vx::word_access_type::JTAGPLLRegister(0x110d0101)};

		{ // write addresses
			std::vector<JTAGPLLRegisterOnDLS> write_addresses;
			visit_preorder(
			    config, coord,
			    stadls::WriteAddressVisitor<std::vector<JTAGPLLRegisterOnDLS>>{write_addresses});
			EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
		}

		std::vector<fisch::vx::word_access_type::JTAGPLLRegister> data;
		visit_preorder(
		    config, coord,
		    stadls::EncodeVisitor<std::vector<fisch::vx::word_access_type::JTAGPLLRegister>>{data});
		EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
	}

	// Omnibus over JTAG access
	{
		auto value = config.get_clock_output(PLLClockOutputOnDLS(0));
		value.set_enable_bypass(true);
		config.set_clock_output(PLLClockOutputOnDLS(0), value);

		std::array<
		    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
		    haldls::vx::v3::PLLClockOutputBlock::config_size_in_words>
		    ref_addresses = {halco::hicann_dls::vx::OmnibusChipOverJTAGAddress(0x80004)};
		std::array<
		    fisch::vx::word_access_type::OmnibusChipOverJTAG,
		    haldls::vx::v3::PLLClockOutputBlock::config_size_in_words>
		    ref_data = {fisch::vx::word_access_type::OmnibusChipOverJTAG(0x110d0103)};

		{ // write addresses
			std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> write_addresses;
			visit_preorder(
			    config, coord,
			    stadls::WriteAddressVisitor<
			        std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>>{
			        write_addresses});
			EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
		}

		{ // read addresses
			std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> read_addresses;
			visit_preorder(
			    config, coord,
			    stadls::ReadAddressVisitor<
			        std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>>{
			        read_addresses});
			EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
		}

		std::vector<fisch::vx::word_access_type::OmnibusChipOverJTAG> data;
		visit_preorder(
		    config, coord,
		    stadls::EncodeVisitor<std::vector<fisch::vx::word_access_type::OmnibusChipOverJTAG>>{
		        data});
		EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

		haldls::vx::v3::PLLClockOutputBlock config_2;
		visit_preorder(
		    config_2, coord,
		    stadls::DecodeVisitor<std::vector<fisch::vx::word_access_type::OmnibusChipOverJTAG>>(
		        data));
		EXPECT_EQ(config, config_2);
	}
}

TEST(PLLClockOutputBlock, CerealizeCoverage)
{
	haldls::vx::v3::PLLClockOutputBlock obj1, obj2;
	for (auto output : iter_all<PLLClockOutputOnDLS>()) {
		auto value = obj1.get_clock_output(output);

		value.set_enable_output(!value.get_enable_output());
		value.set_enable_bypass(!value.get_enable_bypass());
		value.set_select_adpll(draw_ranged_non_default_value<ADPLLOnDLS>(value.get_select_adpll()));
		value.set_select_adpll_output(ADPLL::Output::dco); // != default

		obj1.set_clock_output(output, value);
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
