#include "haldls/vx/container.h"
#include "haldls/vx/v3/event.h"
#include "stadls/vx/v3/absolute_time_playback_program_builder.h"

#include <gtest/gtest.h>
#include <log4cxx/logger.h>

TEST(AbsoluteTimePlaybackProgramBuilder, general)
{
	auto logger = log4cxx::Logger::getLogger("test.AbsoluteTimePlaybackProgramBuilder");
	stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder builder =
	    stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder();

	// testing the empty function
	EXPECT_EQ(builder.empty(), true);

	// testing the << operator
	LOG4CXX_DEBUG(logger, "builder's contents are: \n" << builder << "\n");

	// testing the is_write_only function
	EXPECT_EQ(builder.is_write_only(), true);

	// testing the merge function
	stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder builder2 =
	    stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder();
	halco::hicann_dls::vx::v3::SpikeLabel label2(0);
	haldls::vx::v3::SpikePack1ToChip spike2({label2});
	builder2.write(
	    haldls::vx::v3::Timer::Value(10), halco::hicann_dls::vx::v3::SpikePack1ToChipOnDLS(),
	    spike2);

	builder.merge(builder2);

	LOG4CXX_DEBUG(logger, "builder's contents are now \n" << builder << "\n");
	EXPECT_EQ(builder2.empty(), true);

	// testing the write function
	stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder builder3 =
	    stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder();
	halco::hicann_dls::vx::v3::SpikeLabel label3(0);
	haldls::vx::v3::SpikePack1ToChip spike3({label3});
	builder3.write(
	    haldls::vx::v3::Timer::Value(20), halco::hicann_dls::vx::v3::SpikePack1ToChipOnDLS(),
	    spike3);

	// testing the copy function
	builder.copy(builder3);
	EXPECT_EQ(builder3.empty(), false);

	EXPECT_NE(builder.done().done(), builder3.done().done());

	LOG4CXX_DEBUG(logger, "builder's contents are now: \n" << builder << "\n");
	LOG4CXX_DEBUG(logger, "builder3's contents are now: \n" << builder3 << "\n");
	EXPECT_EQ(builder3.empty(), true);

	// testing the ban on accessing the FPGA-Timer
	EXPECT_THROW(
	    builder3.write(
	        haldls::vx::v3::Timer::Value(10), halco::hicann_dls::vx::v3::TimerOnDLS(),
	        haldls::vx::v3::Timer()),
	    std::runtime_error);

	// testing the offset operators + and +=
	stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder builder4 =
	    stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder();
	halco::hicann_dls::vx::v3::SpikeLabel label4(0);
	haldls::vx::v3::SpikePack1ToChip spike4({label4});
	builder4.write(
	    haldls::vx::v3::Timer::Value(0), halco::hicann_dls::vx::v3::SpikePack1ToChipOnDLS(),
	    spike4);

	stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder builder5 =
	    builder4 + haldls::vx::v3::Timer::Value(10);

	builder4 += haldls::vx::v3::Timer::Value(10);

	stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder builder6 =
	    stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder();
	builder6.write(
	    haldls::vx::v3::Timer::Value(10), halco::hicann_dls::vx::v3::SpikePack1ToChipOnDLS(),
	    spike4);

	auto program4 = builder4.done().done();
	auto program5 = builder5.done().done();
	auto program6 = builder6.done().done();

	EXPECT_EQ(program4, program5);
	EXPECT_EQ(program4, program6);
}
