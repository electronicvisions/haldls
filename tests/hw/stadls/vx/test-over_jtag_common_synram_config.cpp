#include <gtest/gtest.h>

#include "halco/common/typed_array.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/synapse.h"
#include "haldls/vx/timer.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.h"

#include "executor.h"
#include "test-helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx;
using namespace stadls::vx;

/**
 * Enable over JTAG omnibus connection and write and read all Synram configs for verification.
 */
TEST(CommonSynramConfig, WROverJTAG)
{
	PlaybackProgramBuilder builder;

	// Chip reset
	builder.write(ResetChipOnDLS(), ResetChip(true));
	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(10));
	builder.write(ResetChipOnDLS(), ResetChip(false));
	builder.wait_until(TimerOnDLS(), Timer::Value(100));

	// JTAG init
	builder.write(JTAGClockScalerOnDLS(), JTAGClockScaler(JTAGClockScaler::Value(3)));
	builder.write(ResetJTAGTapOnDLS(), ResetJTAGTap());

	typed_array<CommonSynramConfig, CommonSynramConfigOnDLS> configs;

	// Wait Omnibus to come up
	builder.wait_until(TimerOnDLS(), Timer::Value(25 * Timer::Value::fpga_clock_cycles_per_us));
	builder.write(TimerOnDLS(), Timer());

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
	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(40000));
	auto program = builder.done();

	auto executor = generate_playback_program_test_executor();
	executor.run(program);

	for (auto coord : iter_all<CommonSynramConfigOnDLS>()) {
		EXPECT_TRUE(tickets[coord.toEnum()].valid());
		EXPECT_EQ(tickets[coord].get(), configs[coord]);
	}
}
