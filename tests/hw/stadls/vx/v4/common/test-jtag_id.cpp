#include "halco/hicann-dls/vx/v4/coordinates.h"
#include "haldls/vx/v4/timer.h"
#include "hxcomm/vx/connection_from_env.h"
#include "stadls/vx/v4/init_generator.h"
#include "stadls/vx/v4/run.h"
#include <gtest/gtest.h>

using namespace stadls::vx::v4;
using namespace haldls::vx::v4;
using namespace halco::hicann_dls::vx::v4;


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

	auto connection = hxcomm::vx::get_connection_from_env();
	run(connection, program);

	ASSERT_TRUE(jtag_id_ticket.valid());
	auto const& jtag_id = dynamic_cast<JTAGIdCode const&>(jtag_id_ticket.get());

	EXPECT_EQ(jtag_id.get_version(), 4);
	EXPECT_EQ(jtag_id.get_part_number(), 0x4859);    // ASCII('HY')
	EXPECT_EQ(jtag_id.get_manufacturer_id(), 0x057); // UHEI JDEC identifier
}
