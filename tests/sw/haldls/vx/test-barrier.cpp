#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/barrier.h"
#include "halco/hicann-dls/vx/barrier.h"
#include "haldls/cerealization.h"
#include "haldls/vx/v1/barrier.h"

using namespace haldls::vx::v1;

TEST(Barrier, General)
{
	Barrier config;

	{
		auto value = !config.get_enable_omnibus();
		config.set_enable_omnibus(value);
		EXPECT_EQ(config.get_enable_omnibus(), value);
	}

	{
		auto value = !config.get_enable_jtag();
		config.set_enable_jtag(value);
		EXPECT_EQ(config.get_enable_jtag(), value);
	}

	{
		auto value = !config.get_enable_systime();
		config.set_enable_systime(value);
		EXPECT_EQ(config.get_enable_systime(), value);
	}

	Barrier default_config;
	ASSERT_NE(config, default_config);
	ASSERT_TRUE(config != default_config);
	ASSERT_FALSE(config == default_config);
	default_config = config;
	ASSERT_EQ(config, default_config);
	ASSERT_TRUE(config == default_config);
	ASSERT_FALSE(config != default_config);
}

TEST(Barrier, Encode)
{
	Barrier config;
	config.set_enable_omnibus(false);

	EXPECT_EQ(config.encode().get(), 0x6);
}

TEST(Barrier, CerealizeCoverage)
{
	Barrier c1, c2;
	c1.set_enable_omnibus(!c1.get_enable_omnibus());
	c1.set_enable_jtag(!c1.get_enable_jtag());
	c1.set_enable_systime(!c1.get_enable_systime());

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
