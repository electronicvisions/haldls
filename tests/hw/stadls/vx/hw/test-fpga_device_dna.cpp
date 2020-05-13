#include <gtest/gtest.h>

#include "haldls/vx/v1/barrier.h"
#include "haldls/vx/v1/fpga.h"
#include "stadls/vx/v1/playback_program.h"
#include "stadls/vx/v1/playback_program_builder.h"
#include "stadls/vx/v1/run.h"

#include "connection.h"

using namespace haldls::vx::v1;
using namespace stadls::vx::v1;
using namespace halco::hicann_dls::vx;

TEST(FPGADeviceDNA, Read)
{
	PlaybackProgramBuilder builder;

	auto ticket = builder.read(FPGADeviceDNAOnFPGA());

	builder.block_until(BarrierOnFPGA(), Barrier::omnibus);
	auto program = builder.done();

	EXPECT_FALSE(ticket.valid());

	auto connection = generate_test_connection();
	run(connection, program);

	EXPECT_TRUE(ticket.valid());
	EXPECT_NO_THROW(ticket.get());

	// FIXME: Issue #3376 hardcoded ID for wafer 62
	EXPECT_EQ(ticket.get(), FPGADeviceDNA(FPGADeviceDNA::Value(0x74'1d25'8805'1664ull)));
}
