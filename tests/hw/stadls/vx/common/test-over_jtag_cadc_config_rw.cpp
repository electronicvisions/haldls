#include <gtest/gtest.h>

#include "fisch/vx/constants.h"
#include "haldls/vx/barrier.h"
#include "haldls/vx/cadc.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/pll.h"
#include "haldls/vx/reset.h"
#include "stadls/vx/init_generator.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.h"
#include "stadls/vx/run.h"

#include "connection.h"
#include "test-helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx;
using namespace stadls::vx;

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
