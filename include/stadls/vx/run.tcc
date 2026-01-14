
#include "hxcomm/common/visit_connection.h"

#include "fisch/vx/run.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/run_time_info.h"

#include <set>
#include <vector>

namespace stadls::vx {

template <typename Connection>
RunTimeInfo run(
    Connection& connection, std::vector<std::reference_wrapper<PlaybackProgram>> const& programs)
{
	auto const run_impl = [&programs](auto& conn) {
		for (auto& program : programs) {
			std::vector<hxcomm::vx::Target> remaining;

			auto const supported = std::set(conn.supported_targets);
			auto const& unsupported_unsorted = program.get().get_unsupported_targets();
			auto const unsupported =
			    std::set(unsupported_unsorted.begin(), unsupported_unsorted.end());
			std::set_difference(
			    supported.begin(), supported.end(), unsupported.begin(), unsupported.end(),
			    std::back_inserter(remaining));
			if (remaining.empty()) {
				throw std::runtime_error("Trying to execute program on unsupported target.");
			}
		}

		std::vector<std::shared_ptr<fisch::vx::PlaybackProgram>> fisch_playback_programs;

		for (auto& program : programs) {
			fisch_playback_programs.emplace_back(program.get().m_program_impl);
		}

		return stadls::vx::run(conn, fisch_playback_programs);
	};

	// TO-DO: Add info about single connections e.g. unique id ???
	auto unique_ids = hxcomm::visit_connection(
	    [](auto& conn) { return conn.get_unique_identifier(); }, connection);

	auto const print_highspeed_notifications = [&programs, &unique_ids]() {
		std::stringstream ss;
		for (size_t i = 0; i < programs.size(); i++) {
			auto& program = programs.at(i).get();
			ss << unique_ids.at(i);

			if (program.get_highspeed_link_notifications().empty()) {
				ss << " got no highspeed-link notifications.";
			} else {
				ss << " got highspeed-link notifications:" << std::endl;
				ss << hate::join(program.get_highspeed_link_notifications(), ",\n");
			}
			ss << "\n";
		}
		return ss.str();
	};
	auto logger = log4cxx::Logger::getLogger("stadls.run");
	RunTimeInfo time_info;
	try {
		time_info = hxcomm::visit_connection(run_impl, connection);
		LOG4CXX_TRACE(logger, print_highspeed_notifications());
	} catch (std::runtime_error const& error) {
		LOG4CXX_INFO(logger, print_highspeed_notifications());
		throw error;
	}
	return time_info;
}

template <typename Connection>
RunTimeInfo run(
    Connection& connection,
    std::vector<std::shared_ptr<fisch::vx::PlaybackProgram>> const& programs)
{
	return fisch::vx::run(connection, programs);
}

} // namespace stadls::vx
