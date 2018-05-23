#pragma once

#include <memory>
#include <string>
#include <vector>

#include "haldls/v2/board.h"
#include "haldls/v2/chip.h"
#include "haldls/v2/common.h"
#include "haldls/v2/playback.h"
#include "hate/visibility.h"

namespace stadls {
namespace v2 GENPYBIND(tag(stadls_v2)) {

haldls::v2::PlaybackProgram get_configure_program(haldls::v2::Chip chip);

class GENPYBIND(visible) LocalBoardControl
{
public:
	/// \brief creates Flyspi communication object and calls soft_reset
	LocalBoardControl(std::string const& usb_serial_number) SYMBOL_VISIBLE;

	LocalBoardControl(LocalBoardControl&& other) noexcept SYMBOL_VISIBLE;
	LocalBoardControl& operator=(LocalBoardControl&& other) noexcept SYMBOL_VISIBLE;

	LocalBoardControl(LocalBoardControl const& other) = delete;
	LocalBoardControl& operator=(LocalBoardControl const& other) = delete;

	~LocalBoardControl() SYMBOL_VISIBLE;

	std::string usb_serial() const SYMBOL_VISIBLE;

	/// \brief toggle soft reset and chip reset and restore fpga to default config
	void soft_reset() SYMBOL_VISIBLE;

	void configure_static(
		std::vector<haldls::v2::ocp_address_type> const& board_addresses,
		std::vector<haldls::v2::ocp_word_type> const& board_words,
		std::vector<std::vector<haldls::v2::instruction_word_type> > const& chip_program_bytes)
		SYMBOL_VISIBLE;
	void configure_static(haldls::v2::Board const& board, haldls::v2::Chip const& chip)
		SYMBOL_VISIBLE;

	/// \brief transfers the program and sets the program size and address
	///        registers
	void transfer(std::vector<std::vector<haldls::v2::instruction_word_type> > const& program_bytes)
		SYMBOL_VISIBLE;
	void transfer(haldls::v2::PlaybackProgram const& playback_program) SYMBOL_VISIBLE;

	/// \brief toggle the execute flag and wait until turned off again
	void execute() SYMBOL_VISIBLE;

	std::vector<haldls::v2::instruction_word_type> fetch() SYMBOL_VISIBLE;
	void fetch(haldls::v2::PlaybackProgram& playback_program) SYMBOL_VISIBLE;

	static void decode_result_bytes(
		std::vector<haldls::v2::instruction_word_type> const& result_bytes,
		haldls::v2::PlaybackProgram& playback_program) SYMBOL_VISIBLE;

	/// \brief this just wraps the sequence transfer-execute-fetch
	std::vector<haldls::v2::instruction_word_type> run(
		std::vector<std::vector<haldls::v2::instruction_word_type> > const& program_byte)
		SYMBOL_VISIBLE;
	void run(haldls::v2::PlaybackProgram& playback_program) SYMBOL_VISIBLE;

private:
	class Impl;
	std::unique_ptr<Impl> m_impl;
}; // LocalBoardControl

std::vector<std::string> available_board_usb_serial_numbers() SYMBOL_VISIBLE GENPYBIND(visible);

} // namespace v2
} // namespace stadls
