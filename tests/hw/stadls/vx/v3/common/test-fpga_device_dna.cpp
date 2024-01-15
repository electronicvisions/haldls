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

	auto const connection_unique_identifier =
	    std::visit([](auto const& conn) { return conn.get_unique_identifier(); }, connection);

#if SIMULATION_TEST
	FPGADeviceDNA expectation(FPGADeviceDNA::Value(0x15000000000005D));
#else
	auto const [hxcube_id, fpga_id, _, __] =
	    hwdb4cpp::HXCubeSetupEntry::get_ids_from_unique_branch_identifier(
	        connection_unique_identifier);

	hwdb4cpp::database hwdb;
	hwdb.load(hwdb4cpp::database::get_default_path());
	auto const& hxcube_setup_entry = hwdb.get_hxcube_setup_entry(hxcube_id);
	if (!hxcube_setup_entry.fpgas.contains(fpga_id)) {
		throw std::runtime_error("HXCubeSetupEntry doesn't feature FPGA ID from connection.");
	}
	FPGADeviceDNA expectation(
	    FPGADeviceDNA::Value(hxcube_setup_entry.fpgas.at(fpga_id).get_dna_port()));
#endif

	EXPECT_EQ(dynamic_cast<FPGADeviceDNA const&>(ticket.get()), expectation);
}
