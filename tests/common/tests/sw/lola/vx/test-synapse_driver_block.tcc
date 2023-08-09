#ifndef CHIP_REVISION
#error "Requires CHIP_REVISION"
#endif

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/common.h"
#include "lola/vx/synapse_driver.h"
#include "stadls/visitors.h"
#include "test-helper.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#if CHIP_REVISION == 3
#define CHIP_REVISION_STR v3
#include "cereal/types/lola/vx/v3/synapse_driver.h"
#elif CHIP_REVISION == 4
#define CHIP_REVISION_STR v4
#include "cereal/types/lola/vx/v4/synapse_driver.h"
#else
#error "Unknown CHIP_REVISION"
#endif

using namespace haldls::vx;
using namespace lola::vx::CHIP_REVISION_STR;
using namespace halco::hicann_dls::vx::CHIP_REVISION_STR;
using namespace halco::common;

TEST(SynapseDriverBlock, General)
{
	{
		SynapseDriverBlock config;
		config.synapse_drivers[SynapseDriverOnSynapseDriverBlock()].set_enable_receiver(
		    !config.synapse_drivers[SynapseDriverOnSynapseDriverBlock()].get_enable_receiver());
		EXPECT_NE(config, SynapseDriverBlock());
	}
	{
		SynapseDriverBlock config;
		auto enables = config.padi_bus.get_enable_spl1();
		enables[PADIBusOnPADIBusBlock()] = !enables[PADIBusOnPADIBusBlock()];
		config.padi_bus.set_enable_spl1(enables);
		EXPECT_NE(config, SynapseDriverBlock());
	}
	{
		SynapseDriverBlock config;
		config.stp.set_enable_recovery_clock(!config.stp.get_enable_recovery_clock());
		EXPECT_NE(config, SynapseDriverBlock());
	}
	{
		SynapseDriverBlock config;
		config.stp_v_charge_0[CapMemBlockOnHemisphere(0)] = SynapseDriverBlock::AnalogValue(123);
		EXPECT_NE(config, SynapseDriverBlock());
	}
	{
		SynapseDriverBlock config;
		config.stp_v_charge_0[CapMemBlockOnHemisphere(1)] = SynapseDriverBlock::AnalogValue(321);
		EXPECT_NE(config, SynapseDriverBlock());
	}
	{
		SynapseDriverBlock config;
		config.stp_v_charge_1[CapMemBlockOnHemisphere(0)] = SynapseDriverBlock::AnalogValue(123);
		EXPECT_NE(config, SynapseDriverBlock());
	}
	{
		SynapseDriverBlock config;
		config.stp_v_charge_1[CapMemBlockOnHemisphere(1)] = SynapseDriverBlock::AnalogValue(321);
		EXPECT_NE(config, SynapseDriverBlock());
	}
	{
		SynapseDriverBlock config;
		config.stp_v_recover_0[CapMemBlockOnHemisphere(0)] = SynapseDriverBlock::AnalogValue(123);
		EXPECT_NE(config, SynapseDriverBlock());
	}
	{
		SynapseDriverBlock config;
		config.stp_v_recover_0[CapMemBlockOnHemisphere(1)] = SynapseDriverBlock::AnalogValue(321);
		EXPECT_NE(config, SynapseDriverBlock());
	}
	{
		SynapseDriverBlock config;
		config.stp_v_recover_1[CapMemBlockOnHemisphere(0)] = SynapseDriverBlock::AnalogValue(123);
		EXPECT_NE(config, SynapseDriverBlock());
	}
	{
		SynapseDriverBlock config;
		config.stp_v_recover_1[CapMemBlockOnHemisphere(1)] = SynapseDriverBlock::AnalogValue(321);
		EXPECT_NE(config, SynapseDriverBlock());
	}
	{
		SynapseDriverBlock config;
		config.stp_i_bias_comparator[CapMemBlockOnHemisphere(0)] =
		    SynapseDriverBlock::AnalogValue(123);
		EXPECT_NE(config, SynapseDriverBlock());
	}
	{
		SynapseDriverBlock config;
		config.stp_i_bias_comparator[CapMemBlockOnHemisphere(1)] =
		    SynapseDriverBlock::AnalogValue(321);
		EXPECT_NE(config, SynapseDriverBlock());
	}
	{
		SynapseDriverBlock config;
		config.stp_i_ramp[CapMemBlockOnHemisphere(0)] = SynapseDriverBlock::AnalogValue(123);
		EXPECT_NE(config, SynapseDriverBlock());
	}
	{
		SynapseDriverBlock config;
		config.stp_i_ramp[CapMemBlockOnHemisphere(1)] = SynapseDriverBlock::AnalogValue(321);
		EXPECT_NE(config, SynapseDriverBlock());
	}
	{
		SynapseDriverBlock config;
		config.stp_i_calib[CapMemBlockOnHemisphere(0)] = SynapseDriverBlock::AnalogValue(123);
		EXPECT_NE(config, SynapseDriverBlock());
	}
	{
		SynapseDriverBlock config;
		config.stp_i_calib[CapMemBlockOnHemisphere(1)] = SynapseDriverBlock::AnalogValue(321);
		EXPECT_NE(config, SynapseDriverBlock());
	}
	{
		SynapseDriverBlock config;
		config.stp_i_bias_readout = SynapseDriverBlock::AnalogValue(123);
		EXPECT_NE(config, SynapseDriverBlock());
	}
	{
		SynapseDriverBlock config;
		config.hagen_i_bias_dac = SynapseDriverBlock::AnalogValue(321);
		EXPECT_NE(config, SynapseDriverBlock());
	}
}

