#include "halco/hicann-dls/vx/v3/coordinates.h"
#include "haldls/vx/v3/barrier.h"
#include "hxcomm/vx/connection_from_env.h"
#include "stadls/vx/v3/init_generator.h"
#include "stadls/vx/v3/run.h"
#include <gtest/gtest.h>

using namespace stadls::vx::v3;
using namespace haldls::vx::v3;
using namespace halco::hicann_dls::vx::v3;


/**
 * Read back the JTAG ID and assert on its value matches the expectation
 */
TEST(JTAGIdCode, ReadJTAGId)
{
	auto connection = hxcomm::vx::get_connection_from_env();
	auto sequence = DigitalInit(
	    std::visit([](auto const& connection) { return connection.get_hwdb_entry(); }, connection));
	sequence.chip.enable_highspeed_link = false;
	sequence.chip.highspeed_link.enable_systime = false;
	sequence.chip.enable_capmem = false;
	auto [builder, _] = generate(sequence);
	auto jtag_id_ticket = builder.read(JTAGIdCodeOnDLS());
	builder.block_until(BarrierOnFPGA(), Barrier::jtag);
	auto program = builder.done();

	run(connection, program);

	ASSERT_TRUE(jtag_id_ticket.valid());
	auto const& jtag_id = dynamic_cast<JTAGIdCode const&>(jtag_id_ticket.get());

	EXPECT_EQ(jtag_id.get_version(), 3);
	EXPECT_EQ(jtag_id.get_part_number(), 0x4858);    // ASCII('HX')
	EXPECT_EQ(jtag_id.get_manufacturer_id(), 0x057); // UHEI JDEC identifier
}
