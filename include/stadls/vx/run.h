#pragma once

#include "stadls/vx/genpybind.h"
#include "stadls/vx/run_time_info.h"

namespace fisch::vx {

class PlaybackProgram;

} // namespace fisch::vx

namespace stadls {
namespace vx GENPYBIND_TAG_STADLS_VX {

/**
 * Transfer and execute the given playback program and fetch results.
 *
 * @tparam Connection Connection type to be used for running the program
 * @param connection Connection instance to be used for running the program
 * @param program PlaybackProgram to run
 * @return Run time information
 */
template <typename Connection>
RunTimeInfo run(
    Connection& connection, std::vector<std::reference_wrapper<PlaybackProgram>> const& program);

/**
 * Transfer and execute the given fisch playback program and fetch results.
 *
 * @tparam Connection Connection type to be used for running the program
 * @param connection Connection instance to be used for running the program
 * @param program PlaybackProgram to run
 * @return Run time information
 */
template <typename Connection>
RunTimeInfo run(
    Connection& connection,
    std::vector<std::shared_ptr<fisch::vx::PlaybackProgram>> const& programs);

} // namespace vx
} // namespace stadls

#if defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)

#include "pyhxcomm/vx/connection_handle.h"
#include <pybind11/pybind11.h>

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
		    [](T& conn,
		       std::vector<std::reference_wrapper<::stadls::vx::PlaybackProgram>> const& programs) {
			    return ::stadls::vx::run<T>(conn, programs);
		    },
		    pybind11::call_guard<pybind11::gil_scoped_release>());
		m.def(
		    "run",
		    [](T& conn,
		       std::vector<std::shared_ptr<::fisch::vx::PlaybackProgram>> const& programs) {
			    return ::stadls::vx::run<T>(conn, programs);
		    },
		    pybind11::call_guard<pybind11::gil_scoped_release>());
	}
};

} // namespace detail

} // namespace stadls::vx
#endif

#include "stadls/vx/run.tcc"
