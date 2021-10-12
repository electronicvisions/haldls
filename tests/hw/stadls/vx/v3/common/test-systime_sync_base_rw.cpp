#include <gtest/gtest.h>

#include "halco/hicann-dls/vx/v3/coordinates.h"
#include "haldls/vx/v3/barrier.h"
#include "haldls/vx/v3/jtag.h"
#include "haldls/vx/v3/reset.h"
#include "haldls/vx/v3/systime.h"
#include "haldls/vx/v3/traits.h"
#include "stadls/vx/v3/playback_program.h"
#include "stadls/vx/v3/playback_program_builder.h"
#include "stadls/vx/v3/run.h"

#include "hxcomm/vx/connection_from_env.h"

#include "test-helper.h"

using namespace haldls::vx::v3;
using namespace stadls::vx::v3;
using namespace halco::hicann_dls::vx::v3;

/**
 * This test triggers issue #3460 and has therefore been disabled in simulation.
 */
TEST(SystimeSyncBase, WriteRead_OmnibusOnChipOverJTAG)
{
	PlaybackProgramBuilder builder;

	builder.write(ResetChipOnDLS(), ResetChip(true));
	builder.write(TimerOnDLS(), Timer());
	builder.block_until(TimerOnDLS(), Timer::Value(10));
	builder.write(ResetChipOnDLS(), ResetChip(false));
	builder.block_until(TimerOnDLS(), Timer::Value(100));

	builder.write(JTAGClockScalerOnDLS(), JTAGClockScaler(JTAGClockScaler::Value(3)));
	builder.write(ResetJTAGTapOnDLS(), ResetJTAGTap());


	// wait until Omnibus is up (22 us)
	builder.block_until(TimerOnDLS(), Timer::Value(22 * Timer::Value::fpga_clock_cycles_per_us));

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

		tickets.push_back(builder.read(coord, Backend::OmnibusChipOverJTAG));
	}

	builder.block_until(BarrierOnFPGA(), Barrier::jtag);
	auto program = builder.done();

	for (auto const& ticket : tickets) {
		EXPECT_FALSE(ticket.valid());
	}

	auto connection = hxcomm::vx::get_connection_from_env();
	run(connection, program);

	for (auto const& ticket : tickets) {
		EXPECT_TRUE(ticket.valid());
	}

	for (auto const& ticket : tickets) {
		EXPECT_NO_THROW(ticket.get());
	}

	for (size_t i = 0; i < tickets.size(); ++i) {
		EXPECT_EQ(tickets.at(i).get(), configs.at(i));
	}
}
