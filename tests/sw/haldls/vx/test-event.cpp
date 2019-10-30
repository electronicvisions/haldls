#include <gtest/gtest.h>

#include "haldls/cerealization.h"
#include "haldls/vx/event.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;

TEST(SpikeLabel, General)
{
	EXPECT_NO_THROW(SpikeLabel(0x1234));

	SpikeLabel config;
	SpikeLabel other_config = config;

	EXPECT_EQ(other_config, config);

	{
		SPL1Address const value(2);
		config.set_spl1_address(value);
		EXPECT_EQ(config.get_spl1_address(), value);
	}

	{
		NeuronLabel const value(123);
		config.set_neuron_label(value);
		EXPECT_EQ(config.get_neuron_label(), value);
	}

	{
		SpikeLabel::PADILabel const value(321);
		config.set_padi_label(value);
		EXPECT_EQ(config.get_padi_label(), value);
	}

	{
		NeuronBackendConfig::AddressOut const value(243);
		config.set_neuron_backend_address_out(value);
		EXPECT_EQ(config.get_neuron_backend_address_out(), value);
	}

	{
		SynapseQuad::Synapse::Address const value(23);
		config.set_synapse_address(value);
		EXPECT_EQ(config.get_synapse_address(), value);
	}

	EXPECT_NE(config, other_config);

	SpikeLabel config3 = config;
	EXPECT_EQ(config, config3);
}

TEST(SpikeFromChip, General)
{
	EXPECT_NO_THROW(SpikeFromChip());
	EXPECT_NO_THROW(SpikeFromChip(SpikeLabel(), FPGATime(), ChipTime()));

	SpikeFromChip config;
	SpikeFromChip other_config = config;

	EXPECT_EQ(other_config, config);

	{
		SpikeLabel const value(12);
		config.set_label(value);
		EXPECT_EQ(config.get_label(), value);
	}

	{
		FPGATime const value(3);
		config.set_fpga_time(value);
		EXPECT_EQ(config.get_fpga_time(), value);
	}

	{
		ChipTime const value(3);
		config.set_chip_time(value);
		EXPECT_EQ(config.get_chip_time(), value);
	}

	EXPECT_NE(config, other_config);

	SpikeFromChip config3 = config;
	EXPECT_EQ(config, config3);
}

TEST(SpikeFromChip, Ostream)
{
	SpikeFromChip obj(SpikeLabel(12), FPGATime(6), ChipTime(3));

	std::stringstream stream;
	stream << obj;

	EXPECT_EQ(stream.str(), "SpikeFromChip(SpikeLabel(12), FPGATime(6), ChipTime(3))");
}

TEST(SpikeFromChip, CerealizeCoverage)
{
	SpikeFromChip obj1, obj2;

	obj1.set_label(SpikeLabel(12));
	obj1.set_fpga_time(FPGATime(3));
	obj1.set_chip_time(ChipTime(6));

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
