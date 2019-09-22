#pragma once
#include "hate/nil.h"
#include "hate/type_traits.h"
#include "hate/visibility.h"
#include "stadls/vx/genpybind.h"
#include "stadls/vx/playback_program_builder.h"

namespace stadls::vx GENPYBIND_TAG_STADLS_VX {

/**
 * Return type of generate() call on playback sequence.
 * Allows named access to generated PlaybackProgramBuilder and to be specified Result value.
 * @tparam Result Result data structure, e.g. read-ticket data
 */
template <typename Result>
struct PlaybackGeneratorReturn
{
	PlaybackProgramBuilder builder;
	Result result;
};


/**
 * Generate function to be called on an object implementing the PlaybackGenerator concept.
 * The PlaybackGenerator concept for a type Generator is comprised of having a nested Result type
 * and a constant function with signature `PlaybackGeneratorReturn<Generator::Result> generate()
 * const;`.
 */
template <typename Seq>
auto generate(Seq const& seq)
{
	static_assert(
	    std::is_same<
	        decltype(std::declval<Seq>().generate()),
	        PlaybackGeneratorReturn<typename Seq::Result>>::value,
	    "Derived class lacks generate() function returning "
	    "PlaybackGeneratorReturn<Seq::Result>");

	return seq.generate();
}


/**
 * Abstract base class for implementation of a playback sequence in python.
 */
struct SYMBOL_VISIBLE PlaybackGenerator
{
	virtual pybind11::tuple generate() const = 0;
	virtual ~PlaybackGenerator() = 0;
};

namespace detail {

template <typename T>
pybind11::tuple py_generate_impl(T const& seq)
{
	using namespace pybind11::literals;
	auto const [pbp, res] = ::stadls::vx::generate<T>(seq);
	if constexpr (std::is_same<hate::remove_all_qualifiers_t<decltype(res)>, hate::Nil>::value) {
		auto tuple = pybind11::make_tuple<pybind11::return_value_policy::move>(
		    std::move(pbp), pybind11::cast<pybind11::none>(Py_None));
		return tuple;
	} else {
		auto tuple = pybind11::make_tuple<pybind11::return_value_policy::move>(std::move(pbp), res);
		return tuple;
	}
}

} // namespace detail

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

} // namespace stadls::vx
