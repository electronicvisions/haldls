#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/jtag.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/v1/omnibus_constants.h"
#include "haldls/vx/v1/sram_controller.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx::v1;
using namespace haldls::vx::detail;
using namespace halco::hicann_dls::vx;
using namespace halco::common;
using namespace fisch::vx;

typedef std::vector<OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<OmnibusChipOverJTAG> words_type;

TEST(SRAMTimingConfig, General)
{
	SRAMTimingConfig config;

	// test getter/setter
	{
		auto value =
		    draw_ranged_non_default_value<SRAMTimingConfig::ReadDelay>(config.get_read_delay());
		config.set_read_delay(value);
		EXPECT_EQ(config.get_read_delay(), value);
	}

	{
		auto value = draw_ranged_non_default_value<SRAMTimingConfig::AddressSetupTime>(
		    config.get_address_setup_time());
		config.set_address_setup_time(value);
		EXPECT_EQ(config.get_address_setup_time(), value);
	}

	{
		auto value =
		    draw_ranged_non_default_value<SRAMTimingConfig::EnableWidth>(config.get_enable_width());
		config.set_enable_width(value);
		EXPECT_EQ(config.get_enable_width(), value);
	}

	SRAMTimingConfig config_eq = config;
	SRAMTimingConfig config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(SRAMTimingConfig, CerealizeCoverage)
{
	SRAMTimingConfig obj1, obj2;
	{
		auto value =
		    draw_ranged_non_default_value<SRAMTimingConfig::ReadDelay>(obj1.get_read_delay());
		obj1.set_read_delay(value);
	}
	{
		auto value = draw_ranged_non_default_value<SRAMTimingConfig::AddressSetupTime>(
		    obj1.get_address_setup_time());
		obj1.set_address_setup_time(value);
	}
	{
		auto value =
		    draw_ranged_non_default_value<SRAMTimingConfig::EnableWidth>(obj1.get_enable_width());
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
