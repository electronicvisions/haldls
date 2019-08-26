#include <gtest/gtest.h>

#include "haldls/vx/jtag.h"
#include "haldls/vx/playback.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/spi.h"
#include "haldls/vx/timer.h"
#include "stadls/vx/playback_executor.h"

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

	builder.write(ResetChipOnDLS(), ResetChip(true));
	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(10));
	builder.write(ResetChipOnDLS(), ResetChip(false));
	builder.wait_until(TimerOnDLS(), Timer::Value(100));

	// disable LEDs
	builder.write(ShiftRegisterOnBoard(), ShiftRegister());

	// wait 1s
	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(125 * 1000 * 1000));

	// enable LEDs
	ShiftRegister shift_register;
	for (auto led: iter_all<LEDOnBoard>()) {
		shift_register.set_enable_led(led, true);
	}
	builder.write(ShiftRegisterOnBoard(), shift_register);

	// wait 1s
	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(125 * 1000 * 1000));

	// disable LEDs
	builder.write(ShiftRegisterOnBoard(), ShiftRegister());

	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(10000));
	auto program = builder.done();

	auto executor = generate_playback_program_test_executor();
	executor.run(program);
}
