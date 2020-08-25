#include "halco/hicann-dls/vx/v2/coordinates.h"
#include "haldls/vx/v2/barrier.h"
#include "haldls/vx/v2/jtag.h"
#include "haldls/vx/v2/reset.h"
#include "haldls/vx/v2/spi.h"
#include "haldls/vx/v2/timer.h"
#include "hxcomm/vx/connection_from_env.h"
#include "stadls/vx/v2/init_generator.h"
#include "stadls/vx/v2/playback_generator.h"
#include "stadls/vx/v2/playback_program.h"
#include "stadls/vx/v2/playback_program_builder.h"
#include "stadls/vx/v2/run.h"
#include <gtest/gtest.h>

using namespace halco::common;
using namespace halco::hicann_dls::vx::v2;
using namespace haldls::vx::v2;
using namespace stadls::vx::v2;

/**
 * This is a local-board-only test relying on visual verification, that all six addressable LEDs
 * on the xBoard are first disabled and then enabled on execution of this test.
 */
TEST(ShiftRegister, ToggleLEDs)
{
	auto sequence = DigitalInit();
	auto [builder, _] = generate(sequence);

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
