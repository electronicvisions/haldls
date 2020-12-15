#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"
#include "halco/hicann-dls/vx/jtag.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/phy.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusAddress> addresses_type;
typedef std::vector<fisch::vx::Omnibus> words_type;

template <typename PhyConfigDerived>
void test_phy_config_derived_general()
{
	PhyConfigDerived config;

	// test getter/setter
	{
		bool value = !config.get_enable_bit_slip();
		config.set_enable_bit_slip(value);
		EXPECT_EQ(config.get_enable_bit_slip(), value);
	}

	{
		auto value = draw_ranged_non_default_value<typename PhyConfigDerived::ManualDelay>(
		    config.get_manual_delay());
		config.set_manual_delay(value);
		EXPECT_EQ(config.get_manual_delay(), value);
	}

	{
		bool value = !config.get_enable_delay_cell_measurement();
		config.set_enable_delay_cell_measurement(value);
		EXPECT_EQ(config.get_enable_delay_cell_measurement(), value);
	}

	{
		bool value = !config.get_enable_initialization_master_mode();
		config.set_enable_initialization_master_mode(value);
		EXPECT_EQ(config.get_enable_initialization_master_mode(), value);
	}

	{
		bool value = !config.get_enable_manual_tx_data_valid_for_init();
		config.set_enable_manual_tx_data_valid_for_init(value);
		EXPECT_EQ(config.get_enable_manual_tx_data_valid_for_init(), value);
	}

	{
		bool value = !config.get_enable_force_lvds_power_up();
		config.set_enable_force_lvds_power_up(value);
		EXPECT_EQ(config.get_enable_force_lvds_power_up(), value);
	}

	{
		bool value = !config.get_enable_force_start();
		config.set_enable_force_start(value);
		EXPECT_EQ(config.get_enable_force_start(), value);
	}

	{
		bool value = !config.get_enable_manual_training_mode();
		config.set_enable_manual_training_mode(value);
		EXPECT_EQ(config.get_enable_manual_training_mode(), value);
	}

	{
		bool value = !config.get_enable_ber_loopback();
		config.set_enable_ber_loopback(value);
		EXPECT_EQ(config.get_enable_ber_loopback(), value);
	}

	{
		auto value =
		    draw_ranged_non_default_value<typename PhyConfigDerived::VBias>(config.get_vbias());
		config.set_vbias(value);
		EXPECT_EQ(config.get_vbias(), value);
	}

	{
		auto value = draw_ranged_non_default_value<typename PhyConfigDerived::DebugOutputs>(
		    config.get_debug_outputs());
		config.set_debug_outputs(value);
		EXPECT_EQ(config.get_debug_outputs(), value);
	}

	{
		bool value = !config.get_enable_transmission_without_idle_pattern();
		config.set_enable_transmission_without_idle_pattern(value);
		EXPECT_EQ(config.get_enable_transmission_without_idle_pattern(), value);
	}

	{
		bool value = !config.get_enable_clock_pre_alignment();
		config.set_enable_clock_pre_alignment(value);
		EXPECT_EQ(config.get_enable_clock_pre_alignment(), value);
	}

	{
		bool value = !config.get_enable_des_recal();
		config.set_enable_des_recal(value);
		EXPECT_EQ(config.get_enable_des_recal(), value);
	}

	{
		bool value = !config.get_enable_loopback_en();
		config.set_enable_loopback_en(value);
		EXPECT_EQ(config.get_enable_loopback_en(), value);
	}

	{
		bool value = !config.get_enable_auto_init();
		config.set_enable_auto_init(value);
		EXPECT_EQ(config.get_enable_auto_init(), value);
	}

	PhyConfigDerived config_eq = config;
	PhyConfigDerived config_default;


	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(PhyConfigFPGA, General)
{
	test_phy_config_derived_general<PhyConfigFPGA>();
}

TEST(PhyConfigChip, General)
{
	test_phy_config_derived_general<PhyConfigChip>();
}

template <typename PhyConfigDerived>
void test_phy_config_derived_cerealize_coverage()
{
	PhyConfigDerived obj1, obj2;

	{
		bool value = !obj1.get_enable_bit_slip();
		obj1.set_enable_bit_slip(value);
	}

	{
		auto value = draw_ranged_non_default_value<typename PhyConfigDerived::ManualDelay>(
		    obj1.get_manual_delay());
		obj1.set_manual_delay(value);
	}

	{
		bool value = !obj1.get_enable_delay_cell_measurement();
		obj1.set_enable_delay_cell_measurement(value);
	}

	{
		bool value = !obj1.get_enable_initialization_master_mode();
		obj1.set_enable_initialization_master_mode(value);
	}

	{
		bool value = !obj1.get_enable_manual_tx_data_valid_for_init();
		obj1.set_enable_manual_tx_data_valid_for_init(value);
	}

	{
		bool value = !obj1.get_enable_force_lvds_power_up();
		obj1.set_enable_force_lvds_power_up(value);
	}

	{
		bool value = !obj1.get_enable_force_start();
		obj1.set_enable_force_start(value);
	}

	{
		bool value = !obj1.get_enable_manual_training_mode();
		obj1.set_enable_manual_training_mode(value);
	}

	{
		bool value = !obj1.get_enable_ber_loopback();
		obj1.set_enable_ber_loopback(value);
	}

	{
		auto value =
		    draw_ranged_non_default_value<typename PhyConfigDerived::VBias>(obj1.get_vbias());
		obj1.set_vbias(value);
	}

	{
		auto value = draw_ranged_non_default_value<typename PhyConfigDerived::DebugOutputs>(
		    obj1.get_debug_outputs());
		obj1.set_debug_outputs(value);
	}

	{
		bool value = !obj1.get_enable_transmission_without_idle_pattern();
		obj1.set_enable_transmission_without_idle_pattern(value);
	}

	{
		bool value = !obj1.get_enable_clock_pre_alignment();
		obj1.set_enable_clock_pre_alignment(value);
	}

	{
		bool value = !obj1.get_enable_des_recal();
		obj1.set_enable_des_recal(value);
	}

	{
		bool value = !obj1.get_enable_loopback_en();
		obj1.set_enable_loopback_en(value);
	}

	{
		bool value = !obj1.get_enable_auto_init();
		obj1.set_enable_auto_init(value);
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

TEST(PhyConfigFPGA, CerealizeCoverage)
{
	test_phy_config_derived_cerealize_coverage<PhyConfigFPGA>();
}

TEST(PhyConfigChip, CerealizeCoverage)
{
	test_phy_config_derived_cerealize_coverage<PhyConfigChip>();
}


TEST(PhyConfigFPGA, EncodeDecode)
{
	PhyConfigFPGA config;

	PhyConfigFPGAOnDLS coord = draw_ranged_non_default_value<PhyConfigFPGAOnDLS>(0);

	halco::hicann_dls::vx::OmnibusAddress ref_address(phy_omnibus_mask + coord.toEnum());
	std::array<halco::hicann_dls::vx::OmnibusAddress, PhyConfigFPGA::config_size_in_words>
	    ref_addresses = {ref_address};
	std::array<fisch::vx::Omnibus, PhyConfigFPGA::config_size_in_words> ref_data = {
	    fisch::vx::Omnibus(fisch::vx::OmnibusData{0x204040})};

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
}

TEST(PhyConfigChip, EncodeDecode)
{
	PhyConfigChip config;

	PhyConfigChipOnDLS coord = draw_ranged_non_default_value<PhyConfigChipOnDLS>(0);

	std::array<JTAGPhyRegisterOnDLS, PhyConfigChip::write_config_size_in_words> ref_addresses = {
	    coord.toJTAGPhyRegisterOnDLS()};
	std::array<fisch::vx::JTAGPhyRegister, PhyConfigChip::write_config_size_in_words> ref_data = {
	    fisch::vx::JTAGPhyRegister(fisch::vx::JTAGPhyRegister::Value(0x204000))};

	{ // write addresses
		std::vector<JTAGPhyRegisterOnDLS> write_addresses;
		visit_preorder(
		    config, coord,
		    stadls::WriteAddressVisitor<std::vector<JTAGPhyRegisterOnDLS>>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	std::vector<fisch::vx::JTAGPhyRegister> data;
	visit_preorder(
	    config, coord, stadls::EncodeVisitor<std::vector<fisch::vx::JTAGPhyRegister>>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
}

TEST(CommonPhyConfigFPGA, General)
{
	CommonPhyConfigFPGA config;

	// test getter/setter
	for (auto coord : halco::common::iter_all<halco::hicann_dls::vx::PhyConfigFPGAOnDLS>()) {
		auto value = !config.get_enable_phy(coord);
		config.set_enable_phy(coord, value);
		EXPECT_EQ(config.get_enable_phy(coord), value);
	}

	CommonPhyConfigFPGA config_eq = config;
	CommonPhyConfigFPGA config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(CommonPhyConfigFPGA, EncodeDecode)
{
	CommonPhyConfigFPGA config;

	CommonPhyConfigFPGAOnDLS coord;

	std::array<OmnibusAddress, CommonPhyConfigFPGA::config_size_in_words> ref_addresses = {
	    OmnibusAddress(0x8800'4000)};
	std::array<fisch::vx::Omnibus, CommonPhyConfigFPGA::config_size_in_words> ref_data = {
	    fisch::vx::Omnibus(fisch::vx::OmnibusData(0xff))};

	{ // write addresses
		std::vector<OmnibusAddress> write_addresses;
		visit_preorder(
		    config, coord,
		    stadls::WriteAddressVisitor<std::vector<OmnibusAddress>>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	std::vector<fisch::vx::Omnibus> data;
	visit_preorder(config, coord, stadls::EncodeVisitor<std::vector<fisch::vx::Omnibus>>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
}

TEST(CommonPhyConfigFPGA, CerealizeCoverage)
{
	CommonPhyConfigFPGA obj1, obj2;
	auto coord = halco::hicann_dls::vx::PhyConfigFPGAOnDLS();
	obj1.set_enable_phy(coord, !obj1.get_enable_phy(coord));

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

TEST(CommonPhyConfigChip, General)
{
	CommonPhyConfigChip config;

	// test getter/setter
	for (auto coord : halco::common::iter_all<halco::hicann_dls::vx::PhyConfigChipOnDLS>()) {
		auto value = !config.get_enable_phy(coord);
		config.set_enable_phy(coord, value);
		EXPECT_EQ(config.get_enable_phy(coord), value);
	}

	CommonPhyConfigChip config_eq = config;
	CommonPhyConfigChip config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(CommonPhyConfigChip, EncodeDecode)
{
	CommonPhyConfigChip config;
	config.set_enable_phy(PhyConfigChipOnDLS(0), false);
	config.set_enable_phy(PhyConfigChipOnDLS(1), false);
	config.set_enable_phy(PhyConfigChipOnDLS(2), false);
	config.set_enable_phy(PhyConfigChipOnDLS(3), false);

	CommonPhyConfigChipOnDLS coord;

	std::array<OmnibusChipOverJTAGAddress, CommonPhyConfigChip::config_size_in_words>
	    ref_addresses = {OmnibusChipOverJTAGAddress(0x00040000)};
	std::array<fisch::vx::OmnibusChipOverJTAG, CommonPhyConfigChip::config_size_in_words> ref_data =
	    {fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusData(0x0f))};

	{ // write addresses
		std::vector<OmnibusChipOverJTAGAddress> write_addresses;
		visit_preorder(
		    config, coord,
		    stadls::WriteAddressVisitor<std::vector<OmnibusChipOverJTAGAddress>>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		std::vector<OmnibusChipOverJTAGAddress> read_addresses;
		visit_preorder(
		    config, coord,
		    stadls::ReadAddressVisitor<std::vector<OmnibusChipOverJTAGAddress>>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	std::vector<fisch::vx::OmnibusChipOverJTAG> data;
	visit_preorder(
	    config, coord, stadls::EncodeVisitor<std::vector<fisch::vx::OmnibusChipOverJTAG>>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	CommonPhyConfigChip config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<decltype(data)>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(CommonPhyConfigChip, CerealizeCoverage)
{
	CommonPhyConfigChip obj1, obj2;
	auto coord = halco::hicann_dls::vx::PhyConfigChipOnDLS();
	obj1.set_enable_phy(coord, !obj1.get_enable_phy(coord));

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
