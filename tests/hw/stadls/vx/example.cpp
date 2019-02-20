#include <iostream>
#include "fisch/vx/jtag.h"
#include "fisch/vx/playback_executor.h"
#include "fisch/vx/reset.h"
#include "fisch/vx/timer.h"
#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/vx/ppu.h"
#include "hxcomm/vx/simconnection.h"

using namespace halco::hicann_dls::vx;

/**
 * Example for writing and reading a haldls PPU memory word container via omnibus on chip over JTAG.
 */
int main()
{
	fisch::vx::PlaybackProgramBuilder builder;

	fisch::vx::ResetChip reset;
	reset.set(true);
	builder.write<fisch::vx::ResetChip>(halco::hicann_dls::vx::ResetChipOnDLS(), reset);
	builder.write<fisch::vx::Timer>(halco::hicann_dls::vx::TimerOnDLS(), fisch::vx::Timer());
	builder.wait_until(halco::hicann_dls::vx::TimerOnDLS(), fisch::vx::Timer::Value(10));
	reset.set(false);
	builder.write<fisch::vx::ResetChip>(halco::hicann_dls::vx::ResetChipOnDLS(), reset);
	builder.wait_until(halco::hicann_dls::vx::TimerOnDLS(), fisch::vx::Timer::Value(100));

	fisch::vx::JTAGClockScaler jtag_clock_scaler;
	jtag_clock_scaler.set(fisch::vx::JTAGClockScaler::Value(3));
	builder.write<fisch::vx::JTAGClockScaler>(
	    halco::hicann_dls::vx::JTAGOnDLS(), jtag_clock_scaler);
	builder.write<fisch::vx::ResetJTAGTap>(
	    halco::hicann_dls::vx::JTAGOnDLS(), fisch::vx::ResetJTAGTap());

	builder.wait_until(halco::hicann_dls::vx::TimerOnDLS(), fisch::vx::Timer::Value(22 * 125));

	haldls::vx::PPUMemoryWord ppu_word(haldls::vx::PPUMemoryWord::Value(10));
	PPUMemoryWordOnDLS word_coord(PPUMemoryWordOnPPU(3), PPUOnDLS(0));

	halco::hicann_dls::vx::OmnibusAddress addr(ppu_word.addresses(word_coord)[0]);
	fisch::vx::OmnibusOnChipOverJTAG config(fisch::vx::OmnibusData(ppu_word.encode()[0]));

	auto ticket1 = builder.read<fisch::vx::OmnibusOnChipOverJTAG>(addr);
	builder.write(addr, config);
	auto ticket2 = builder.read<fisch::vx::OmnibusOnChipOverJTAG>(addr);

	builder.wait_until(halco::hicann_dls::vx::TimerOnDLS(), fisch::vx::Timer::Value(10000));
	builder.halt();
	auto program = builder.done();

	std::cout << program << std::endl;

	fisch::vx::PlaybackProgramExecutor<hxcomm::vx::SimConnection> executor("127.0.0.1", 50004);
	executor.transfer(program);
	executor.execute();
	executor.fetch(program);

	auto result1 = ticket1.get();
	auto result2 = ticket2.get();
	std::cout << result1.get() << std::endl;
	std::cout << result2.get() << std::endl;
	return 0;
}
