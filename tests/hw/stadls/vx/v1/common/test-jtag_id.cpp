#include <gtest/gtest.h>

#include "halco/hicann-dls/vx/v1/coordinates.h"
#include "haldls/vx/v1/timer.h"
#include "stadls/vx/v1/init_generator.h"
#include "stadls/vx/v1/run.h"

#include "connection.h"

using namespace stadls::vx::v1;
using namespace haldls::vx::v1;
using namespace halco::hicann_dls::vx::v1;


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
	builder.block_until(TimerOnDLS(), Timer::Value(1000));
	auto program = builder.done();

	auto connection = generate_test_connection();
	run(connection, program);

	ASSERT_TRUE(jtag_id_ticket.valid());
	auto jtag_id = jtag_id_ticket.get();

	EXPECT_TRUE((jtag_id.get_version() >= 0) && (jtag_id.get_version() <= 3));
	EXPECT_EQ(jtag_id.get_part_number(), 0x4858);    // ASCII('HX')
	EXPECT_EQ(jtag_id.get_manufacturer_id(), 0x057); // UHEI JDEC identifier
}
