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
std::shared_ptr<haldls::v2::PlaybackProgram> get_configure_program(haldls::v2::Chip chip);

class GENPYBIND(visible) ExperimentControl
{
public:
	/// \brief Automatically choose board to connect to (locally or remotely) via set environmental
	/// variables
	ExperimentControl() SYMBOL_VISIBLE;

	/// \brief Explicitly connect to a local board using the usb serial
	ExperimentControl(std::string const& usb_serial_number) SYMBOL_VISIBLE;

	/// \brief Explicitly connect to remote board via quiggeldy using given ip and port
	ExperimentControl(std::string const& ip, uint16_t port) SYMBOL_VISIBLE;

	ExperimentControl(ExperimentControl&& other) noexcept SYMBOL_VISIBLE;
	ExperimentControl& operator=(ExperimentControl&& other) noexcept SYMBOL_VISIBLE;

	ExperimentControl(ExperimentControl const& other) = delete;
	ExperimentControl& operator=(ExperimentControl const& other) = delete;

	~ExperimentControl() SYMBOL_VISIBLE;

	/// \brief Run experiment on given board and chip
	void run_experiment(
	    haldls::v2::Board const& board,
	    haldls::v2::Chip const& chip,
	    std::shared_ptr<haldls::v2::PlaybackProgram> const& playback_program) SYMBOL_VISIBLE;

private:
	class Impl;
	std::unique_ptr<Impl> m_impl;
}; // ExperimentControl

} // namespace v2
} // namespace stadls
