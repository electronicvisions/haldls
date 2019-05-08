#include <gtest/gtest.h>

#include "fisch/vx/playback_executor.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/playback.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/spi.h"
#include "haldls/vx/timer.h"
#include "hxcomm/vx/arqconnection.h"

#include "executor.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx;

/**
 * This is a local-board-only test relying on visual verification, that all six addressable LEDs
 * on the xBoard are first disabled and then enabled on execution of this test.
 */
TEST(ShiftRegister, ToggleLEDs)
{
	PlaybackProgramBuilder builder;

	builder.write<ResetChip>(ResetChipOnDLS(), ResetChip(true));
	builder.write<Timer>(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(10));
	builder.write<ResetChip>(ResetChipOnDLS(), ResetChip(false));
	builder.wait_until(TimerOnDLS(), Timer::Value(100));

	// disable LEDs
	builder.write(ShiftRegisterOnBoard(), ShiftRegister());

	// wait 1s
	builder.write<Timer>(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(125 * 1000 * 1000));

	// enable LEDs
	ShiftRegister shift_register;
	for (auto led: iter_all<LEDOnBoard>()) {
		shift_register.set_enable_led(led, true);
	}
	builder.write(ShiftRegisterOnBoard(), shift_register);

	// wait 1s
	builder.write<Timer>(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(125 * 1000 * 1000));

	// disable LEDs
	builder.write(ShiftRegisterOnBoard(), ShiftRegister());

	builder.write<Timer>(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(10000));
	builder.halt();
	auto program = builder.done();

	auto executor = generate_playback_program_test_executor();
	executor.run(program.impl());
}
