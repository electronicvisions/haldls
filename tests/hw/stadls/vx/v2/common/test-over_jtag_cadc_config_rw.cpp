#include <gtest/gtest.h>

#include "fisch/vx/constants.h"
#include "halco/hicann-dls/vx/v2/coordinates.h"
#include "haldls/vx/v2/barrier.h"
#include "haldls/vx/v2/cadc.h"
#include "haldls/vx/v2/jtag.h"
#include "haldls/vx/v2/pll.h"
#include "haldls/vx/v2/reset.h"
#include "haldls/vx/v2/traits.h"
#include "stadls/vx/v2/init_generator.h"
#include "stadls/vx/v2/playback_program.h"
#include "stadls/vx/v2/playback_program_builder.h"
#include "stadls/vx/v2/run.h"

#include "connection.h"
#include "test-helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx::v2;
using namespace haldls::vx::v2;
using namespace stadls::vx::v2;

/**
 * Enable over JTAG omnibus connection and write and read the CADC config for verification.
 */
TEST(CADCConfig, WROverJTAG)
{
	auto sequence = DigitalInit();
	sequence.enable_highspeed_link = false;
	auto [builder, _] = generate(sequence);

	// Write unique configs
	std::vector<CADCConfig> configs;
	for (auto coord : iter_all<CADCConfigOnDLS>()) {
		CADCConfig config;
		config.set_enable(coord);
		config.set_reset_wait(
		    draw_ranged_non_default_value<CADCConfig::ResetWait>(config.get_reset_wait()));
		config.set_dead_time(
		    draw_ranged_non_default_value<CADCConfig::DeadTime>(config.get_dead_time()));
		builder.write(coord, config, Backend::OmnibusChipOverJTAG);
		configs.push_back(config);
	}

	// Read configs with Omnibus over JTAG backend
	std::vector<PlaybackProgram::ContainerTicket<CADCConfig>> responses;
	for (auto coord : iter_all<CADCConfigOnDLS>()) {
		responses.push_back(builder.read(coord, Backend::OmnibusChipOverJTAG));
	}

	builder.block_until(BarrierOnFPGA(), Barrier::jtag);
	auto program = builder.done();

	auto connection = generate_test_connection();
	run(connection, program);

	for (auto coord : iter_all<CADCConfigOnDLS>()) {
		EXPECT_TRUE(responses.at(coord).valid());
		EXPECT_EQ(responses.at(coord).get(), configs.at(coord));
	}
}
