#pragma once

#include <memory>
#include <string>
#include <vector>

#include "haldls/common/visibility.h"

namespace haldls {
namespace container {
namespace v2 {
class Board;
class Chip;
} // namespace v2
} // namespace container

namespace io {
namespace v2 {

class PlaybackProgram;

class ExperimentControl {
	ExperimentControl(std::string const& usb_serial_number);

public:
	ExperimentControl(ExperimentControl&& other) noexcept;
	ExperimentControl& operator=(ExperimentControl&& other) noexcept;

	ExperimentControl(ExperimentControl const& other) = delete;
	ExperimentControl& operator=(ExperimentControl const& other) = delete;

	~ExperimentControl();

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

ExperimentControl connect(std::string const& usb_serial_number) HALDLS_VISIBLE;

std::vector<std::string> available_board_usb_serial_numbers() HALDLS_VISIBLE;

} // namespace v2
} // namespace io
} // namespace haldls
