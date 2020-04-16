#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "fisch/vx/jtag.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/vx/common.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/synapse_driver.h"
#include "test-helper.h"

using namespace haldls::vx;
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
