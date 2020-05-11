#include <gtest/gtest.h>

#include "halco/hicann-dls/vx/jtag.h"
#include "haldls/vx/timer.h"
#include "stadls/vx/init_generator.h"

#include "executor.h"

using namespace stadls::vx;
using namespace haldls::vx;
using namespace halco::hicann_dls::vx;


/**
 * Read back the JTAG ID and assert on its value matches the expectation
 */
TEST(JTAGIdCode, ReadJTAGId)
{
	auto sequence = DigitalInit();
	sequence.highspeed_link.enable_systime = false;
	auto [builder, _] = generate(sequence);
	auto jtag_id_ticket = builder.read(JTAGIdCodeOnDLS());

	// Wait for read
	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(1000));
	auto program = builder.done();

	auto executor = generate_playback_program_test_executor();
	executor.run(program);

	ASSERT_TRUE(jtag_id_ticket.valid());
	auto jtag_id = jtag_id_ticket.get();

	EXPECT_TRUE((jtag_id.get_version() >= 0) && (jtag_id.get_version() <= 2));
	EXPECT_EQ(jtag_id.get_part_number(), 0x4858);    // ASCII('HX')
	EXPECT_EQ(jtag_id.get_manufacturer_id(), 0x057); // UHEI JDEC identifier
}
