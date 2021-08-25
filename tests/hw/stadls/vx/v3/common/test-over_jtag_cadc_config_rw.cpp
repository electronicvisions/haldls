#include <gtest/gtest.h>

#include "fisch/vx/constants.h"
#include "halco/hicann-dls/vx/v3/coordinates.h"
#include "haldls/vx/v3/barrier.h"
#include "haldls/vx/v3/cadc.h"
#include "haldls/vx/v3/jtag.h"
#include "haldls/vx/v3/pll.h"
#include "haldls/vx/v3/reset.h"
#include "haldls/vx/v3/traits.h"
#include "stadls/vx/v3/init_generator.h"
#include "stadls/vx/v3/playback_program.h"
#include "stadls/vx/v3/playback_program_builder.h"
#include "stadls/vx/v3/run.h"

#include "hxcomm/vx/connection_from_env.h"
#include "test-helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx::v3;
using namespace haldls::vx::v3;
using namespace stadls::vx::v3;

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

	auto connection = hxcomm::vx::get_connection_from_env();
	run(connection, program);

	for (auto coord : iter_all<CADCConfigOnDLS>()) {
		EXPECT_TRUE(responses.at(coord).valid());
		EXPECT_EQ(responses.at(coord).get(), configs.at(coord));
	}
}
