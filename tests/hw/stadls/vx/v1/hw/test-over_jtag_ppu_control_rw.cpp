#include <gtest/gtest.h>

#include "halco/hicann-dls/vx/v1/coordinates.h"
#include "haldls/vx/v1/barrier.h"
#include "haldls/vx/v1/jtag.h"
#include "haldls/vx/v1/ppu.h"
#include "haldls/vx/v1/reset.h"
#include "haldls/vx/v1/traits.h"
#include "stadls/vx/v1/init_generator.h"
#include "stadls/vx/v1/playback_program.h"
#include "stadls/vx/v1/playback_program_builder.h"
#include "stadls/vx/v1/run.h"

#include "hxcomm/vx/connection_from_env.h"
#include "test-helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx::v1;
using namespace haldls::vx::v1;
using namespace stadls::vx::v1;

extern std::optional<size_t> const max_words_per_reduced_test;

/**
 * Write and read control register
 *
 * This test triggers issue #3459 and has therefore been disabled in simulation.
 */
TEST(PPUControlRegister, WROverJTAG)
{
	auto sequence = DigitalInit();
	sequence.enable_highspeed_link = false;
	auto [builder, _] = generate(sequence);

	PPUControlRegister reg1, reg2;
	PPUControlRegisterOnDLS ppu_coord1(0);
	PPUControlRegisterOnDLS ppu_coord2(1);
	reg1.set_cache_controller_enable(true);
	reg1.set_inhibit_reset(true);
	reg1.set_force_clock_on(true);
	reg1.set_force_clock_off(false);

	reg2.set_cache_controller_enable(false);
	reg2.set_inhibit_reset(false);
	reg2.set_force_clock_on(false);
	reg2.set_force_clock_off(true);

	// Write and read back reg1
	builder.write(ppu_coord1, reg1, Backend::OmnibusChipOverJTAG);
	builder.write(ppu_coord2, reg1, Backend::OmnibusChipOverJTAG);
	std::vector<PlaybackProgram::ContainerTicket<PPUControlRegister>> reg1_tickets;
	reg1_tickets.push_back(builder.read(ppu_coord1, Backend::OmnibusChipOverJTAG));
	reg1_tickets.push_back(builder.read(ppu_coord2, Backend::OmnibusChipOverJTAG));


	// Write and read back reg2
	builder.write(ppu_coord1, reg2, Backend::OmnibusChipOverJTAG);
	builder.write(ppu_coord2, reg2, Backend::OmnibusChipOverJTAG);
	std::vector<PlaybackProgram::ContainerTicket<PPUControlRegister>> reg2_tickets;
	reg2_tickets.push_back(builder.read(ppu_coord1, Backend::OmnibusChipOverJTAG));
	reg2_tickets.push_back(builder.read(ppu_coord2, Backend::OmnibusChipOverJTAG));

	builder.block_until(BarrierOnFPGA(), Barrier::jtag);
	auto program = builder.done();

	auto connection = hxcomm::vx::get_connection_from_env();
	run(connection, program);

	EXPECT_TRUE(reg1_tickets[0].valid());
	EXPECT_TRUE(reg1_tickets[1].valid());
	EXPECT_TRUE(reg1_tickets[0].get() == reg1);
	EXPECT_TRUE(reg1_tickets[1].get() == reg1);

	EXPECT_TRUE(reg2_tickets[0].valid());
	EXPECT_TRUE(reg2_tickets[1].valid());
	EXPECT_TRUE(reg2_tickets[0].get() == reg2);
	EXPECT_TRUE(reg2_tickets[1].get() == reg2);
}
