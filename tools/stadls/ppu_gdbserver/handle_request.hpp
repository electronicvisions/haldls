#include <cstdlib>
#include <netinet/in.h>

#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <tbb/concurrent_queue.h>

#include "halco/hicann-dls/v2/coordinates.h"
#include "haldls/v2/board.h"
#include "haldls/v2/chip.h"
#include "haldls/v2/playback.h"
#include "logging_ctrl.h"

#include "gdb_protocol.hpp"
#include "ppu_registers.hpp"

extern tbb::concurrent_queue<std::string> requests;
extern tbb::concurrent_queue<std::string> responses;

extern std::map<std::string, halco::hicann_dls::v2::PPUMemoryWordOnDLS> addresses;

extern bool run;

void write_command(haldls::v2::PlaybackProgramBuilder& builder, uint32_t const command)
{
	haldls::v2::PPUMemoryWord::Value command_to_ppu;
	command_to_ppu = haldls::v2::PPUMemoryWord::Value(command);
	builder.write<haldls::v2::PPUMemoryWord>(
	    addresses["InCommand"], haldls::v2::PPUMemoryWord(command_to_ppu));
	builder.write<haldls::v2::PPUMemoryWord>(
	    addresses["IncomingPacket"],
	    haldls::v2::PPUMemoryWord(haldls::v2::PPUMemoryWord::Value(true)));
}

void write_buffer(haldls::v2::PlaybackProgramBuilder& builder, std::vector<uint32_t> const words)
{
	auto size = haldls::v2::PPUMemoryBlock::Size(words.size());
	haldls::v2::PPUMemoryBlock block(size);
	std::vector<haldls::v2::PPUMemoryWord> memory_words;
	for (size_t i = 0; i < words.size(); ++i) {
		memory_words.push_back(
		    haldls::v2::PPUMemoryWord(haldls::v2::PPUMemoryWord::Value(words[i])));
	}
	block.set_words(memory_words);
	builder.write<haldls::v2::PPUMemoryBlock>(std::pair(addresses["InBuffer"], halco::hicann_dls::v2::PPUMemoryWordOnDLS(addresses["InBuffer"] + words.size() - 1)), block);
}

