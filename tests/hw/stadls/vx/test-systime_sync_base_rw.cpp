#include <gtest/gtest.h>

#include "haldls/vx/systime.h"

#include "fisch/vx/constants.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/playback.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/timer.h"

#include "executor.h"

#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;

TEST(SystimeSyncBase, WriteRead_OmnibusOnChipOverJTAG)
{
	PlaybackProgramBuilder builder;

	builder.write<ResetChip>(ResetChipOnDLS(), ResetChip(true));
	builder.write<Timer>(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(10));
	builder.write<ResetChip>(ResetChipOnDLS(), ResetChip(false));
	builder.wait_until(TimerOnDLS(), Timer::Value(100));

	builder.write<JTAGClockScaler>(
	    JTAGClockScalerOnDLS(), JTAGClockScaler(JTAGClockScaler::Value(3)));
	builder.write<ResetJTAGTap>(ResetJTAGTapOnDLS(), ResetJTAGTap());


	// wait until Omnibus is up (22 us)
	builder.wait_until(TimerOnDLS(), Timer::Value(22 * fisch::vx::fpga_clock_cycles_per_us));

	// number of written and read values
	constexpr size_t num = 100;

	std::vector<PlaybackProgram::ContainerTicket<SystimeSyncBase>> tickets;
	std::vector<SystimeSyncBase> configs;

	SystimeSyncBaseOnDLS coord;
	for (size_t i = 0; i < num; ++i) {
		SystimeSyncBase config;
		// random data
		config.set_value(
		    draw_ranged_non_default_value<SystimeSyncBase::Value>(SystimeSyncBase::Value()));

		builder.write(coord, config, Backend::OmnibusChipOverJTAG);
		configs.push_back(config);

		tickets.push_back(builder.read<SystimeSyncBase>(coord, Backend::OmnibusChipOverJTAG));
	}

	builder.wait_until(TimerOnDLS(), Timer::Value(10000));
	auto program = builder.done();

	for (auto const ticket : tickets) {
		EXPECT_FALSE(ticket.valid());
	}

	auto executor = generate_playback_program_test_executor();
	executor.run(program);

	for (auto const ticket : tickets) {
		EXPECT_TRUE(ticket.valid());
	}

	for (auto const ticket : tickets) {
		EXPECT_NO_THROW(ticket.get());
	}

	for (size_t i = 0; i < tickets.size(); ++i) {
		EXPECT_EQ(tickets.at(i).get(), configs.at(i));
	}
}
