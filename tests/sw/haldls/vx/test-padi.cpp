#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/vx/padi.h"

#include "fisch/vx/jtag.h"
#include "haldls/vx/omnibus_constants.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<fisch::vx::OmnibusChipOverJTAG> words_type;

TEST(PADIEvent, General)
{
	PADIEvent event;

	// test getter/setter
	{
		auto value = event.get_fire_bus();
		value[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(2)] = true;
		event.set_fire_bus(value);
		EXPECT_EQ(event.get_fire_bus(), value);
	}

	{
		auto value = draw_ranged_non_default_value<decltype(event.get_event_address())>(
		    event.get_event_address());
		event.set_event_address(value);
		EXPECT_EQ(event.get_event_address(), value);
	}

	{
		auto value = draw_ranged_non_default_value<decltype(event.get_row_select_address())>(
		    event.get_row_select_address());
		event.set_row_select_address(value);
		EXPECT_EQ(event.get_row_select_address(), value);
	}

	PADIEvent event_eq = event;
	PADIEvent event_default;

	// test comparison
	ASSERT_EQ(event, event);
	ASSERT_FALSE(event == event_default);

	ASSERT_NE(event, event_default);
	ASSERT_FALSE(event != event_eq);
}

TEST(PADIEvent, CerealizeCoverage)
{
	PADIEvent e1, e2;

	auto value = e1.get_fire_bus();
	value[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(2)] = true;
	e1.set_fire_bus(value);
	e1.set_event_address(
	    draw_ranged_non_default_value<decltype(e1.get_event_address())>(e1.get_event_address()));
	e1.set_row_select_address(draw_ranged_non_default_value<decltype(e1.get_row_select_address())>(
	    e1.get_row_select_address()));

	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(e1);
	}
	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(e2);
	}
	ASSERT_EQ(e1, e2);
}

