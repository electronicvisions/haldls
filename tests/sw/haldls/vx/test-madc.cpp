#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/vx/madc.h"

#include "fisch/vx/jtag.h"
#include "haldls/vx/omnibus_constants.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<fisch::vx::OmnibusChipOverJTAG> words_type;

TEST(MADCControl, General)
{
	MADCControl control;

	// test getter/setter
	{
		auto value = !control.get_enable_iconv_amplifier();
		control.set_enable_iconv_amplifier(value);
		EXPECT_EQ(control.get_enable_iconv_amplifier(), value);
	}

	{
		auto value = !control.get_enable_pre_amplifier();
		control.set_enable_pre_amplifier(value);
		EXPECT_EQ(control.get_enable_pre_amplifier(), value);
	}

	{
		auto value = !control.get_enable_continuous_sampling();
		control.set_enable_continuous_sampling(value);
		EXPECT_EQ(control.get_enable_continuous_sampling(), value);
	}

	{
		auto value = !control.get_enable_power_down_after_sampling();
		control.set_enable_power_down_after_sampling(value);
		EXPECT_EQ(control.get_enable_power_down_after_sampling(), value);
	}

	{
		auto value = !control.get_stop_recording();
		control.set_stop_recording(value);
		EXPECT_EQ(control.get_stop_recording(), value);
	}

	{
		auto value = !control.get_start_recording();
		control.set_start_recording(value);
		EXPECT_EQ(control.get_start_recording(), value);
	}

	{
		auto value = !control.get_wake_up();
		control.set_wake_up(value);
		EXPECT_EQ(control.get_wake_up(), value);
	}

	MADCControl control_eq = control;
	MADCControl control_default;

	// test comparison
	ASSERT_EQ(control, control_eq);
	ASSERT_FALSE(control == control_default);

	ASSERT_NE(control, control_default);
	ASSERT_FALSE(control != control_eq);
}

TEST(MADCControl, CerealizeCoverage)
{
	MADCControl c1, c2;

	c1.set_enable_iconv_amplifier(true);
	c1.set_enable_pre_amplifier(true);
	c1.set_enable_continuous_sampling(true);
	c1.set_enable_power_down_after_sampling(true);
	c1.set_stop_recording(true);
	c1.set_start_recording(true);
	c1.set_wake_up(true);

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

TEST(MADCControl, EncodeDecode)
{
	MADCControl control;
	control.set_enable_iconv_amplifier(true);
	control.set_enable_pre_amplifier(true);
	control.set_enable_continuous_sampling(false);
	control.set_enable_power_down_after_sampling(true);
	control.set_stop_recording(false);
	control.set_start_recording(false);
	control.set_wake_up(true);

	auto const coord = MADCControlOnDLS(0);

	std::array<
	    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, MADCControl::write_config_size_in_words>
	    ref_addresses = {halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{0x000c'0000}};
	std::array<fisch::vx::OmnibusChipOverJTAG, MADCControl::write_config_size_in_words> ref_data = {
	    static_cast<fisch::vx::OmnibusData>(0b1101001)};

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(
		    control, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(control, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
}
