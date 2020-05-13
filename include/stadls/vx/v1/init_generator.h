#pragma once
#include "hate/visibility.h"
#include "stadls/vx/init_generator.h"
#include "stadls/vx/v1/playback_generator.h"
#include "stadls/vx/v1/playback_program_builder.h"

extern template class SYMBOL_VISIBLE
    stadls::vx::detail::InitGenerator<stadls::vx::v1::PlaybackProgramBuilder>;
extern template class SYMBOL_VISIBLE
    stadls::vx::ExperimentInit<stadls::vx::v1::PlaybackProgramBuilder>;
extern template class SYMBOL_VISIBLE
    stadls::vx::DigitalInit<stadls::vx::v1::PlaybackProgramBuilder>;

extern template SYMBOL_VISIBLE std::ostream& stadls::vx::detail::operator<<(
    std::ostream&,
    stadls::vx::detail::InitGenerator<stadls::vx::detail::PlaybackProgramBuilderAdapter<
        fisch::vx::PlaybackProgramBuilder,
        stadls::vx::PlaybackProgram> > const&);

namespace stadls::vx::v1 GENPYBIND_TAG_STADLS_VX_V1 {

namespace detail {

using InitGenerator GENPYBIND(opaque(true), expose_as(_InitGenerator)) =
    stadls::vx::detail::InitGenerator<stadls::vx::v1::PlaybackProgramBuilder>;

} // namespace detail

using ExperimentInit = stadls::vx::ExperimentInit<stadls::vx::v1::PlaybackProgramBuilder>;
using DigitalInit = stadls::vx::DigitalInit<stadls::vx::v1::PlaybackProgramBuilder>;

namespace detail {

// TODO: we can't use the alias above, cf. https://github.com/kljohann/genpybind/issues/32
struct GENPYBIND(expose_as(ExperimentInit), inline_base("*ExperimentInit*")) PyExperimentInit
    : public /*ExperimentInit*/ stadls::vx::ExperimentInit<stadls::vx::v1::PlaybackProgramBuilder>
    , public PlaybackGenerator
{
	virtual pybind11::tuple generate() const override
	{
		return stadls::vx::detail::py_generate_impl(static_cast<ExperimentInit>(*this));
	}
};

struct GENPYBIND(expose_as(DigitalInit), inline_base("*DigitalInit*")) PyDigitalInit
    : public /*DigitalInit*/ stadls::vx::DigitalInit<stadls::vx::v1::PlaybackProgramBuilder>
    , public PlaybackGenerator
{
	virtual pybind11::tuple generate() const override
	{
		return stadls::vx::detail::py_generate_impl(static_cast<DigitalInit>(*this));
	}
};

} // namespace detail

GENPYBIND_MANUAL({
	parent.def(
	    "generate",
	    [](::stadls::vx::v1::ExperimentInit const& seq) {
		    return ::stadls::vx::detail::py_generate_impl(seq);
	    },
	    pybind11::return_value_policy::move);
})

GENPYBIND_MANUAL({
	parent.def(
	    "generate",
	    [](::stadls::vx::v1::DigitalInit const& seq) {
		    return ::stadls::vx::detail::py_generate_impl(seq);
	    },
	    pybind11::return_value_policy::move);
})

} // namespace stadls::vx::v1
