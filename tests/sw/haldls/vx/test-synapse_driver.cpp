#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "fisch/vx/jtag.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/vx/common.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/sram_controller.h"
#include "haldls/vx/synapse_driver.h"
#include "test-helper.h"

using namespace fisch::vx;
using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<fisch::vx::OmnibusChipOverJTAG> words_type;

TEST(SynapseDriverSRAMTimingConfig, General)
{
	SynapseDriverSRAMTimingConfig config;

	// test getter/setter
	{
		auto value = draw_ranged_non_default_value<SynapseDriverSRAMTimingConfig::ReadDelay>(
		    config.get_read_delay());
		config.set_read_delay(value);
		EXPECT_EQ(config.get_read_delay(), value);
	}

	{
		auto value = draw_ranged_non_default_value<SynapseDriverSRAMTimingConfig::AddressSetupTime>(
		    config.get_address_setup_time());
		config.set_address_setup_time(value);
		EXPECT_EQ(config.get_address_setup_time(), value);
	}

	{
		auto value = draw_ranged_non_default_value<SynapseDriverSRAMTimingConfig::EnableWidth>(
		    config.get_enable_width());
		config.set_enable_width(value);
		EXPECT_EQ(config.get_enable_width(), value);
	}

	SynapseDriverSRAMTimingConfig config_eq = config;
	SynapseDriverSRAMTimingConfig config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(SynapseDriverSRAMTimingConfig, EncodeDecode)
{
	SynapseDriverSRAMTimingConfig config;

	config.set_read_delay(SynapseDriverSRAMTimingConfig::ReadDelay(100));
	config.set_address_setup_time(SynapseDriverSRAMTimingConfig::AddressSetupTime(5));
	config.set_enable_width(SynapseDriverSRAMTimingConfig::EnableWidth(7));

	auto coord = SynapseDriverSRAMTimingConfigOnDLS(1);

	std::array<OmnibusChipOverJTAGAddress, SynapseDriverSRAMTimingConfig::config_size_in_words>
	    ref_addresses = {
	        OmnibusChipOverJTAGAddress{synapse_driver_bottom_sram_timing_base_address},
	        OmnibusChipOverJTAGAddress{synapse_driver_bottom_sram_timing_base_address + 1}};
	std::array<OmnibusChipOverJTAG, SynapseDriverSRAMTimingConfig::config_size_in_words> ref_data =
	    {OmnibusData{100}, OmnibusData{5 | 7 << 4}};

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

	SynapseDriverSRAMTimingConfig config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(SynapseDriverSRAMTimingConfig, CerealizeCoverage)
{
	SynapseDriverSRAMTimingConfig obj1, obj2;
	{
		auto value = draw_ranged_non_default_value<SynapseDriverSRAMTimingConfig::ReadDelay>(
		    obj1.get_read_delay());
		obj1.set_read_delay(value);
	}
	{
		auto value = draw_ranged_non_default_value<SynapseDriverSRAMTimingConfig::AddressSetupTime>(
		    obj1.get_address_setup_time());
		obj1.set_address_setup_time(value);
	}
	{
		auto value = draw_ranged_non_default_value<SynapseDriverSRAMTimingConfig::EnableWidth>(
		    obj1.get_enable_width());
		obj1.set_enable_width(value);
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
