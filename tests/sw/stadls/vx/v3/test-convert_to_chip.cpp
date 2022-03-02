#include <gtest/gtest.h>

#include "haldls/vx/v3/fpga.h"
#include "lola/vx/v3/chip.h"
#include "stadls/vx/v3/decode.h"
#include "stadls/vx/v3/dumper.h"
#include <random>

TEST(convert_to_chip, General)
{
	std::mt19937 rng(1234);
	lola::vx::v3::Chip previous;
	stadls::vx::decode_random<lola::vx::v3::Chip>(rng, previous);

	{
		auto converted = stadls::vx::v3::convert_to_chip(stadls::vx::v3::DumperDone(), previous);
		EXPECT_EQ(converted, previous);
	}
	{
		stadls::vx::v3::PlaybackProgramBuilderDumper dumper;
		dumper.write(halco::hicann_dls::vx::v3::ChipOnDLS(), previous);
		auto dumperdone = dumper.done();
		auto converted = stadls::vx::v3::convert_to_chip(dumperdone);
		EXPECT_EQ(converted, previous);
	}
	{
		stadls::vx::v3::PlaybackProgramBuilderDumper dumper;
		// some Omnibus config, which is not part of the Chip
		dumper.write(
		    halco::hicann_dls::vx::v3::EventRecordingConfigOnFPGA(),
		    haldls::vx::v3::EventRecordingConfig());
		auto dumperdone = dumper.done();
		// expect warning
		auto converted = stadls::vx::v3::convert_to_chip(dumperdone);
		// chip should not have been touched
		EXPECT_EQ(converted, lola::vx::v3::Chip());
	}
}
