#include <cstdlib>
#include <netinet/in.h>

#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <thread>
#include <utility>
#include <vector>

#include <gelf.h>
#include <libelf.h>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <gtest/gtest.h>
#include <tbb/concurrent_queue.h>

#include "halco/hicann-dls/v2/coordinates.h"
#include "haldls/v2/board.h"
#include "haldls/v2/chip.h"
#include "haldls/v2/playback.h"
#include "logging_ctrl.h"
#include "stadls/v2/local_board_control.h"

#include "read_symbols.hpp"

#include "ppu_gdbserver/handle_request.hpp"
#include "ppu_gdbserver/handle_response.hpp"
#include "ppu_gdbserver/tcp_server.hpp"

using boost::asio::ip::tcp;
using namespace halco::common;
using namespace halco::hicann_dls::v2;
using namespace haldls::v2;
using namespace stadls::v2;
namespace po = boost::program_options;

tbb::concurrent_queue<std::string> requests;
tbb::concurrent_queue<std::string> responses;

std::map<std::string, PPUMemoryWordOnDLS> addresses{
    {"InCommand", PPUMemoryWordOnDLS(0)},      {"OutCommand", PPUMemoryWordOnDLS(0)},
    {"InBuffer", PPUMemoryWordOnDLS(0)},       {"OutBuffer", PPUMemoryWordOnDLS(0)},
    {"IncomingPacket", PPUMemoryWordOnDLS(0)}, {"OutgoingPacket", PPUMemoryWordOnDLS(0)}};

void read_elf(char const* filename)
{
	auto logger = log4cxx::Logger::getLogger(__func__);

	try {
		auto symbols = ppu_program_symbols::read_symbols(filename);
		int num_found = 0;
		for (auto const x : symbols) {
			if (addresses.find(x.first) != addresses.end()) {
				addresses[x.first] = x.second.first;
				LOG4CXX_INFO(logger, x.first + " address: " << x.second.first);
				++num_found;
			}
		}
		if (num_found != 6) {
			LOG4CXX_FATAL(logger, "Didn't find all debug addresses.");
			exit(EXIT_FAILURE);
		}
		LOG4CXX_INFO(logger, "Found all addresses.");
	} catch (std::runtime_error& e) {
		LOG4CXX_FATAL(logger, e.what());
		exit(EXIT_FAILURE);
	}
}

// signal to exit execution loop
bool run = true;

// TODO: this function should come from a helper header
void print_ppu_mailbox(PPUMemory const& ppu_memory)
{
	std::cout << "Mailbox: " << std::endl;
	auto const words = ppu_memory.get_words();
	size_t addr = 0;
	for (auto const x : words) {
		if (addr++ >= 3072) {
			uint32_t w = ntohl(static_cast<uint32_t>(x.get()));
			char* c = reinterpret_cast<char*>(&w);
			std::cout << c[0] << c[1] << c[2] << c[3];
		}
	}
	std::cout << std::endl;
}

// TODO: this function should come from a helper header
void load_ppu_program(PPUMemory& memory, std::string const filename)
{
	auto logger = log4cxx::Logger::getLogger(__func__);

	LOG4CXX_INFO(logger, "Loading PPU program from " << filename << " ...");
	std::ifstream in;
	in.open(filename, std::ios::binary);
	if (in.fail()) {
		LOG4CXX_FATAL(logger, "Error loading/reading PPU program from " << filename << ".");
		exit(EXIT_FAILURE);
	}
	std::vector<uint8_t> buffer(
	    (std::istreambuf_iterator<char>(in)), (std::istreambuf_iterator<char>()));

	in.close();
	size_t const size_in_words = (buffer.size() - 1) / sizeof(uint32_t) + 1;

	if (size_in_words > 4095) {
		LOG4CXX_FATAL(logger, "PPU program too large");
		exit(EXIT_FAILURE);
	}
	std::vector<uint32_t> program(size_in_words, 0);
	std::copy(buffer.begin(), buffer.end(), reinterpret_cast<char*>(&program[0]));
	std::for_each(program.begin(), program.end(), [](uint32_t& x) { x = htonl(x); });
	size_t addr = 0;
	for (auto x : program) {
		memory.set_word(PPUMemoryWordOnDLS(addr), PPUMemoryWord::Value(x));
		addr++;
	}
	LOG4CXX_INFO(logger, "PPU program loaded.");
}