TEST(PADIEvent, EncodeDecode)
{
	PADIEvent event;
	auto fire_bus = event.get_fire_bus();
	fire_bus[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(0)] = true;
	fire_bus[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(2)] = true;
	event.set_fire_bus(fire_bus);
	event.set_event_address(PADIEvent::EventAddress(13));
	event.set_row_select_address(PADIEvent::RowSelectAddress(7));

	PADIEventOnDLS coord(PADIEventOnDLS::top);

	std::array<
	    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, PADIEvent::write_config_size_in_words>
	    ref_addresses = {
	        halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{padi_base_addresses[coord] + 0}};
	std::array<fisch::vx::OmnibusChipOverJTAG, PADIEvent::write_config_size_in_words> ref_data = {
	    static_cast<fisch::vx::OmnibusData>(0b0101 | (13 << 16) | (7 << 22))};

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(event, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(event, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
}

TEST(CommonPADIBusConfig, General)
{
	CommonPADIBusConfig config;

	// test getter/setter
	{
		auto value = config.get_enable_spl1();
		value[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(1)] = true;
		value[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(3)] = true;
		config.set_enable_spl1(value);
		EXPECT_EQ(config.get_enable_spl1(), value);
	}

	{
		auto value = config.get_enable_extended_timing();
		value[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(0)] = true;
		config.set_enable_extended_timing(value);
		EXPECT_EQ(config.get_enable_extended_timing(), value);
	}

	{
		auto value = config.get_dacen_pulse_extension();
		value[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(1)] =
		    CommonPADIBusConfig::DacenPulseExtension(3);
		value[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(2)] =
		    CommonPADIBusConfig::DacenPulseExtension(3);
		config.set_dacen_pulse_extension(value);
		EXPECT_EQ(config.get_dacen_pulse_extension(), value);
	}

	CommonPADIBusConfig config_eq = config;
	CommonPADIBusConfig config_default;

	// test comparison
	ASSERT_EQ(config, config);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(CommonPADIBusConfig, CerealizeCoverage)
{
	CommonPADIBusConfig c1, c2;

	auto enable_spl1 = c1.get_enable_spl1();
	enable_spl1[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(1)] = true;
	enable_spl1[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(3)] = true;
	c1.set_enable_spl1(enable_spl1);

	auto enable_extended_timing = c1.get_enable_extended_timing();
	enable_extended_timing[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(0)] = true;
	c1.set_enable_extended_timing(enable_extended_timing);

	auto dacen_extension = c1.get_dacen_pulse_extension();
	dacen_extension[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(1)] =
	    CommonPADIBusConfig::DacenPulseExtension(3);
	dacen_extension[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(2)] =
	    CommonPADIBusConfig::DacenPulseExtension(3);
	c1.set_dacen_pulse_extension(dacen_extension);

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

TEST(CommonPADIBusConfig, EncodeDecode)
{
	CommonPADIBusConfig config;

	auto enable_spl1 = config.get_enable_spl1();
	enable_spl1[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(1)] = true;
	enable_spl1[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(3)] = true;
	config.set_enable_spl1(enable_spl1);

	auto enable_extended_timing = config.get_enable_extended_timing();
	enable_extended_timing[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(0)] = true;
	config.set_enable_extended_timing(enable_extended_timing);

	auto dacen_extension = config.get_dacen_pulse_extension();
	dacen_extension[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(2)] =
	    CommonPADIBusConfig::DacenPulseExtension(3);
	config.set_dacen_pulse_extension(dacen_extension);

	CommonPADIBusConfigOnDLS coord(CommonPADIBusConfigOnDLS::bottom);

	std::array<
	    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
	    CommonPADIBusConfig::write_config_size_in_words>
	    ref_addresses = {
	        halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{padi_base_addresses[coord] + 1}};
	std::array<fisch::vx::OmnibusChipOverJTAG, CommonPADIBusConfig::write_config_size_in_words>
	    ref_data = {static_cast<fisch::vx::OmnibusData>(0b1010 | (0b0001 << 4) | (3 << 24))};

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
}

TEST(CommonSTPConfig, General)
{
	CommonSTPConfig config;

	// test getter/setter
	{
		bool value = !config.get_enable_recovery_clock();
		config.set_enable_recovery_clock(value);
		EXPECT_EQ(config.get_enable_recovery_clock(), value);
	}

	{
		auto value = draw_ranged_non_default_value<decltype(config.get_recovery_clock_speed())>(
		    config.get_recovery_clock_speed());
		config.set_recovery_clock_speed(value);
		EXPECT_EQ(config.get_recovery_clock_speed(), value);
	}

	CommonSTPConfig config_eq = config;
	CommonSTPConfig config_default;

	// test comparison
	ASSERT_EQ(config, config);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(CommonSTPConfig, CerealizeCoverage)
{
	CommonSTPConfig c1, c2;

	c1.set_enable_recovery_clock(true);
	c1.set_recovery_clock_speed(
	    draw_ranged_non_default_value<decltype(c1.get_recovery_clock_speed())>(
	        c1.get_recovery_clock_speed()));

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

TEST(CommonSTPConfig, EncodeDecode)
{
	CommonSTPConfig config;

	config.set_enable_recovery_clock(true);
	auto recovery_speed =
	    draw_ranged_non_default_value<decltype(config.get_recovery_clock_speed())>(
	        config.get_recovery_clock_speed());
	config.set_recovery_clock_speed(recovery_speed);

	CommonSTPConfigOnDLS coord(CommonSTPConfigOnDLS::top);

	std::array<
	    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
	    CommonSTPConfig::write_config_size_in_words>
	    ref_addresses = {
	        halco::hicann_dls::vx::OmnibusChipOverJTAGAddress{padi_base_addresses[coord] + 2}};
	std::array<fisch::vx::OmnibusChipOverJTAG, CommonSTPConfig::write_config_size_in_words>
	    ref_data = {static_cast<fisch::vx::OmnibusData>(recovery_speed | (1 << 4))};

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
}
