
#include "hxcomm/common/connection_variant.h"
#include "hxcomm/common/visit_connection.h"

#include "hxcomm/vx/connection_parameter.h"

#include "fisch/vx/run.h"
#include "stadls/vx/playback_program.h"

namespace stadls::vx {

template <typename Connection>
RunTimeInfo run(Connection& connection, PlaybackProgram& program)
{
	if constexpr (hxcomm::ConnectionIsPlain<Connection>::value) {
		auto const executable_restriction = program.get_executable_restriction();
		if (executable_restriction) {
			if (((*executable_restriction == ExecutorBackend::simulation) &&
			     std::is_same_v<
			         Connection, hxcomm::ARQConnection<hxcomm::vx::ConnectionParameter>>) ||
			    ((*executable_restriction == ExecutorBackend::hardware) &&
			     std::is_same_v<
			         Connection, hxcomm::SimConnection<hxcomm::vx::ConnectionParameter>>) ) {
				throw std::runtime_error(
				    "Trying to execute program with non-matching executable restriction.");
			}
		}
		return stadls::vx::run(connection, program.m_program_impl);
	} else {
		return hxcomm::visit_connection(
		    [&program](auto& conn) { return run(conn, program); }, connection);
	}
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
