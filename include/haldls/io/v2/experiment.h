#pragma once

#include <memory>
#include <string>
#include <vector>

#include "haldls/common/visibility.h"
#include "haldls/container/v2/board.h"
#include "haldls/container/v2/chip.h"
#include "haldls/container/v2/playback.h"

namespace haldls {
namespace io {
namespace v2 GENPYBIND(tag(haldls_io_v2)) {

class GENPYBIND(visible) ExperimentControl {
public:
	/// \brief creates Flyspi communication object and calls soft_reset
	ExperimentControl(std::string const& usb_serial_number) HALDLS_VISIBLE;

	ExperimentControl(ExperimentControl&& other) noexcept HALDLS_VISIBLE;
	ExperimentControl& operator=(ExperimentControl&& other) noexcept HALDLS_VISIBLE;

	ExperimentControl(ExperimentControl const& other) = delete;
	ExperimentControl& operator=(ExperimentControl const& other) = delete;

	~ExperimentControl() HALDLS_VISIBLE;

	/// \brief toggle soft reset and chip reset and restore fpga to default config
	void soft_reset() HALDLS_VISIBLE;

	void configure_static(container::v2::Board const& board, container::v2::Chip const& chip) HALDLS_VISIBLE;

	/// \brief transfers the program and sets the program size and address
	///        registers
	void transfer(container::v2::PlaybackProgram const& playback_program) HALDLS_VISIBLE;
	/// \brief toggle the execute flag and wait until turned off again
	void execute() HALDLS_VISIBLE;
	void fetch(container::v2::PlaybackProgram& playback_program) HALDLS_VISIBLE;

	/// \brief this just wraps the sequence transfer-execute-fetch
	void run(container::v2::PlaybackProgram& playback_program) HALDLS_VISIBLE;

private:
	class Impl;
	std::unique_ptr<Impl> m_impl;
}; // ExperimentControl

std::vector<std::string> available_board_usb_serial_numbers() HALDLS_VISIBLE GENPYBIND(visible);

} // namespace v2
} // namespace io
} // namespace haldls
