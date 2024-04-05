
#include "hxcomm/common/visit_connection.h"

#include "fisch/vx/run.h"
#include "stadls/vx/playback_program.h"

#include <vector>
#include <set>

namespace stadls::vx {

template <typename Connection>
RunTimeInfo run(Connection& connection, PlaybackProgram& program)
{
	auto const run_impl = [&program](auto& conn) {
		std::vector<hxcomm::vx::Target> remaining;

		auto const supported = std::set(conn.supported_targets);
		auto const& unsupported_unsorted = program.get_unsupported_targets();
		auto const unsupported = std::set(unsupported_unsorted.begin(), unsupported_unsorted.end());
		std::set_difference(
		    supported.begin(), supported.end(), unsupported.begin(), unsupported.end(),
		    std::back_inserter(remaining));
		if (remaining.empty()) {
			throw std::runtime_error("Trying to execute program on unsupported target.");
		}

		return stadls::vx::run(conn, program.m_program_impl);
	};
	auto const time_info = hxcomm::visit_connection(run_impl, connection);
	{
		auto logger = log4cxx::Logger::getLogger("stadls.run");
		std::stringstream ss;
		ss << "Got highspeed-link notifications:" << std::endl;
		ss << hate::join(program.get_highspeed_link_notifications(), ",\n");
		LOG4CXX_TRACE(logger, ss.str());
	}
	return time_info;
}

template <typename Connection>
RunTimeInfo run(Connection& connection, PlaybackProgram&& program)
{
	return stadls::vx::run(connection, program);
}

template <typename Connection>
RunTimeInfo run(Connection& connection, std::shared_ptr<fisch::vx::PlaybackProgram> const& program)
{
	return fisch::vx::run(connection, program);
}

} // namespace stadls::vx
