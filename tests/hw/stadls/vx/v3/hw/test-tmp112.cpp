#include "halco/hicann-dls/vx/v3/coordinates.h"
#include "haldls/vx/v3/barrier.h"
#include "haldls/vx/v3/i2c.h"
#include "hwdb4cpp/hwdb4cpp.h"
#include "hxcomm/vx/connection_from_env.h"
#include "stadls/vx/v3/container_ticket.h"
#include "stadls/vx/v3/init_generator.h"
#include "stadls/vx/v3/playback_program.h"
#include "stadls/vx/v3/playback_program_builder.h"
#include "stadls/vx/v3/run.h"
#include <gtest/gtest.h>

using namespace halco::common;
using namespace halco::hicann_dls::vx::v3;
using namespace haldls::vx::v3;
using namespace stadls::vx::v3;

TEST(TMP112, Temperature)
{
	// no xboard/digital-init required since TMP112 on chip carrier operates on standby voltage
	auto builder = PlaybackProgramBuilder();

	auto temp = builder.read(TMP112StatusOnBoard());

	builder.block_until(BarrierOnFPGA(), Barrier::omnibus);
	auto program = builder.done();

	auto connection = hxcomm::vx::get_connection_from_env();
	run(connection, program);

	EXPECT_TRUE(temp.valid());

	auto const connection_unique_identifier =
	    std::visit([](auto const& conn) { return conn.get_unique_identifier(); }, connection);

	auto const [hxcube_id, fpga_id, _, __] =
	    hwdb4cpp::HXCubeSetupEntry::get_ids_from_unique_branch_identifier(
	        connection_unique_identifier);

	hwdb4cpp::database hwdb;
	hwdb.load(hwdb4cpp::database::get_default_path());
	auto& hxcube_setup_entry = hwdb.get_hxcube_setup_entry(hxcube_id);
	if (!hxcube_setup_entry.fpgas.contains(fpga_id)) {
		throw std::runtime_error("HXCubeSetupEntry doesn't feature FPGA ID from connection.");
	}
	if (!hxcube_setup_entry.fpgas[fpga_id].wing.has_value()) {
		throw std::runtime_error("HXCubeFPGAEntry doesn't feature wing.");
	}
	if (hxcube_setup_entry.fpgas[fpga_id].wing.value().handwritten_chip_serial < 75) {
		// old chip carrier version, no temperature sensor available
		EXPECT_ANY_THROW(temp.get());
		return;
	}

	EXPECT_NO_THROW(temp.get());

	float const temp_degc =
	    dynamic_cast<TMP112Status const&>(temp.get()).get_temperature().toUncalibratedTemperature();
	EXPECT_LE(temp_degc, 40.0);
	EXPECT_GE(temp_degc, 15.0);
}
