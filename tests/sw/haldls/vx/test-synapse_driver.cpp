#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "fisch/vx/jtag.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/vx/v1/common.h"
#include "haldls/vx/v1/omnibus_constants.h"
#include "haldls/vx/v1/sram_controller.h"
#include "haldls/vx/v1/synapse_driver.h"
#include "test-helper.h"

using namespace fisch::vx;
using namespace haldls::vx::v1;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<fisch::vx::OmnibusChipOverJTAG> words_type;

TEST(SynapseDriverConfig, General)
{
	SynapseDriverConfig config;

	bool value;

	value = !config.get_enable_receiver();
	config.set_enable_receiver(value);
	ASSERT_EQ(config.get_enable_receiver(), value);

	value = !config.get_enable_address_out();
	config.set_enable_address_out(value);
	ASSERT_EQ(config.get_enable_address_out(), value);

	{
		auto const value = SynapseDriverConfig::RowMode::excitatory;
		config.set_row_mode_bottom(value);
		ASSERT_EQ(config.get_row_mode_bottom(), value);
	}

	{
		auto const value = SynapseDriverConfig::RowMode::inhibitory;
		config.set_row_mode_top(value);
		ASSERT_EQ(config.get_row_mode_top(), value);
	}

	value = !config.get_enable_readout();
	config.set_enable_readout(value);
	ASSERT_EQ(config.get_enable_readout(), value);

	value = !config.get_enable_renewing();
	config.set_enable_renewing(value);
	ASSERT_EQ(config.get_enable_renewing(), value);

	value = !config.get_enable_hagen_modulation();
	config.set_enable_hagen_modulation(value);
	ASSERT_EQ(config.get_enable_hagen_modulation(), value);

	value = !config.get_enable_stp();
	config.set_enable_stp(value);
	ASSERT_EQ(config.get_enable_stp(), value);

	value = !config.get_enable_charge_sharing();
	config.set_enable_charge_sharing(value);
	ASSERT_EQ(config.get_enable_charge_sharing(), value);

	value = !config.get_enable_recovery();
	config.set_enable_recovery(value);
	ASSERT_EQ(config.get_enable_recovery(), value);

	auto row_address_compare_mask =
	    draw_ranged_non_default_value<SynapseDriverConfig::RowAddressCompareMask>(
	        config.get_row_address_compare_mask());
	config.set_row_address_compare_mask(row_address_compare_mask);
	ASSERT_EQ(config.get_row_address_compare_mask(), row_address_compare_mask);

	auto utilization = SynapseDriverConfig::Utilization(config.get_utilization() + 1);
	config.set_utilization(utilization);
	ASSERT_EQ(config.get_utilization(), utilization);

	auto offset = SynapseDriverConfig::Offset(config.get_offset() + 1);
	config.set_offset(offset);
	ASSERT_EQ(config.get_offset(), offset);

	auto recovery = SynapseDriverConfig::Recovery(config.get_recovery() + 1);
	config.set_recovery(recovery);
	ASSERT_EQ(config.get_recovery(), recovery);

	auto select_target_voltages =
	    SynapseDriverConfig::TargetVoltages(config.get_select_target_voltages() + 1);
	config.set_select_target_voltages(select_target_voltages);
	ASSERT_EQ(config.get_select_target_voltages(), select_target_voltages);

	SynapseDriverConfig default_config;
	ASSERT_NE(config, default_config);
	ASSERT_TRUE(config != default_config);
	ASSERT_FALSE(config == default_config);
	default_config = config;
	ASSERT_EQ(config, default_config);
	ASSERT_TRUE(config == default_config);
	ASSERT_FALSE(config != default_config);
}

TEST(SynapseDriverConfig, EncodeDecode)
{
	SynapseDriverConfig config;
	config.set_recovery(SynapseDriverConfig::Recovery(7));
	config.set_enable_receiver(true);
	config.set_row_mode_top(SynapseDriverConfig::RowMode::excitatory);
	config.set_enable_readout(true);

	auto coord =
	    SynapseDriverOnDLS(SynapseDriverOnSynapseDriverBlock(42), SynapseDriverBlockOnDLS(1));

	std::array<OmnibusChipOverJTAGAddress, SynapseDriverConfig::config_size_in_words>
	    ref_addresses = {OmnibusChipOverJTAGAddress{0x118400 + 42 * 4 + 0},
	                     OmnibusChipOverJTAGAddress{0x118400 + 42 * 4 + 1},
	                     OmnibusChipOverJTAGAddress{0x118400 + 42 * 4 + 2}};

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
	ASSERT_EQ(data[1].get() & 0b00001111, 14);       // recovery (bits reversed)
	ASSERT_TRUE(data[0].get() & 0b0000000100000000); // en_receiver
	ASSERT_TRUE(data[1].get() & 0b0000000000100000); // en_exc_top
	ASSERT_TRUE(data[2].get() & 0b0000000000000010); // en_readout

	// Decode back
	SynapseDriverConfig config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(SynapseDriverConfig, CerealizeCoverage)
{
	SynapseDriverConfig c1, c2;
	c1.set_recovery(SynapseDriverConfig::Recovery(7));
	c1.set_row_mode_top(SynapseDriverConfig::RowMode::excitatory);
	c1.set_row_mode_bottom(SynapseDriverConfig::RowMode::inhibitory);
	c1.set_enable_receiver(true);
	c1.set_row_address_compare_mask(SynapseDriverConfig::RowAddressCompareMask(0b01010));
	c1.set_enable_address_out(false);
	c1.set_utilization(SynapseDriverConfig::Utilization(3));
	c1.set_offset(SynapseDriverConfig::Offset(4));
	c1.set_recovery(SynapseDriverConfig::Recovery(5));
	c1.set_select_target_voltages(SynapseDriverConfig::TargetVoltages(1));
	c1.set_enable_readout(true);
	c1.set_enable_renewing(true);
	c1.set_enable_hagen_modulation(true);
	c1.set_enable_stp(true);
	c1.set_enable_charge_sharing(true);
	c1.set_enable_recovery(true);

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
