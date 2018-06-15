#include <fstream>

#include <boost/program_options.hpp>

#include "halco/hicann-dls/v2/coordinates.h"
#include "haldls/v2/board.h"
#include "haldls/v2/chip.h"
#include "haldls/v2/playback.h"
#include "logging_ctrl.h"
#include "stadls/v2/experiment.h"

#include "helpers.hpp"

using namespace halco::common;
using namespace halco::hicann_dls::v2;
using namespace haldls::v2;
using namespace stadls::v2;
namespace po = boost::program_options;

PPUMemoryWordOnDLS const stack_begin(3071);
PPUMemoryWordOnDLS const mailbox_begin(3072);
PPUMemoryWordOnDLS const mailbox_end(4095);
PPUMemoryWordOnDLS const return_code(3071);

PPUMemoryBlockOnDLS const mailbox_coord(mailbox_begin, mailbox_end);

int main(int argc, char* argv[])
{
	po::options_description opt_desc("Options");
	int wait;
	std::string test_board;
	std::string program_filename;
	std::string loglevel;
	// clang-format off
	opt_desc.add_options()
		("help", "Print this help message and exit.")
		("board-id", po::value<std::string>(&test_board), "FPGA board ID. If not supplied, environment variables are used.")
		("program", po::value<std::string>(&program_filename)->required(),
		"Filename of (stripped) PPU program (suffix .binary).")
		("wait", po::value<int>(&wait)->default_value(10000000),
		"Wait time in FPGA cycles, until memory readout and exit of program.")
		("print-mailbox", "Print mailbox memory region as string.")
		("dump-stack-heap", "Dump stack/heap memory region.")
		("dump-memory", "Dump memory after execution.")
		("loglevel", po::value<std::string>(&loglevel)->default_value("info"),
		"Set loglevel. Valid arguments are 'debug', 'info', 'warning', 'error' or 'fatal'");
	// clang-format on
	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(opt_desc).run(), vm);
	if (vm.count("help")) {
		std::cout << "Run PPU program." << std::endl << std::endl << opt_desc << std::endl;
		exit(EXIT_SUCCESS);
	}
	po::notify(vm);

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

	auto ppu_memory_program = helpers::load_PPUMemoryBlock_from_file(program_filename);
	auto ppu_memory_program_coord = PPUMemoryBlockOnDLS(PPUMemoryWordOnDLS(0), PPUMemoryWordOnDLS(ppu_memory_program.size() - 1));
	auto ppu_memory_stack_heap_coord =
	    PPUMemoryBlockOnDLS(PPUMemoryWordOnDLS(ppu_memory_program.size()), stack_begin);

	PPUControlRegister ppu_control_register;
	ppu_control_register.set_inhibit_reset(false);
	builder.write(PPUControlRegisterOnDLS(), ppu_control_register);

	builder.write(ppu_memory_program_coord, ppu_memory_program);

	ppu_control_register.set_inhibit_reset(true);
	builder.write(PPUControlRegisterOnDLS(), ppu_control_register);

	builder.wait_until(wait);

	auto ppu_memory_program_ticket = builder.read<PPUMemoryBlock>(ppu_memory_program_coord);
	auto ppu_memory_mailbox_ticket = builder.read<PPUMemoryBlock>(mailbox_coord);
	auto ppu_memory_stack_heap_ticket =
	    builder.read<PPUMemoryBlock>(ppu_memory_stack_heap_coord);
	auto ppu_memory_ticket = builder.read<PPUMemory>(PPUMemoryOnDLS());
	auto ppu_memory_return_ticket = builder.read<PPUMemoryWord>(return_code);
	auto ppu_status_ticket = builder.read<PPUStatusRegister>(PPUStatusRegisterOnDLS());
	builder.halt();

	// run ppu program
	auto program = builder.done();
	LOG4CXX_INFO(logger, "Running built program.");
	{
		ExperimentControl ctrl = vm.count("board-id") ? ExperimentControl(test_board) : ExperimentControl();
		ctrl.run_experiment(Board(), Chip(), program);
	}

	// get results

	auto status_register = ppu_status_ticket.get();
	if (status_register.get_sleep()) {
		LOG4CXX_INFO(logger, "PPU program finished.");
	} else {
		LOG4CXX_WARN(logger, "PPU program didn't finish.");
	}

	auto ppu_memory_program_after = ppu_memory_program_ticket.get();
	if (ppu_memory_program == ppu_memory_program_after) {
		LOG4CXX_INFO(logger, "PPU program didn't change.");
	} else {
		LOG4CXX_WARN(logger, "PPU program changed.");
	}

	uint32_t exit_code = ppu_memory_return_ticket.get().get();
	if (exit_code == PPUMemoryWord::Value(0)) {
		LOG4CXX_INFO(logger, "PPU program exited with exit code 0.");
	} else {
		LOG4CXX_ERROR(logger, "PPU program exited with exit code " << exit_code << ".");
	}

	if (vm.count("dump-stack-heap")) {
		auto ppu_memory_stack_heap_after = ppu_memory_stack_heap_ticket.get();
		std::stringstream stack_heap;
		stack_heap << "Dumping stack/heap." << std::endl;
		stack_heap << ppu_memory_stack_heap_after << std::endl;
		stack_heap << "Stack/heap dumped.";
		LOG4CXX_INFO(logger, stack_heap.str());
	}

	if (vm.count("dump-memory")) {
		auto ppu_memory_after = ppu_memory_ticket.get();
		std::stringstream memory;
		memory << "Dumping memory." << std::endl;
		memory << ppu_memory_after << std::endl;
		memory << "Memory dumped.";
		LOG4CXX_INFO(logger, memory.str());
	}

	if (vm.count("print-mailbox")) {
		auto ppu_memory_mailbox_after = ppu_memory_mailbox_ticket.get();
		std::stringstream mailbox;
		mailbox << "Printing mailbox." << std::endl;
		mailbox << ppu_memory_mailbox_after.to_string() << std::endl;
		mailbox << "Mailbox printed.";
		LOG4CXX_INFO(logger, mailbox.str());
	}
	return exit_code;
}
