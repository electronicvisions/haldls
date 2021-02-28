#include <gtest/gtest.h>

#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/v2/coordinates.h"
#include "haldls/vx/v2/barrier.h"
#include "haldls/vx/v2/jtag.h"
#include "haldls/vx/v2/reset.h"
#include "haldls/vx/v2/synapse.h"
#include "haldls/vx/v2/traits.h"
#include "stadls/vx/v2/init_generator.h"
#include "stadls/vx/v2/playback_program.h"
#include "stadls/vx/v2/playback_program_builder.h"
#include "stadls/vx/v2/run.h"

#include "hxcomm/vx/connection_from_env.h"
#include "test-helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx::v2;
using namespace haldls::vx::v2;
using namespace stadls::vx::v2;

/**
 * Enable over JTAG omnibus connection and write and read all Synram configs for verification.
 */
TEST(CommonSynramConfig, WROverJTAG)
{
	auto sequence = DigitalInit();
	sequence.enable_highspeed_link = false;
	auto [builder, _] = generate(sequence);

	typed_array<CommonSynramConfig, CommonSynramConfigOnDLS> configs;

	// Fill configs with random data
	for (auto coord : iter_all<CommonSynramConfigOnDLS>()) {
		configs[coord].set_w_conf_west(
		    draw_ranged_non_default_value<decltype(configs[coord].get_w_conf_west())>(
		        configs[coord].get_w_conf_west()));
		configs[coord].set_w_conf_east(
		    draw_ranged_non_default_value<decltype(configs[coord].get_w_conf_east())>(
		        configs[coord].get_w_conf_east()));
		configs[coord].set_pc_conf_west(
		    draw_ranged_non_default_value<decltype(configs[coord].get_pc_conf_west())>(
		        configs[coord].get_pc_conf_west()));
		configs[coord].set_pc_conf_east(
		    draw_ranged_non_default_value<decltype(configs[coord].get_pc_conf_east())>(
		        configs[coord].get_pc_conf_east()));
		configs[coord].set_wait_ctr_clear(
		    draw_ranged_non_default_value<decltype(configs[coord].get_wait_ctr_clear())>(
		        configs[coord].get_wait_ctr_clear()));
	}

	// Write using JTAG
	for (auto coord : iter_all<CommonSynramConfigOnDLS>()) {
		builder.write(coord, configs[coord], Backend::OmnibusChipOverJTAG);
	}

	std::vector<PlaybackProgram::ContainerTicket<CommonSynramConfig>> tickets;
	// Read back
	for (auto coord : iter_all<CommonSynramConfigOnDLS>()) {
		tickets.push_back(builder.read(coord, Backend::OmnibusChipOverJTAG));
	}
	builder.block_until(BarrierOnFPGA(), Barrier::jtag);
	auto program = builder.done();

	auto connection = hxcomm::vx::get_connection_from_env();
	run(connection, program);

	for (auto coord : iter_all<CommonSynramConfigOnDLS>()) {
		EXPECT_TRUE(tickets[coord.toEnum()].valid());
		EXPECT_EQ(tickets[coord].get(), configs[coord]);
	}
}
