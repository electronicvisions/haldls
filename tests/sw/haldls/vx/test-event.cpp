#include <gtest/gtest.h>

#include "halco/hicann-dls/vx/event.h"
#include "haldls/cerealization.tcc"
#include "haldls/vx/event.h"
#include "haldls/vx/neuron.h"
#include "haldls/vx/padi.h"
#include "haldls/vx/synapse.h"

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
		NeuronBackendAddressOut const value(243);
		config.set_neuron_backend_address_out(value);
		EXPECT_EQ(config.get_neuron_backend_address_out(), value);
	}

	{
		NeuronEventOutputOnDLS const value(halco::common::Enum(5));
		config.set_neuron_event_output(value);
		EXPECT_EQ(config.get_neuron_event_output(), value);
	}

	{
		PADIEvent::RowSelectAddress const value(23);
		config.set_row_select_address(value);
		EXPECT_EQ(config.get_row_select_address(), value);
	}

	{
		SynapseLabel const value(23);
		config.set_synapse_label(value);
		EXPECT_EQ(config.get_synapse_label(), value);
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
		config.label = value;
		EXPECT_EQ(config.label, value);
	}

	{
		FPGATime const value(3);
		config.fpga_time = value;
		EXPECT_EQ(config.fpga_time, value);
	}

	{
		ChipTime const value(3);
		config.chip_time = value;
		EXPECT_EQ(config.chip_time, value);
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

	obj1.label = SpikeLabel(12);
	obj1.fpga_time = FPGATime(3);
	obj1.chip_time = ChipTime(6);

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

TEST(HighspeedLinkNotification, General)
{
	EXPECT_NO_THROW(HighspeedLinkNotification());
	EXPECT_NO_THROW(HighspeedLinkNotification(fisch::vx::HighspeedLinkNotification()));

	HighspeedLinkNotification config;
	HighspeedLinkNotification other_config = config;

	EXPECT_EQ(other_config, config);

	{
		halco::hicann_dls::vx::PhyStatusOnFPGA const value(3);
		config.set_phy(value);
		EXPECT_EQ(config.get_phy(), value);
	}

	{
		bool const value = !config.get_link_up();
		config.set_link_up(value);
		EXPECT_EQ(config.get_link_up(), value);
	}

	{
		bool const value = !config.get_decode_error();
		config.set_decode_error(value);
		EXPECT_EQ(config.get_decode_error(), value);
	}

	{
		bool const value = !config.get_crc_error();
		config.set_crc_error(value);
		EXPECT_EQ(config.get_crc_error(), value);
	}

	{
		bool const value = !config.get_crc_recover();
		config.set_crc_recover(value);
		EXPECT_EQ(config.get_crc_recover(), value);
	}

	{
		bool const value = !config.get_check_error();
		config.set_check_error(value);
		EXPECT_EQ(config.get_check_error(), value);
	}

	{
		FPGATime const value(3);
		config.set_fpga_time(value);
		EXPECT_EQ(config.get_fpga_time(), value);
	}

	EXPECT_NE(config, other_config);

	HighspeedLinkNotification config3 = config;
	EXPECT_EQ(config, config3);
}

TEST(HighspeedLinkNotification, Ostream)
{
	HighspeedLinkNotification obj(fisch::vx::HighspeedLinkNotification(
	    fisch::vx::HighspeedLinkNotification::Value(0b10010100), FPGATime(15)));

	std::stringstream stream;
	stream << obj;

	EXPECT_EQ(
	    stream.str(),
	    "HighspeedLinkNotification(PhyStatusOnFPGA(4), FPGATime(15): link up, crc error)");
}

TEST(HighspeedLinkNotification, CerealizeCoverage)
{
	HighspeedLinkNotification obj1, obj2;

	obj1.set_phy(halco::hicann_dls::vx::PhyStatusOnFPGA(4));
	obj1.set_fpga_time(FPGATime(3));
	obj1.set_link_up(!obj1.get_link_up());
	obj1.set_decode_error(!obj1.get_decode_error());
	obj1.set_crc_error(!obj1.get_crc_error());
	obj1.set_crc_recover(!obj1.get_crc_recover());
	obj1.set_check_error(!obj1.get_check_error());

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
