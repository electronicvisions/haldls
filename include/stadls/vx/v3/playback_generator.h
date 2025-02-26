#pragma once
#include "stadls/vx/genpybind.h"
#include "stadls/vx/playback_generator.h"
#include "stadls/vx/v3/playback_program_builder.h"

namespace stadls::vx {
namespace v3 GENPYBIND_TAG_STADLS_VX_V3 {

template <typename Result>
using PlaybackGeneratorReturn = stadls::vx::PlaybackGeneratorReturn<PlaybackProgramBuilder, Result>;

template <typename... Ts>
auto generate(Ts&&... args) -> decltype(stadls::vx::generate(std::forward<Ts>(args)...))
{
	return stadls::vx::generate(std::forward<Ts>(args)...);
}

#if defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)
// only used for python
using PlaybackGenerator GENPYBIND(visible) = stadls::vx::PlaybackGenerator;
#endif

GENPYBIND_MANUAL({
	struct PyPlaybackGenerator : public ::stadls::vx::PlaybackGenerator
	{
		using ::stadls::vx::PlaybackGenerator::PlaybackGenerator;

		virtual pybind11::tuple generate() const override
		{
			PYBIND11_OVERLOAD_PURE(pybind11::tuple, ::stadls::vx::PlaybackGenerator, generate, );
		}
	};

	// clang-format off
	parent->py::template class_<::stadls::vx::PlaybackGenerator, PyPlaybackGenerator>(
	        parent, "PlaybackGenerator")
	    .def(parent->py::template init<>())
	    .def("generate", &::stadls::vx::PlaybackGenerator::generate);
	// clang-format on

	parent.def(
	    "generate", [](::stadls::vx::PlaybackGenerator const& self) { return self.generate(); },
	    pybind11::return_value_policy::move);
})

} // namespace v3
} // namespace stadls::vx
