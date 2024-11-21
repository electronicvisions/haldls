#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "fisch/vx/word_access/type/jtag.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/capmem.h"
#include "haldls/vx/common.h"
#include "haldls/vx/omnibus_constants.h"
#include "test-helper.h"

using namespace fisch::vx::word_access_type;
using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<fisch::vx::word_access_type::OmnibusChipOverJTAG> words_type;

TEST(CapMemSRAMTimingConfig, General)
{
	CapMemSRAMTimingConfig config;

	// test getter/setter
	{
		auto value = draw_ranged_non_default_value<CapMemSRAMTimingConfig::ReadDelay>(
		    config.get_read_delay());
		config.set_read_delay(value);
		EXPECT_EQ(config.get_read_delay(), value);
	}

	{
		auto value = draw_ranged_non_default_value<CapMemSRAMTimingConfig::AddressSetupTime>(
		    config.get_address_setup_time());
		config.set_address_setup_time(value);
		EXPECT_EQ(config.get_address_setup_time(), value);
	}

	{
		auto value = draw_ranged_non_default_value<CapMemSRAMTimingConfig::WriteWidth>(
		    config.get_write_width());
		config.set_write_width(value);
		EXPECT_EQ(config.get_write_width(), value);
	}

	CapMemSRAMTimingConfig config_eq = config;
	CapMemSRAMTimingConfig config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(CapMemSRAMTimingConfig, EncodeDecode)
{
	CapMemSRAMTimingConfig config;

	config.set_read_delay(CapMemSRAMTimingConfig::ReadDelay(100));
	config.set_address_setup_time(CapMemSRAMTimingConfig::AddressSetupTime(5));
	config.set_write_width(CapMemSRAMTimingConfig::WriteWidth(7));

	auto coord = CapMemSRAMTimingConfigOnDLS(1);

	std::array<OmnibusChipOverJTAGAddress, CapMemSRAMTimingConfig::config_size_in_words>
	    ref_addresses = {
	        OmnibusChipOverJTAGAddress{capmem_ne_sram_timing_base_address},
	        OmnibusChipOverJTAGAddress{capmem_ne_sram_timing_base_address + 1}};

	// Reverse gray coded values of 100 and 5 | 7 << 4. See issue 4047.
	std::array<OmnibusChipOverJTAG, CapMemSRAMTimingConfig::config_size_in_words> ref_data = {
	    OmnibusChipOverJTAG(71), OmnibusChipOverJTAG(89)};

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		addresses_type read_addresses;
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	CapMemSRAMTimingConfig config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(CapMemSRAMTimingConfig, CerealizeCoverage)
{
	CapMemSRAMTimingConfig obj1, obj2;
	{
		auto value =
		    draw_ranged_non_default_value<CapMemSRAMTimingConfig::ReadDelay>(obj1.get_read_delay());
		obj1.set_read_delay(value);
	}
	{
		auto value = draw_ranged_non_default_value<CapMemSRAMTimingConfig::AddressSetupTime>(
		    obj1.get_address_setup_time());
		obj1.set_address_setup_time(value);
	}
	{
		auto value = draw_ranged_non_default_value<CapMemSRAMTimingConfig::WriteWidth>(
		    obj1.get_write_width());
		obj1.set_write_width(value);
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