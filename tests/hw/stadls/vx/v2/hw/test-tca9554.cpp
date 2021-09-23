#include <gtest/gtest.h>

#include "halco/common/iter_all.h"
#include "halco/hicann-dls/vx/v2/coordinates.h"
#include "halco/hicann-dls/vx/v2/ultra96.h"
#include "haldls/vx/i2c.h"
#include "haldls/vx/v2/barrier.h"
#include "haldls/vx/v2/i2c.h"
#include "haldls/vx/v2/jtag.h"
#include "haldls/vx/v2/reset.h"
#include "haldls/vx/v2/timer.h"
#include "hxcomm/vx/connection_from_env.h"
#include "stadls/vx/v2/playback_program.h"
#include "stadls/vx/v2/playback_program_builder.h"
#include "stadls/vx/v2/run.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx::v2;
using namespace haldls::vx::v2;
using namespace stadls::vx::v2;

/**
 * This is a local-board-only test relying on visual verification, that all eight addressable
 * channels on the TCA9554 are first disabled and then enabled on execution of this test.
 *
 * Since the hardware is not available on all setups, this test is disabled.
 */
TEST(TCA9554Config, DISABLED_ToggleChannels)
{
	PlaybackProgramBuilder builder;

	// This has been copied from test-shift_register.cpp
	// TODO: Evaluate necessity of chip reset and waits
	builder.write(ResetChipOnDLS(), ResetChip(true));
	builder.write(TimerOnDLS(), Timer());
	builder.block_until(TimerOnDLS(), Timer::Value(10));
	builder.write(ResetChipOnDLS(), ResetChip(false));
	builder.block_until(TimerOnDLS(), Timer::Value(100));

	auto config_off = TCA9554Config();
	auto config_on = TCA9554Config();
	auto channel_output = config_on.get_channel_output();
	for (auto channel : iter_all<TCA9554ChannelOnBoard>()) {
		channel_output[channel] = true;
	}
	config_on.set_channel_output(channel_output);


	for (int i = 0; i < 10; i++) {
		// disable LEDs
		builder.write(TCA9554ConfigOnBoard(), config_off);

		// wait 200ms
		builder.write(TimerOnDLS(), Timer());
		builder.block_until(TimerOnDLS(), Timer::Value(125 * 1000 * 200));

		// enable LEDs
		builder.write(TCA9554ConfigOnBoard(), config_on);

		// wait 200ms
		builder.write(TimerOnDLS(), Timer());
		builder.block_until(TimerOnDLS(), Timer::Value(125 * 1000 * 200));
	}

	builder.write(TCA9554ConfigOnBoard(), config_off);

	builder.block_until(BarrierOnFPGA(), Barrier::omnibus);
	auto program = builder.done();

	auto connection = hxcomm::vx::get_connection_from_env();
	run(connection, program);
}


// Since the hardware is not available on all setups, this test is disabled.
TEST(TCA9554Inputs, DISABLED_ReadChannels)
{
	PlaybackProgramBuilder builder;

	// This has been copied from test-shift_register.cpp
	// TODO: Evaluate necessity of chip reset and waits
	builder.write(ResetChipOnDLS(), ResetChip(true));
	builder.write(TimerOnDLS(), Timer());
	builder.block_until(TimerOnDLS(), Timer::Value(10));
	builder.write(ResetChipOnDLS(), ResetChip(false));
	builder.block_until(TimerOnDLS(), Timer::Value(100));

	// test read of inputs
	PlaybackProgram::ContainerTicket<TCA9554Inputs> ticket = builder.read(TCA9554InputsOnBoard());

	builder.block_until(BarrierOnFPGA(), Barrier::omnibus);
	auto program = builder.done();

	auto connection = hxcomm::vx::get_connection_from_env();
	run(connection, program);

	auto channel_inputs = ticket.get().get_channel_input();
	for (auto channel : iter_all<TCA9554ChannelOnBoard>()) {
		EXPECT_EQ(channel_inputs[channel], 0);
	}
}