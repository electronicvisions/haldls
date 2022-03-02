#include <gtest/gtest.h>

#include "haldls/vx/v1/fpga.h"
#include "lola/vx/v1/chip.h"
#include "stadls/vx/v1/decode.h"
#include "stadls/vx/v1/dumper.h"
#include <random>

TEST(convert_to_chip, General)
{
	std::mt19937 rng(1234);
	lola::vx::v1::Chip previous;
	stadls::vx::decode_random<lola::vx::v1::Chip>(rng, previous);

	{
		auto converted = stadls::vx::v1::convert_to_chip(stadls::vx::v1::DumperDone(), previous);
		EXPECT_EQ(converted, previous);
	}
	{
		stadls::vx::v1::PlaybackProgramBuilderDumper dumper;
		dumper.write(halco::hicann_dls::vx::v1::ChipOnDLS(), previous);
		auto dumperdone = dumper.done();
		auto converted = stadls::vx::v1::convert_to_chip(dumperdone);
		EXPECT_EQ(converted, previous);
	}
	{
		stadls::vx::v1::PlaybackProgramBuilderDumper dumper;
		// some Omnibus config, which is not part of the Chip
		dumper.write(
		    halco::hicann_dls::vx::v1::EventRecordingConfigOnFPGA(),
		    haldls::vx::v1::EventRecordingConfig());
		auto dumperdone = dumper.done();
		// expect warning
		auto converted = stadls::vx::v1::convert_to_chip(dumperdone);
		// chip should not have been touched
		EXPECT_EQ(converted, lola::vx::v1::Chip());
	}
}
