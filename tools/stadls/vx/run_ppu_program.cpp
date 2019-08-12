#include <fstream>
#include <boost/program_options.hpp>

#include "fisch/vx/constants.h"
#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/playback.h"
#include "haldls/vx/ppu.h"
#include "haldls/vx/timer.h"
#include "logging_ctrl.h"
#include "stadls/vx/playback_executor.h"

#include "helpers.hpp"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx;
namespace po = boost::program_options;

PPUMemoryWordOnPPU const stack_begin(3071);
PPUMemoryWordOnPPU const mailbox_begin(3072);
PPUMemoryWordOnPPU const mailbox_end(4095);
PPUMemoryWordOnPPU const return_code(3071);

PPUMemoryBlockOnPPU const mailbox_coord_on_ppu(mailbox_begin, mailbox_end);

using ip_t = stadls::vx::PlaybackProgramExecutor::ip_t;
using port_t = stadls::vx::PlaybackProgramExecutor::port_t;

int main(int argc, char* argv[])
{
	po::options_description opt_desc("Options");
	int wait;
	std::string test_board;
	std::string program_filename;
	std::string loglevel;
	int ppu_id;
	bool use_jtag;
	ip_t fpga_ip;
	ip_t sim_ip;
	port_t sim_port;
	opt_desc.add_options()("help", "Print this help message and exit.")(
	    "ppu", po::value<int>(&ppu_id)->required(), "Number of PPU to use (0 or 1).")(
	    "fpga_ip", po::value<ip_t>(&fpga_ip)->default_value("192.168.4.4"))(
	    "use_jtag", po::value<bool>(&use_jtag)->default_value(false),
	    "Use JTAG link instead of highspeed link.")(
	    "sim_ip", po::value<ip_t>(&sim_ip)->default_value("0.0.0.0"), "IP of simulation server.")(
	    "sim_port", po::value<port_t>(&sim_port)->default_value(0), "Port of simulation server.")(
	    "program", po::value<std::string>(&program_filename)->required(),
	    "Filename of (stripped) PPU program (suffix .binary).")(
	    "wait", po::value<int>(&wait)->default_value(10000000),
	    "Wait time in FPGA cycles, until memory readout and exit of program.")(
	    "print-mailbox", "Print mailbox memory region as string.")(
	    "dump-stack-heap", "Dump stack/heap memory region.")(
	    "dump-memory", "Dump memory after execution.")(
	    "loglevel", po::value<std::string>(&loglevel)->default_value("info"),
	    "Set loglevel. Valid arguments are 'debug', 'info', 'warning', 'error' or 'fatal'");
	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(opt_desc).run(), vm);
	if (vm.count("help")) {
		std::cout << "Run PPU program. Uses hardware by default and uses simulation if options "
		             "sim_ip, sim_port are provided."
		          << std::endl
		          << std::endl
		          << opt_desc << std::endl;
		exit(EXIT_SUCCESS);
	}
	po::notify(vm);

	if ((ppu_id != 1) and (ppu_id != 0)) {
		throw po::validation_error(
		    po::validation_error::invalid_option_value, "Choose between PPU 0 and PPU 1.");
	}
	PPUOnDLS ppu_coord(ppu_id);
	auto backend = Backend::OmnibusChip;
	if (use_jtag) {
		backend = Backend::OmnibusChipOverJTAG;
	}

	if (loglevel == "debug")
		logger_default_config(log4cxx::Level::getDebug());
	else if (loglevel == "info")
		logger_default_config(log4cxx::Level::getInfo());
	else if (loglevel == "warning")
		logger_default_config(log4cxx::Level::getWarn());
	else if (loglevel == "error")
		logger_default_config(log4cxx::Level::getError());
	else if (loglevel == "fatal")
		logger_default_config(log4cxx::Level::getFatal());
	else {
		std::cout
		    << "Please specify a valid loglevel 'debug', 'info', 'warning', 'error' or 'fatal'."
		    << std::endl;
		exit(EXIT_FAILURE);
	}
	auto logger = log4cxx::Logger::getRootLogger();

	// build program
	PlaybackProgramBuilder builder;

	// Chip reset
	builder.write<ResetChip>(ResetChipOnDLS(), ResetChip(true));
	builder.write<Timer>(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(10));
	builder.write<ResetChip>(ResetChipOnDLS(), ResetChip(false));
	builder.wait_until(TimerOnDLS(), Timer::Value(100));

	// JTAG init
	builder.write(JTAGClockScalerOnDLS(), JTAGClockScaler(JTAGClockScaler::Value(3)));
	builder.write(ResetJTAGTapOnDLS(), ResetJTAGTap());

	// PLL init, reconfiguration needed to slow down PPU to a working state
	ADPLL adpll_config;
	for (auto adpll : halco::common::iter_all<halco::hicann_dls::vx::ADPLLOnDLS>()) {
		builder.write(adpll, adpll_config, Backend::JTAGPLLRegister);
	}

	PLLClockOutputBlock config;
	builder.write(
	    halco::hicann_dls::vx::PLLClockOutputBlockOnDLS(), config, Backend::JTAGPLLRegister);

	// Wait for PLL and Omnibus to come up
	builder.wait_until(TimerOnDLS(), Timer::Value(100 * fisch::vx::fpga_clock_cycles_per_us));
	builder.write<haldls::vx::Timer>(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer());
	if (not use_jtag) {
		// Configure all FPGA-side Phys
		for (auto phy : iter_all<PhyConfigFPGAOnDLS>()) {
			builder.write(phy, PhyConfigFPGA());
		}

		// Enable all FPGA-side Phys
		builder.write(CommonPhyConfigFPGAOnDLS(), CommonPhyConfigFPGA());

		// Configure all Chip-side Phys
		for (auto phy : iter_all<PhyConfigChipOnDLS>()) {
			builder.write(phy, PhyConfigChip());
		}

		// Enable all Chip-side Phys
		builder.write(CommonPhyConfigChipOnDLS(), CommonPhyConfigChip());

		// wait until highspeed is up (omnibus clock lock + phy config write over JTAG)
		builder.write<Timer>(TimerOnDLS(), Timer());
		builder.wait_until(TimerOnDLS(), Timer::Value(80 * fisch::vx::fpga_clock_cycles_per_us));
	}

	auto ppu_memory_program = helpers::load_PPUMemoryBlock_from_file(program_filename);
	LOG4CXX_INFO(logger, "PPU program size " << ppu_memory_program.size());
	PPUMemoryBlockOnDLS ppu_memory_program_coord(
	    PPUMemoryBlockOnPPU(
	        PPUMemoryWordOnPPU(0), PPUMemoryWordOnPPU(ppu_memory_program.size() - 1)),
	    ppu_coord);
	PPUMemoryBlockOnDLS ppu_memory_stack_heap_coord(
	    PPUMemoryBlockOnPPU(PPUMemoryWordOnPPU(ppu_memory_program.size()), stack_begin), ppu_coord);
	PPUMemoryBlockOnDLS ppu_mailbox_coord(mailbox_coord_on_ppu, ppu_coord);
	PPUMemoryWordOnDLS ppu_return_code_coord(return_code, ppu_coord);
	PPUMemoryOnDLS ppu_memory_coord(PPUMemoryOnPPU(), ppu_coord);

	PPUControlRegister ppu_control_register;
	PPUControlRegisterOnDLS ppu_control_register_coord(PPUControlRegisterOnPPU(), ppu_coord);
	PPUStatusRegisterOnDLS ppu_status_register_coord(PPUStatusRegisterOnPPU(), ppu_coord);
	ppu_control_register.set_inhibit_reset(false);
	LOG4CXX_INFO(logger, "Emitting write for control register.")
	builder.write(ppu_control_register_coord, ppu_control_register, backend);

	LOG4CXX_INFO(logger, "Emitting write for program.")
	// First reset memory by writing all zeros
	PPUMemory::words_type zero_words;
	zero_words.fill(PPUMemoryWord(PPUMemoryWord::Value(0)));
	PPUMemory zero_memory;
	zero_memory.set_words(zero_words);
	builder.write(ppu_memory_coord, zero_memory, backend);
	// Write PPU memory words with Omnibus over JTAG backend
	builder.write(ppu_memory_program_coord, ppu_memory_program, backend);

	ppu_control_register.set_inhibit_reset(true);
	LOG4CXX_INFO(logger, "Emitting write for control register.")
	builder.write(ppu_control_register_coord, ppu_control_register, backend);

	builder.write<Timer>(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(wait));

	LOG4CXX_INFO(logger, "Emitting read commands.");
	auto ppu_memory_ticket = builder.read(ppu_memory_coord, backend);
	// Read PPU memory words with Omnibus over JTAG backend
	auto ppu_memory_return_ticket = builder.read(ppu_return_code_coord, backend);
	auto ppu_status_ticket = builder.read(ppu_status_register_coord, backend);

	// run ppu program
	LOG4CXX_INFO(logger, "Creating program.");
	auto program = builder.done();
	auto executor = stadls::vx::PlaybackProgramExecutor();
	if ((sim_ip == "0.0.0.0") and (sim_port == 0)) {
		LOG4CXX_INFO(logger, "Running built program on hardware.");
		executor.connect_hardware(fpga_ip);
		executor.run(program);
		executor.disconnect();
	} else {
		LOG4CXX_INFO(logger, "Running built program on simulation.");
		executor.connect_simulator(sim_ip, sim_port);
		executor.run(program);
		executor.disconnect();
	}

	// get results

	auto status_register = ppu_status_ticket.get();
	if (status_register.get_sleep()) {
		LOG4CXX_INFO(logger, "PPU program finished.");
	} else {
		LOG4CXX_WARN(logger, "PPU program didn't finish.");
	}

	auto ppu_memory_after = ppu_memory_ticket.get();
	auto ppu_memory_program_after = ppu_memory_after.get_block(ppu_memory_program_coord);
	if (ppu_memory_program == ppu_memory_program_after) {
		LOG4CXX_INFO(logger, "PPU program didn't change.");
	} else {
		LOG4CXX_WARN(logger, "PPU program changed.");
	}

	uint32_t exit_code = ppu_memory_return_ticket.get().get_value();
	if (exit_code == PPUMemoryWord::Value(0)) {
		LOG4CXX_INFO(logger, "PPU program exited with exit code 0.");
	} else {
		LOG4CXX_ERROR(logger, "PPU program exited with exit code " << exit_code << ".");
	}

	if (vm.count("dump-stack-heap")) {
		auto ppu_memory_stack_heap_after = ppu_memory_after.get_block(ppu_memory_stack_heap_coord);
		std::stringstream stack_heap;
		stack_heap << "Dumping stack/heap." << std::endl;
		stack_heap << ppu_memory_stack_heap_after << std::endl;
		stack_heap << "Stack/heap dumped.";
		LOG4CXX_INFO(logger, stack_heap.str());
	}

	if (vm.count("dump-memory")) {
		std::stringstream memory;
		memory << "Dumping memory." << std::endl;
		memory << ppu_memory_after << std::endl;
		memory << "Memory dumped.";
		LOG4CXX_INFO(logger, memory.str());
	}

	if (vm.count("print-mailbox")) {
		auto ppu_memory_mailbox_after = ppu_memory_after.get_block(ppu_mailbox_coord);
		std::stringstream mailbox;
		mailbox << "Printing mailbox." << std::endl;
		mailbox << ppu_memory_mailbox_after.to_string() << std::endl;
		mailbox << "Mailbox printed.";
		LOG4CXX_INFO(logger, mailbox.str());
	}
	return exit_code;
}
