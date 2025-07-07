#include "halco/hicann-dls/vx/v3/coordinates.h"
#include "haldls/vx/v3/barrier.h"
#include "haldls/vx/v3/fpga.h"
#include "hwdb4cpp/hwdb4cpp.h"
#include "hxcomm/vx/connection_from_env.h"
#include "stadls/vx/v3/playback_program.h"
#include "stadls/vx/v3/playback_program_builder.h"
#include "stadls/vx/v3/run.h"
#include <gtest/gtest.h>

using namespace haldls::vx::v3;
using namespace stadls::vx::v3;
using namespace halco::hicann_dls::vx::v3;

TEST(FPGADeviceDNA, Read)
{
	PlaybackProgramBuilder builder;

	auto ticket = builder.read(FPGADeviceDNAOnFPGA());

	builder.block_until(BarrierOnFPGA(), Barrier::omnibus);
	auto program = builder.done();

	EXPECT_FALSE(ticket.valid());

	auto connection = hxcomm::vx::get_connection_from_env();
	run(connection, program);

	EXPECT_TRUE(ticket.valid());
	EXPECT_NO_THROW(ticket.get());

	auto const hwdb_entry =
	    std::visit([](auto const& conn) { return conn.get_hwdb_entry(); }, connection);

#if SIMULATION_TEST
	FPGADeviceDNA expectation(FPGADeviceDNA::Value(0x15000000000005D));
#else
	hwdb4cpp::HXCubeFPGAEntry fpga_entry;

	if (std::holds_alternative<hwdb4cpp::HXCubeSetupEntry>(hwdb_entry)) {
		auto cube_entry = std::get<hwdb4cpp::HXCubeSetupEntry>(hwdb_entry);
		EXPECT_EQ(cube_entry.fpgas.size(), 1);
		fpga_entry = cube_entry.fpgas.begin()->second;
	} else if (std::holds_alternative<hwdb4cpp::JboaSetupEntry>(hwdb_entry)) {
		auto jboa_entry = std::get<hwdb4cpp::JboaSetupEntry>(hwdb_entry);
		EXPECT_EQ(jboa_entry.fpgas.size(), 1);
		fpga_entry = jboa_entry.fpgas.begin()->second;
	} else {
		throw std::runtime_error("Connection does not support this test.");
	}

	FPGADeviceDNA expectation(FPGADeviceDNA::Value(fpga_entry.get_dna_port()));
#endif

	EXPECT_EQ(dynamic_cast<FPGADeviceDNA const&>(ticket.get()), expectation);
}
