#include <cereal/cereal.hpp>
#include <gtest/gtest.h>

#include "halco/hicann-dls/vx/jtag.h"
#include "haldls/vx/jtag.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;


TEST(JTAGIdCode, General)
{
	JTAGIdCode config;

	auto version = draw_ranged_non_default_value<JTAGIdCode::Version>();
	config.set_version(version);
	ASSERT_EQ(config.get_version(), version);

	auto part_number = draw_ranged_non_default_value<JTAGIdCode::PartNumber>();
	config.set_part_number(part_number);
	ASSERT_EQ(config.get_part_number(), part_number);

	auto manufacturer_id = draw_ranged_non_default_value<JTAGIdCode::ManufacturerId>();
	config.set_manufacturer_id(manufacturer_id);
	ASSERT_EQ(config.get_manufacturer_id(), manufacturer_id);

	JTAGIdCode default_config;
	ASSERT_NE(config, default_config);
	ASSERT_TRUE(config != default_config);
	ASSERT_FALSE(config == default_config);
	JTAGIdCode copy_config = config;
	ASSERT_EQ(config, copy_config);
	ASSERT_TRUE(config == copy_config);
	ASSERT_FALSE(config != copy_config);
}

TEST(JTAGIdCode, Decode)
{
	auto good_jtag_id = fisch::vx::JTAGIdCode::Value(0x020B40DD);
	auto bad_jtag_id = fisch::vx::JTAGIdCode::Value(0x020B40DC); // Marker bit has to be 0b1.

	JTAGIdCode config;
	std::array<fisch::vx::JTAGIdCode, JTAGIdCode::read_config_size_in_words> data{
	    fisch::vx::JTAGIdCode(good_jtag_id)};

	config.decode(data);
	EXPECT_EQ(0, config.get_version());
	EXPECT_EQ(8372, config.get_part_number());
	EXPECT_EQ(110, config.get_manufacturer_id());

	data[0] = fisch::vx::JTAGIdCode(bad_jtag_id);
	EXPECT_ANY_THROW(config.decode(data));
}

TEST(JTAGIdCode, CerealizeCoverage)
{
	JTAGIdCode c1, c2;
	c1.set_version(JTAGIdCode::Version(1));
	c1.set_part_number(JTAGIdCode::PartNumber(2));
	c1.set_manufacturer_id(JTAGIdCode::ManufacturerId(3));
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
