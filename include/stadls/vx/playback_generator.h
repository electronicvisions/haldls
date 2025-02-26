#pragma once
#include "hate/nil.h"
#include "hate/type_traits.h"
#include "hate/visibility.h"
#include "stadls/vx/genpybind.h"

#if defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)
#include <pybind11/pybind11.h>
#endif

namespace stadls {
namespace vx GENPYBIND_TAG_STADLS_VX {

/**
 * Return type of generate() call on playback sequence.
 * Allows named access to generated PlaybackProgramBuilder and to be specified Result value.
 * @tparam Result Result data structure, e.g. read-ticket data
 */
template <typename Builder, typename Result>
struct PlaybackGeneratorReturn
{
	Builder builder;
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
	        PlaybackGeneratorReturn<typename Seq::Builder, typename Seq::Result>>::value,
	    "Derived class lacks generate() function returning "
	    "PlaybackGeneratorReturn<Seq::Builder, Seq::Result>");

	return seq.generate();
}


#if defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)
/**
 * Abstract base class for implementation of a playback sequence in python.
 */
struct SYMBOL_VISIBLE PlaybackGenerator
{
	virtual pybind11::tuple generate() const = 0;
	virtual ~PlaybackGenerator() {}
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

#endif // defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)

} // namespace vx
} // namespace stadls
