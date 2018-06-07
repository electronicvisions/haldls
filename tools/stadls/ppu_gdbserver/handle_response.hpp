#pragma once
#include <sstream>
#include <string>
#include <vector>

#include <tbb/concurrent_queue.h>

#include <logging_ctrl.h>
#include "halco/hicann-dls/v2/coordinates.h"
#include "haldls/v2/board.h"
#include "haldls/v2/chip.h"
#include "haldls/v2/playback.h"

#include "esr_signals.hpp"
#include "gdb_protocol.hpp"
#include "ppu_registers.hpp"

extern tbb::concurrent_queue<std::string> responses;

extern std::map<std::string, halco::hicann_dls::v2::PPUMemoryWordOnDLS> addresses;

/*
    Process resposne from PPU and push to responses queue.
*/
void handle_response(haldls::v2::PlaybackProgramBuilder& builder, haldls::v2::PPUMemory memory)
{
	auto logger = log4cxx::Logger::getLogger(__func__);

	LOG4CXX_INFO(logger, "Got response from PPU...");
	// do stuff with memory
	uint32_t command = memory.get_word(addresses["OutCommand"]);
	LOG4CXX_INFO(logger, "Command from PPU: " << reinterpret_cast<char*>(&command));

	std::stringstream ss;

	switch (command) {
		case 'S': { // get interrupt signal
			uint32_t const esr = memory.get_word(addresses["OutBuffer"]);
			LOG4CXX_INFO(logger, "Got ESR: " << esr);
			uint32_t const signal = esr_to_signal(esr);
			ss << "S" << to_hex(signal, sizeof(char));
			LOG4CXX_INFO(logger, "PPU interrupted with signal " << signal);
			responses.push(ss.str());
			break;
		}
		case 'T': { // get interrupt signal, pc, and sp state
			// get register values
			uint32_t const esr = memory.get_word(addresses["OutBuffer"]);
			uint32_t const pc = memory.get_word(
			    halco::hicann_dls::v2::PPUMemoryWordOnDLS(addresses["OutBuffer"] + 1));
			uint32_t const sp = memory.get_word(
			    halco::hicann_dls::v2::PPUMemoryWordOnDLS(addresses["OutBuffer"] + 2));
			LOG4CXX_INFO(logger, "Got ESR: " << esr);
			LOG4CXX_INFO(logger, "Got PC: " << pc);
			LOG4CXX_INFO(logger, "Got SP: " << sp);
			uint32_t const signal = esr_to_signal(esr);
			// build response
			ss << "T" << to_hex(signal, sizeof(char));
			LOG4CXX_INFO(logger, "PPU interrupted with signal " << signal);
			ss << "20:" << to_hex(pc, sizeof(uint32_t));
			ss << ";1:" << to_hex(sp, sizeof(uint32_t));
			ss << ";";
			responses.push(ss.str());
			break;
		}
		case 'g': { // get registers
			// general purpose and special purpose registers
			for (size_t i = 0; i < NUM_GPR + NUM_SPR; ++i) {
				uint32_t const r = memory.get_word(
				    halco::hicann_dls::v2::PPUMemoryWordOnDLS(addresses["OutBuffer"] + i));
				ss << to_hex(r, sizeof(uint32_t));
			}
			// vector conditional register unavailable
			ss << "xxxxxxxx";
			// vector registers
			for (uint32_t i = 0; i < NUM_VR * SIZEOF_VR / sizeof(uint32_t); ++i) {
				uint32_t const r = memory.get_word(halco::hicann_dls::v2::PPUMemoryWordOnDLS(
				    addresses["OutBuffer"] + NUM_GPR + NUM_SPR + i));
				ss << to_hex(r, sizeof(uint32_t));
			}
			// vector accumulator register unavailable
			ss << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
			responses.push(ss.str());
			break;
		}
		case 'p': { // get single register
			// get register size
			uint32_t const size = memory.get_word(addresses["OutBuffer"]);
			// create response of requested register
			for (size_t i = 1; i < size / sizeof(uint32_t); ++i) {
				uint32_t const r = memory.get_word(
				    halco::hicann_dls::v2::PPUMemoryWordOnDLS(addresses["OutBuffer"] + i));
				ss << to_hex(r, sizeof(uint32_t));
			}
			responses.push(ss.str());
			break;
		}
		case 'E': { // error
			// TODO: forward to gdb
			LOG4CXX_ERROR(logger, "PPU returned error!");
			break;
		}
		case 'c': { // continued
			// no interaction with PPU or gdb wanted
			return;
		}
		case 'P': { // set single register
			ss << "OK";
			responses.push(ss.str());
			break;
		}
		case 'f': { // flushed cache
			ss << "OK";
			responses.push(ss.str());
			break;
		}
		default:
			LOG4CXX_ERROR(logger, "Received unknown command from PPU!");
			break;
	}
	LOG4CXX_INFO(logger, "Created response for GDB: " << ss.str());

	// now we're done, write back that we have read response
	builder.write<haldls::v2::PPUMemoryWord>(
	    addresses["OutgoingPacket"],
	    haldls::v2::PPUMemoryWord(haldls::v2::PPUMemoryWord::Value(false)));
	LOG4CXX_INFO(logger, "Set OutgoingPacket to false in PlaybackProgramBuilder.");
}