TEST(SynapseDriverBlock, CerealizeCoverage)
{
	SynapseDriverBlock obj1, obj2;

	obj1.synapse_drivers[SynapseDriverOnSynapseDriverBlock()].set_enable_receiver(
	    !obj1.synapse_drivers[SynapseDriverOnSynapseDriverBlock()].get_enable_receiver());
	auto enables = obj1.padi_bus.get_enable_spl1();
	enables[PADIBusOnPADIBusBlock()] = !enables[PADIBusOnPADIBusBlock()];
	obj1.padi_bus.set_enable_spl1(enables);
	obj1.stp.set_enable_recovery_clock(!obj1.stp.get_enable_recovery_clock());
	obj1.stp_v_charge_0[CapMemBlockOnHemisphere(0)] = SynapseDriverBlock::AnalogValue(123);
	obj1.stp_v_charge_0[CapMemBlockOnHemisphere(1)] = SynapseDriverBlock::AnalogValue(234);
	obj1.stp_v_charge_1[CapMemBlockOnHemisphere(0)] = SynapseDriverBlock::AnalogValue(456);
	obj1.stp_v_charge_1[CapMemBlockOnHemisphere(1)] = SynapseDriverBlock::AnalogValue(567);
	obj1.stp_v_recover_0[CapMemBlockOnHemisphere(0)] = SynapseDriverBlock::AnalogValue(678);
	obj1.stp_v_recover_0[CapMemBlockOnHemisphere(1)] = SynapseDriverBlock::AnalogValue(789);
	obj1.stp_v_recover_1[CapMemBlockOnHemisphere(0)] = SynapseDriverBlock::AnalogValue(321);
	obj1.stp_v_recover_1[CapMemBlockOnHemisphere(1)] = SynapseDriverBlock::AnalogValue(432);
	obj1.stp_i_bias_comparator[CapMemBlockOnHemisphere(0)] = SynapseDriverBlock::AnalogValue(543);
	obj1.stp_i_bias_comparator[CapMemBlockOnHemisphere(1)] = SynapseDriverBlock::AnalogValue(654);
	obj1.stp_i_ramp[CapMemBlockOnHemisphere(0)] = SynapseDriverBlock::AnalogValue(765);
	obj1.stp_i_ramp[CapMemBlockOnHemisphere(1)] = SynapseDriverBlock::AnalogValue(876);
	obj1.stp_i_calib[CapMemBlockOnHemisphere(0)] = SynapseDriverBlock::AnalogValue(987);
	obj1.stp_i_calib[CapMemBlockOnHemisphere(1)] = SynapseDriverBlock::AnalogValue(222);
	obj1.stp_i_bias_readout = SynapseDriverBlock::AnalogValue(222);
	obj1.hagen_i_bias_dac = SynapseDriverBlock::AnalogValue(333);

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

TEST(SynapseDriverBlock, EncodeDecode)
{
	SynapseDriverBlock config;
	config.synapse_drivers[SynapseDriverOnSynapseDriverBlock()].set_enable_receiver(
	    !config.synapse_drivers[SynapseDriverOnSynapseDriverBlock()].get_enable_receiver());
	auto enables = config.padi_bus.get_enable_spl1();
	enables[PADIBusOnPADIBusBlock()] = !enables[PADIBusOnPADIBusBlock()];
	config.padi_bus.set_enable_spl1(enables);
	config.stp.set_enable_recovery_clock(!config.stp.get_enable_recovery_clock());
	config.stp_v_charge_0[CapMemBlockOnHemisphere(0)] = SynapseDriverBlock::AnalogValue(123);
	config.stp_v_charge_0[CapMemBlockOnHemisphere(1)] = SynapseDriverBlock::AnalogValue(234);
	config.stp_v_charge_1[CapMemBlockOnHemisphere(0)] = SynapseDriverBlock::AnalogValue(456);
	config.stp_v_charge_1[CapMemBlockOnHemisphere(1)] = SynapseDriverBlock::AnalogValue(567);
	config.stp_v_recover_0[CapMemBlockOnHemisphere(0)] = SynapseDriverBlock::AnalogValue(678);
	config.stp_v_recover_0[CapMemBlockOnHemisphere(1)] = SynapseDriverBlock::AnalogValue(789);
	config.stp_v_recover_1[CapMemBlockOnHemisphere(0)] = SynapseDriverBlock::AnalogValue(321);
	config.stp_v_recover_1[CapMemBlockOnHemisphere(1)] = SynapseDriverBlock::AnalogValue(432);
	config.stp_i_bias_comparator[CapMemBlockOnHemisphere(0)] =
	    SynapseDriverBlock::AnalogValue(543);
	config.stp_i_bias_comparator[CapMemBlockOnHemisphere(1)] =
	    SynapseDriverBlock::AnalogValue(654);
	config.stp_i_ramp[CapMemBlockOnHemisphere(0)] = SynapseDriverBlock::AnalogValue(765);
	config.stp_i_ramp[CapMemBlockOnHemisphere(1)] = SynapseDriverBlock::AnalogValue(876);
	config.stp_i_calib[CapMemBlockOnHemisphere(0)] = SynapseDriverBlock::AnalogValue(987);
	config.stp_i_calib[CapMemBlockOnHemisphere(1)] = SynapseDriverBlock::AnalogValue(222);
	config.stp_i_bias_readout = SynapseDriverBlock::AnalogValue(222);
	config.hagen_i_bias_dac = SynapseDriverBlock::AnalogValue(333);

	typedef std::vector<OmnibusAddress> addresses_type;
	typedef std::vector<fisch::vx::word_access_type::Omnibus> words_type;

	SynapseDriverBlockOnDLS coord;

	addresses_type read_addresses;
	addresses_type write_addresses;
	visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
	visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
	EXPECT_THAT(read_addresses, write_addresses);

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});

	SynapseDriverBlock config_decoded;
	visit_preorder(config_decoded, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	EXPECT_EQ(config_decoded, config);
}