int main(int argc, char* argv[])
{
	po::options_description opt_desc("Options");
	int port;
	std::string test_board;
	std::string program_filename;
	std::string stripped_program_filename;
	opt_desc.add_options()("info", "Print informational messages")(
	    "print-mailbox", "Print mailbox")("help", "Print help message and exit")(
	    "port", po::value<int>(&port)->required(), "Port to use")(
	    "board-id", po::value<std::string>(&test_board)->required(), "Board ID")(
	    "program", po::value<std::string>(&program_filename)->required(),
	    "PPU program (suffix .bin, non-stripped)")(
	    "stripped-program", po::value<std::string>(&stripped_program_filename)->required(),
	    "Stripped PPU Program (suffix .binary)");
	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(opt_desc).run(), vm);
	if (vm.count("help")) {
		std::cout << "PPU GDB Server" << std::endl << std::endl << opt_desc << std::endl;
		exit(0);
	}
	po::notify(vm);
	if (vm.count("info"))
		logger_default_config(log4cxx::Level::getInfo());
	else
		logger_default_config(log4cxx::Level::getWarn());

	auto logger = log4cxx::Logger::getRootLogger();
	LOG4CXX_INFO(logger, "Started PPU GDB server");
	// read debug addresses from elf file
	read_elf(program_filename.c_str());

	// start tcp server
	std::thread server_thread(server, std::move(port));
	LOG4CXX_INFO(logger, "Started TCP server; listening on port " << port);

	// initialize board and chip
	Board board;
	Chip chip;

	// build startup program
	PlaybackProgramBuilder builder;
	// wait time, i.e. time between reads from / writes to ppu
	uint32_t wait = 1000000;
	// load ppu program
	auto ppu_memory = PPUMemory();
	load_ppu_program(ppu_memory, stripped_program_filename);
	// build program deploying ppu program and starting ppu
	PPUControlRegister ppu_control_register;
	ppu_control_register.set_inhibit_reset(false);
	builder.write(PPUControlRegisterOnDLS(), ppu_control_register);
	ppu_control_register.set_inhibit_reset(true);
	builder.write(PPUMemoryOnDLS(), ppu_memory);
	builder.write(PPUControlRegisterOnDLS(), ppu_control_register);
	builder.halt();

	// start ppu program
	auto start_program = builder.done();
	LocalBoardControl ctrl(test_board);
	ctrl.configure_static(board, chip);
	ctrl.run(start_program);
	LOG4CXX_INFO(logger, "Started program on PPU");

	// enter debugging loop
	while (run) {
		builder.wait_until(wait);
		auto ppu_memory_ticket = builder.read<PPUMemory>(PPUMemoryOnDLS());
		auto ppu_status_ticket = builder.read<PPUStatusRegister>(PPUStatusRegisterOnDLS());
		builder.halt();
		auto program = builder.done();
		LOG4CXX_TRACE(logger, "Running built program...");
		ctrl.run(program);
		ppu_memory = program.get(ppu_memory_ticket);
		auto ppu_status = program.get(ppu_status_ticket);
		if (ppu_status.get_sleep()) {
			// PPU stopped, tell gdb and exit cleanly
			// FIXME: insert correct exit code from PPU here
			responses.push("W0");
		}
		LOG4CXX_TRACE(logger, "Read PPU memory:");
		LOG4CXX_TRACE(
		    logger, "IncomingPacket = " << ppu_memory.get_word(addresses["IncomingPacket"]));
		LOG4CXX_TRACE(logger, "InCommand = " << ppu_memory.get_word(addresses["InCommand"]));
		LOG4CXX_TRACE(
		    logger, "OutgoingPacket = " << ppu_memory.get_word(addresses["OutgoingPacket"]));
		LOG4CXX_TRACE(logger, "OutCommand = " << ppu_memory.get_word(addresses["OutCommand"]));
		if (ppu_memory.get_word(addresses["IncomingPacket"]) == 0) {
			handle_request(builder, ppu_memory);
		}
		if (ppu_memory.get_word(addresses["OutgoingPacket"]) != 0) {
			handle_response(builder, ppu_memory);
		}
	}
	if (vm.count("print-mailbox"))
		print_ppu_mailbox(ppu_memory);
	server_thread.join();
	return 0;
}
