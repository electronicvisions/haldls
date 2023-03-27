#pragma once

#include "stadls/vx/genpybind.h"
#include "stadls/vx/run_time_info.h"

namespace fisch::vx {

class PlaybackProgram;

} // namespace fisch::vx

namespace stadls::vx GENPYBIND_TAG_STADLS_VX {

/**
 * Transfer and execute the given playback program and fetch results.
 *
 * @tparam Connection Connection type to be used for running the program
 * @param connection Connection instance to be used for running the program
 * @param program PlaybackProgram to run
 * @return Run time information
 */
template <typename Connection>
RunTimeInfo run(Connection& connection, PlaybackProgram& program);

/**
 * Transfer and execute the given playback program and fetch results.
 *
 * @tparam Connection Connection type to be used for running the program
 * @param connection Connection instance to be used for running the program
 * @param program PlaybackProgram to run
 * @return Run time information
 */
template <typename Connection>
RunTimeInfo run(Connection& connection, PlaybackProgram&& program);

/**
 * Transfer and execute the given fisch playback program and fetch results.
 *
 * @tparam Connection Connection type to be used for running the program
 * @param connection Connection instance to be used for running the program
 * @param program PlaybackProgram to run
 * @return Run time information
 */
template <typename Connection>
RunTimeInfo run(Connection& connection, std::shared_ptr<fisch::vx::PlaybackProgram> const& program);

} // namespace stadls::vx

#if defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)

#include <pybind11/pybind11.h>
#include "pyhxcomm/vx/connection_handle.h"

namespace stadls::vx {

namespace detail {

template <typename... Ts>
struct RunUnrollPyBind11Helper
{
	RunUnrollPyBind11Helper(pybind11::module&){};
};

template <typename T, typename... Ts>
struct RunUnrollPyBind11Helper<std::variant<T, Ts...>>
    : RunUnrollPyBind11Helper<std::variant<Ts...>>
{
	using parent_t = RunUnrollPyBind11Helper<std::variant<Ts...>>;

	RunUnrollPyBind11Helper(pybind11::module& m) : parent_t(m)
	{
		m.def(
		    "run",
		    [](T& conn, ::stadls::vx::PlaybackProgram& prog) {
			    return ::stadls::vx::run<T>(conn, prog);
		    },
		    pybind11::call_guard<pybind11::gil_scoped_release>());
		m.def(
		    "run",
		    [](T& conn, std::shared_ptr<::fisch::vx::PlaybackProgram> const& prog) {
			    return ::stadls::vx::run<T>(conn, prog);
		    },
		    pybind11::call_guard<pybind11::gil_scoped_release>());
	}
};

} // namespace detail

} // namespace stadls::vx
#endif

#include "stadls/vx/run.tcc"
