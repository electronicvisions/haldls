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

	auto const hwdb_entry =
	    std::visit([](auto const& conn) { return conn.get_hwdb_entry(); }, connection);

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

	if (fpga_entry.wing.value().handwritten_chip_serial < 75) {
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
