#pragma once

#include <memory>
#include <string>
#include <vector>

#include "haldls/common/visibility.h"
#include "haldls/container/v2/board.h"
#include "haldls/container/v2/chip.h"

namespace haldls {
namespace io {
namespace v2 GENPYBIND(tag(haldls_io_v2)) {

class PlaybackProgram;

class GENPYBIND(visible) ExperimentControl {
	ExperimentControl(std::string const& usb_serial_number);

public:
	ExperimentControl(ExperimentControl&& other) noexcept HALDLS_VISIBLE;
	ExperimentControl& operator=(ExperimentControl&& other) noexcept HALDLS_VISIBLE;

	ExperimentControl(ExperimentControl const& other) = delete;
	ExperimentControl& operator=(ExperimentControl const& other) = delete;

	~ExperimentControl() HALDLS_VISIBLE;

	void soft_reset() HALDLS_VISIBLE;

	void configure_static(container::v2::Board const& board, container::v2::Chip const& chip) HALDLS_VISIBLE;

	void transfer(PlaybackProgram const& playback_program) HALDLS_VISIBLE;
	void execute() HALDLS_VISIBLE;
	void fetch(PlaybackProgram& playback_program) HALDLS_VISIBLE;

	void run(PlaybackProgram& playback_program) HALDLS_VISIBLE;

	friend ExperimentControl connect(std::string const& usb_serial_number);

private:
	class Impl;
	std::unique_ptr<Impl> m_impl;
}; // ExperimentControl

ExperimentControl connect(std::string const& usb_serial_number) HALDLS_VISIBLE GENPYBIND(visible);

std::vector<std::string> available_board_usb_serial_numbers() HALDLS_VISIBLE GENPYBIND(visible);

} // namespace v2
} // namespace io
} // namespace haldls
