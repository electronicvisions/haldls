#include "stadls/v2/experiment.h"

#include <chrono>
#include <sstream>
#include <thread>
#include <boost/variant.hpp>

#include "log4cxx/logger.h"

#include "stadls/v2/local_board_control.h"
#include "stadls/v2/quick_queue.h"

namespace stadls {
namespace v2 {

class ExperimentControl::Impl
{
public:
	Impl();

	Impl(std::string const& usb_serial_number);

	Impl(std::string const& ip, uint16_t port);

	Impl(Impl&&) noexcept = default;

	Impl& operator=(Impl&&) noexcept = default;

	~Impl() = default;

	void run_experiment(
	    haldls::v2::Board const& board,
	    haldls::v2::Chip const& chip,
	    std::shared_ptr<haldls::v2::PlaybackProgram> const& playback_program);

	using control_t = typename boost::variant<LocalBoardControl, QuickQueueClient>;

	class RunExperimentVisitor : public boost::static_visitor<void>
	{
	public:
		RunExperimentVisitor(
		    haldls::v2::Board const& board,
		    haldls::v2::Chip const& chip,
		    std::shared_ptr<haldls::v2::PlaybackProgram> const& playback_program)
		    : m_board(board), m_chip(chip), m_playback_program(playback_program)
		{}

		template <typename T>
		void operator()(T& ctrl) const
		{
			ctrl.run_experiment(m_board, m_chip, m_playback_program);
		}

		haldls::v2::Board const& m_board;
		haldls::v2::Chip const& m_chip;
		std::shared_ptr<haldls::v2::PlaybackProgram> const& m_playback_program;
	};

	std::unique_ptr<control_t> m_control;
};

ExperimentControl::ExperimentControl() : m_impl(new Impl()) {}

ExperimentControl::ExperimentControl(std::string const& usb_serial_number)
	: m_impl(new Impl(usb_serial_number))
{}

ExperimentControl::ExperimentControl(std::string const& ip, uint16_t port)
	: m_impl(new Impl(ip, port))
{}

ExperimentControl::ExperimentControl(ExperimentControl&&) noexcept = default;

ExperimentControl& ExperimentControl::operator=(ExperimentControl&&) noexcept = default;

ExperimentControl::~ExperimentControl() = default;

void ExperimentControl::run_experiment(
    haldls::v2::Board const& board,
    haldls::v2::Chip const& chip,
    std::shared_ptr<haldls::v2::PlaybackProgram> const& playback_program)
{
	m_impl->run_experiment(board, chip, playback_program);
}

ExperimentControl::Impl::Impl()
{
	char const* env_ip = std::getenv(QuickQueueClient::env_name_ip);
	char const* env_port = std::getenv(QuickQueueClient::env_name_port);
	if ((env_ip != nullptr) && (env_port != nullptr)) {
		// ip and port set so we can connect!
		std::string ip(env_ip);
		uint16_t port((uint16_t) atoi(env_port));

		m_control.reset(new control_t(QuickQueueClient(ip, port)));
	} else {
		// try and see if we have the flyspi id in our environment
		char const* env_board_id = std::getenv(LocalBoardControl::env_name_board_id);
		std::string board_id;

		if (env_board_id != nullptr) {
			board_id = std::string(env_board_id);
		} else {
			// try to find local boards
			std::vector<std::string> board_ids = available_board_usb_serial_numbers();
			if (board_ids.size() == 0) {
				std::stringstream ss;
				ss << "Neither " << QuickQueueClient::env_name_ip << "/"
				   << QuickQueueClient::env_name_port << "/" << LocalBoardControl::env_name_board_id
				   << " defined nor any local board available.";
				throw std::runtime_error(ss.str());
			}
			// just take the first by default
			board_id = board_ids.front();
		}
		m_control.reset(new control_t(LocalBoardControl(board_id)));
	}
}

ExperimentControl::Impl::Impl(std::string const& usb_serial_number)
	: m_control(new control_t(LocalBoardControl(usb_serial_number)))
{}

ExperimentControl::Impl::Impl(std::string const& ip, uint16_t port)
	: m_control(new control_t(QuickQueueClient(ip, port)))
{}

void ExperimentControl::Impl::run_experiment(
    haldls::v2::Board const& board,
    haldls::v2::Chip const& chip,
    std::shared_ptr<haldls::v2::PlaybackProgram> const& playback_program)
{
	boost::apply_visitor(RunExperimentVisitor(board, chip, playback_program), *m_control);
}
} // namespace v2
} // namespace stadls
