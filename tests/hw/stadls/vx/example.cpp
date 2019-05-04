#include <iostream>
#include "fisch/vx/constants.h"
#include "fisch/vx/playback_executor.h"
#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/playback.h"
#include "haldls/vx/ppu.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/timer.h"
#include "hxcomm/vx/simconnection.h"

using namespace halco::hicann_dls::vx;

/**
 * Example for writing and reading a haldls PPU memory word container via omnibus on chip over JTAG.
 */
int main()
{
	haldls::vx::PlaybackProgramBuilder builder;

	haldls::vx::ResetChip reset;
	reset.set(true);
	builder.write<haldls::vx::ResetChip>(halco::hicann_dls::vx::ResetChipOnDLS(), reset);
	builder.write<haldls::vx::Timer>(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer());
	builder.wait_until(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer::Value(10));
	reset.set(false);
	builder.write<haldls::vx::ResetChip>(halco::hicann_dls::vx::ResetChipOnDLS(), reset);
	builder.wait_until(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer::Value(100));

	haldls::vx::JTAGClockScaler jtag_clock_scaler;
	jtag_clock_scaler.set(haldls::vx::JTAGClockScaler::Value(3));
	builder.write<haldls::vx::JTAGClockScaler>(
	    halco::hicann_dls::vx::JTAGClockScalerOnDLS(), jtag_clock_scaler);
	builder.write<haldls::vx::ResetJTAGTap>(
	    halco::hicann_dls::vx::ResetJTAGTapOnDLS(), haldls::vx::ResetJTAGTap());

	auto ticket_jtag_id = builder.read<haldls::vx::JTAGIdCode>(halco::hicann_dls::vx::JTAGIdCodeOnDLS());

	// wait until ASIC omnibus is up (22 us)
	builder.wait_until(
	    halco::hicann_dls::vx::TimerOnDLS(),
	    haldls::vx::Timer::Value(22 * fisch::vx::fpga_clock_cycles_per_us));

	haldls::vx::PPUMemoryWord ppu_word(haldls::vx::PPUMemoryWord::Value(10));
	PPUMemoryWordOnDLS word_coord(PPUMemoryWordOnPPU(3), PPUOnDLS(0));

	auto ticket1 = builder.read<haldls::vx::PPUMemoryWord>(word_coord);
	builder.write(word_coord, ppu_word);
	auto ticket2 = builder.read<haldls::vx::PPUMemoryWord>(word_coord);

	builder.wait_until(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer::Value(10000));
	builder.halt();
	auto program = builder.done();

	std::cout << program << std::endl;

	fisch::vx::PlaybackProgramExecutor<hxcomm::vx::SimConnection> executor("127.0.0.1", 50004);
	executor.transfer(program.impl());
	executor.execute();
	executor.fetch(program.impl());

	auto result_jtag_id = ticket_jtag_id.get();
	auto result1 = ticket1.get();
	auto result2 = ticket2.get();
	std::cout << "JTAGIdCode_ticket.valid(): " << ticket_jtag_id.valid() << std::endl;
	std::cout << "JTAGIdCode: " << result_jtag_id.get() << std::endl;
	std::cout << "Read before write PPUMemroyWord: " << result1.get() << std::endl;
	std::cout << "Read after write PPUMemoryWord: " << result2.get() << std::endl;
	return 0;
}
