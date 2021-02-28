#include "halco/hicann-dls/vx/v1/coordinates.h"
#include "haldls/vx/v1/barrier.h"
#include "haldls/vx/v1/jtag.h"
#include "haldls/vx/v1/reset.h"
#include "haldls/vx/v1/spi.h"
#include "haldls/vx/v1/timer.h"
#include "hxcomm/vx/connection_from_env.h"
#include "stadls/vx/v1/playback_program.h"
#include "stadls/vx/v1/playback_program_builder.h"
#include "stadls/vx/v1/run.h"
#include <gtest/gtest.h>

using namespace halco::common;
using namespace halco::hicann_dls::vx::v1;
using namespace haldls::vx::v1;
using namespace stadls::vx::v1;

/**
 * This is a local-board-only test relying on visual verification, that all six addressable LEDs
 * on the xBoard are first disabled and then enabled on execution of this test.
 */
TEST(ShiftRegister, ToggleLEDs)
{
	PlaybackProgramBuilder builder;

	builder.write(ResetChipOnDLS(), ResetChip(true));
	builder.write(TimerOnDLS(), Timer());
	builder.block_until(TimerOnDLS(), Timer::Value(10));
	builder.write(ResetChipOnDLS(), ResetChip(false));
	builder.block_until(TimerOnDLS(), Timer::Value(100));

	// disable LEDs
	builder.write(ShiftRegisterOnBoard(), ShiftRegister());

	// wait 1s
	builder.write(TimerOnDLS(), Timer());
	builder.block_until(TimerOnDLS(), Timer::Value(125 * 1000 * 1000));

	// enable LEDs
	ShiftRegister shift_register;
	for (auto led : iter_all<LEDOnBoard>()) {
		shift_register.set_enable_led(led, true);
	}
	builder.write(ShiftRegisterOnBoard(), shift_register);

	// wait 1s
	builder.write(TimerOnDLS(), Timer());
	builder.block_until(TimerOnDLS(), Timer::Value(125 * 1000 * 1000));

	// disable LEDs
	builder.write(ShiftRegisterOnBoard(), ShiftRegister());

	builder.block_until(BarrierOnFPGA(), Barrier::omnibus);
	auto program = builder.done();

	auto connection = hxcomm::vx::get_connection_from_env();
	run(connection, program);
}
