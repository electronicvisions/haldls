#pragma once

#include <memory>
#include <string>
#include <vector>

#include "hate/visibility.h"
#include "haldls/v2/board.h"
#include "haldls/v2/chip.h"
#include "haldls/v2/playback.h"

namespace stadls {
namespace v2 GENPYBIND(tag(stadls_v2)) {

class GENPYBIND(visible) ExperimentControl {
public:
	/// \brief creates Flyspi communication object and calls soft_reset
	ExperimentControl(std::string const& usb_serial_number) SYMBOL_VISIBLE;

	ExperimentControl(ExperimentControl&& other) noexcept SYMBOL_VISIBLE;
	ExperimentControl& operator=(ExperimentControl&& other) noexcept SYMBOL_VISIBLE;

	ExperimentControl(ExperimentControl const& other) = delete;
	ExperimentControl& operator=(ExperimentControl const& other) = delete;

	~ExperimentControl() SYMBOL_VISIBLE;

	/// \brief toggle soft reset and chip reset and restore fpga to default config
	void soft_reset() SYMBOL_VISIBLE;

	void configure_static(haldls::v2::Board const& board, haldls::v2::Chip const& chip) SYMBOL_VISIBLE;

	/// \brief transfers the program and sets the program size and address
	///        registers
	void transfer(haldls::v2::PlaybackProgram const& playback_program) SYMBOL_VISIBLE;
	/// \brief toggle the execute flag and wait until turned off again
	void execute() SYMBOL_VISIBLE;
	void fetch(haldls::v2::PlaybackProgram& playback_program) SYMBOL_VISIBLE;

	/// \brief this just wraps the sequence transfer-execute-fetch
	void run(haldls::v2::PlaybackProgram& playback_program) SYMBOL_VISIBLE;

private:
	class Impl;
	std::unique_ptr<Impl> m_impl;
}; // ExperimentControl

std::vector<std::string> available_board_usb_serial_numbers() SYMBOL_VISIBLE GENPYBIND(visible);

} // namespace v2
} // namespace stadls
