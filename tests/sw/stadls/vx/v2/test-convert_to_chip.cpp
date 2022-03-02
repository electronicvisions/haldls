#include <gtest/gtest.h>

#include "haldls/vx/v2/fpga.h"
#include "lola/vx/v2/chip.h"
#include "stadls/vx/v2/decode.h"
#include "stadls/vx/v2/dumper.h"
#include <random>

TEST(convert_to_chip, General)
{
	std::mt19937 rng(1234);
	lola::vx::v2::Chip previous;
	stadls::vx::decode_random<lola::vx::v2::Chip>(rng, previous);

	{
		auto converted = stadls::vx::v2::convert_to_chip(stadls::vx::v2::DumperDone(), previous);
		EXPECT_EQ(converted, previous);
	}
	{
		stadls::vx::v2::PlaybackProgramBuilderDumper dumper;
		dumper.write(halco::hicann_dls::vx::v2::ChipOnDLS(), previous);
		auto dumperdone = dumper.done();
		auto converted = stadls::vx::v2::convert_to_chip(dumperdone);
		EXPECT_EQ(converted, previous);
	}
	{
		stadls::vx::v2::PlaybackProgramBuilderDumper dumper;
		// some Omnibus config, which is not part of the Chip
		dumper.write(
		    halco::hicann_dls::vx::v2::EventRecordingConfigOnFPGA(),
		    haldls::vx::v2::EventRecordingConfig());
		auto dumperdone = dumper.done();
		// expect warning
		auto converted = stadls::vx::v2::convert_to_chip(dumperdone);
		// chip should not have been touched
		EXPECT_EQ(converted, lola::vx::v2::Chip());
	}
}