/*
    Handle request from requests queue.
*/
void handle_request(haldls::v2::PlaybackProgramBuilder& builder, haldls::v2::PPUMemory& memory)
{
	std::string request;
	if (requests.try_pop(request)) {
		auto logger = log4cxx::Logger::getLogger(__func__);

		LOG4CXX_INFO(logger, "Handling request from GDB...");

		haldls::v2::PPUMemoryWord::Value command_to_ppu;

		LOG4CXX_INFO(logger, "Request: " << request);
		switch (request[0]) {
			case 'q': { // general query
				if (request.find("qSupported") != std::string::npos) {
					// we support nothing, but have to return max. packetsize
					responses.push("Packetsize=4000;multiprocess-;swbreak+;hwbreak+");
				} else if (request.find("qTStatus") != std::string::npos) {
					// no traces running (TODO: introduce PPU code here)
					responses.push("T0");
				} else if (request.find("qC") != std::string::npos) {
					responses.push("");
				} else if (request.find("qAttached") != std::string::npos) {
					responses.push("0");
				} else if (request.find("qfThreadInfo") != std::string::npos) {
					responses.push("m0");
				} else if (request.find("qsThreadInfo") != std::string::npos) {
					responses.push("l");
				} else {
					// respond empty, if not known
					LOG4CXX_INFO(logger, "Received unknown q command from GDB");
					responses.push("");
				}
				break;
			}
			case '?': { // interrupt signal
				write_command(builder, '?');
				break;
			}
			case 'v': {
				if (request.find("vCont?") != std::string::npos) {
					// the vCont package is not supported
					responses.push("");
				} else {
					// respond empty, if not known
					LOG4CXX_ERROR(logger, "Received unknown v command from GDB");
					responses.push("");
				}
				break;
			}
			case 'c': { // continue
				write_command(builder, 'c');
				break;
			}
			case 's': { // single step
				write_command(builder, 's');
				break;
			}
			case 'g': { // get registers
				write_command(builder, 'g');
				break;
			}
			case 'G': {
				write_command(builder, 'G');
				std::stringstream ss;
				ss << request.substr(1);
				std::vector<uint32_t> buffer;
				// general purpose and special purpose registers
				uint32_t word;
				for (size_t i = 0; i < NUM_GPR + NUM_SPR; ++i) {
					ss >> word;
					buffer.push_back(word);
				}
				// vector conditional register unavailable
				ss >> word;
				// vector registers
				for (uint32_t i = 0; i < NUM_VR * SIZEOF_VR / sizeof(uint32_t); ++i) {
					ss >> word;
					buffer.push_back(word);
				}
				write_buffer(builder, buffer);
				break;
			}
			case 'p': {
				std::stringstream ss;
				ss << std::hex << request.substr(1);
				uint32_t regnum;
				ss >> regnum;
				if (regnum != 71) {
					std::vector<uint32_t> buffer;
					buffer.push_back(regnum);
					write_buffer(builder, buffer);
					write_command(builder, 'p');
				} else {
					responses.push(
					    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
				}
				break;
			}
			case 'H': {
				// FIXME: Is this always the right response?
				responses.push("OK");
				break;
			}
			case 'k': { // exit gdbserver
				run = false;
				break;
			}
			case 'm': {
				// FIXME: magic numbering
				std::vector<uint8_t> memory_bytes;
				std::stringstream ss1;
				ss1 << std::hex << request.substr(1, request.find(',') - 1);
				uint32_t addr;
				ss1 >> addr;
				std::stringstream ss2;
				ss2 << std::hex << request.substr(request.find(',') + 1);
				size_t num_bytes;
				ss2 >> num_bytes;
				std::vector<uint32_t> buffer;
				for (uint32_t a = addr / sizeof(uint32_t);
				     a < (addr + num_bytes + sizeof(uint32_t) - 1) / sizeof(uint32_t); ++a) {
					if (a < 4096) {
						buffer.push_back(
						    ntohl(memory.get_word(halco::hicann_dls::v2::PPUMemoryWordOnDLS(a))));
					} else {
						// out of bounds request
						responses.push("E01");
					}
				}
				std::stringstream ss;
				for (uint32_t i = addr % sizeof(uint32_t); i < buffer.size() * sizeof(uint32_t);
				     ++i) {
					ss << std::hex << std::setfill('0') << std::setw(2)
					   << static_cast<uint32_t>(reinterpret_cast<uint8_t*>(
					          &buffer[i / sizeof(uint32_t)])[i % sizeof(uint32_t)]);
				}
				responses.push(ss.str());
				break;
			}
			case 'P': {
				std::stringstream ss1;
				ss1 << std::hex << request.substr(1, request.find('=') - 1);
				std::vector<uint32_t> buffer;
				uint32_t regnum;
				ss1 >> regnum;
				buffer.push_back(regnum);
				std::string s2 = request.substr(request.find('=') + 1);
				for (size_t i = 0; i < s2.size() / (2 * sizeof(uint32_t)); ++i) {
					std::stringstream ss2;
					std::string s3 = s2.substr(i * 2 * sizeof(uint32_t), 2 * sizeof(uint32_t));
					ss2 << std::hex << s3;
					uint32_t r;
					ss2 >> r;
					buffer.push_back(r);
				}
				write_buffer(builder, buffer);
				write_command(builder, 'P');
				break;
			}
			case 'M': {
				std::vector<uint32_t> buffer;

				std::stringstream ss1;
				ss1 << std::hex << request.substr(1, request.find(',') - 1);
				uint32_t addr;
				ss1 >> addr;
				addr /= sizeof(uint32_t);
				buffer.push_back(addr);

				std::stringstream ss2;
				ss2 << std::hex << request.substr(request.find(',') + 1, request.find(':') - 1);
				size_t length;
				ss2 >> length;
				length = (length + sizeof(uint32_t) - 1) / sizeof(uint32_t);
				buffer.push_back(length);

				std::stringstream ss3;
				ss3 << std::hex << request.substr(request.find(':') + 1);
				std::vector<uint32_t> words;
				for (size_t i = 0; i < length; ++i) {
					uint32_t r;
					ss3 >> r;
					r = ntohl(r);
					words.push_back(r);
				}
				if (length != 0) {
					auto size = haldls::v2::PPUMemoryBlock::Size(length);
					haldls::v2::PPUMemoryBlock block(size);
					std::vector<haldls::v2::PPUMemoryWord> memory_words;
					for (size_t i = 0; i < words.size(); ++i) {
						memory_words.push_back(
						    haldls::v2::PPUMemoryWord(haldls::v2::PPUMemoryWord::Value(words[i])));
					}
					block.set_words(memory_words);
					builder.write<haldls::v2::PPUMemoryBlock>(std::pair(
						halco::hicann_dls::v2::PPUMemoryWordOnDLS(addr),
						halco::hicann_dls::v2::PPUMemoryWordOnDLS(addr + length - 1)),
						block);

					write_buffer(builder, buffer);
					write_command(builder, 'M');
				} else {
					responses.push("OK");
				}
				break;
			}
			case 'X': {
				std::vector<uint32_t> buffer;

				std::stringstream ss1;
				ss1 << std::hex << request.substr(1, request.find(',') - 1);
				uint32_t addr;
				ss1 >> addr;
				addr /= sizeof(uint32_t);
				buffer.push_back(addr);

				std::stringstream ss2;
				ss2 << std::hex << request.substr(request.find(',') + 1, request.find(':') - 1);
				size_t length;
				ss2 >> length;
				length = (length + sizeof(uint32_t) - 1) / sizeof(uint32_t);
				buffer.push_back(length);

				std::string s3 = erase_escape_sequences_from_binary_string(
				    request.substr(request.find(':') + 1));
				std::vector<uint32_t> words;
				for (size_t i = 0; i < length; ++i) {
					uint32_t r;
					std::string sub = s3.substr(i, i + sizeof(uint32_t));
					std::copy(sub.begin(), sub.end(), reinterpret_cast<char*>(&r));
					r = ntohl(r);
					words.push_back(r);
				}
				if (length != 0) {
					auto size = haldls::v2::PPUMemoryBlock::Size(length);
					haldls::v2::PPUMemoryBlock block(size);
					std::vector<haldls::v2::PPUMemoryWord> memory_words;
					for (size_t i = 0; i < words.size(); ++i) {
						memory_words.push_back(
						    haldls::v2::PPUMemoryWord(haldls::v2::PPUMemoryWord::Value(words[i])));
					}
					block.set_words(memory_words);
					builder.write<haldls::v2::PPUMemoryBlock>(std::pair(
						halco::hicann_dls::v2::PPUMemoryWordOnDLS(addr),
						halco::hicann_dls::v2::PPUMemoryWordOnDLS(addr + length - 1)),
						block);

					write_buffer(builder, buffer);
					write_command(builder, 'X');
				} else {
					responses.push("OK");
				}
				break;
			}
			case 'Z': { // enable breakpoints, watchpoints
				// direct set is not supported
				responses.push("");
				break;
			}
			case 'z': { // disable breakpoints, watchpoints
				// direct set is not supported
				responses.push("");
				break;
			}
			default:
				// respond empty, if request is not known
				LOG4CXX_ERROR(logger, "Received unknown command from GDB");
				responses.push("");
				break;
		}
	}
}
