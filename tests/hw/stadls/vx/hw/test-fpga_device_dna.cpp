#include <gtest/gtest.h>

#include "haldls/vx/fpga.h"

#include "haldls/vx/timer.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.h"

#include "executor.h"

using namespace haldls::vx;
using namespace stadls::vx;
using namespace halco::hicann_dls::vx;

TEST(FPGADeviceDNA, Read)
{
	PlaybackProgramBuilder builder;

	auto ticket = builder.read(FPGADeviceDNAOnFPGA());

	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(100));
	auto program = builder.done();

	EXPECT_FALSE(ticket.valid());

	auto executor = generate_playback_program_test_executor();
	executor.run(program);

	EXPECT_TRUE(ticket.valid());
	EXPECT_NO_THROW(ticket.get());

	// FIXME: Issue #3376 hardcoded ID for wafer 62
	EXPECT_EQ(ticket.get(), FPGADeviceDNA(FPGADeviceDNA::Value(0x74'1d25'8805'1664ull)));
}
