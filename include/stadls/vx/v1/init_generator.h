#pragma once
#include "hate/visibility.h"
#include "stadls/vx/init_generator.h"
#include "stadls/vx/v1/playback_generator.h"
#include "stadls/vx/v1/playback_program_builder.h"

extern template class SYMBOL_VISIBLE stadls::vx::detail::
    InitGenerator<stadls::vx::v1::PlaybackProgramBuilder, halco::hicann_dls::vx::v1::Coordinates>;
extern template class SYMBOL_VISIBLE stadls::vx::
    DigitalInit<stadls::vx::v1::PlaybackProgramBuilder, halco::hicann_dls::vx::v1::Coordinates>;

extern template SYMBOL_VISIBLE std::ostream& stadls::vx::detail::operator<<(
    std::ostream&,
    stadls::vx::detail::InitGenerator<
        stadls::vx::detail::PlaybackProgramBuilderAdapter<
            fisch::vx::PlaybackProgramBuilder,
            stadls::vx::PlaybackProgram,
            stadls::vx::v1::CoordinateToContainer>,
        halco::hicann_dls::vx::v1::Coordinates> const&);

namespace stadls::vx::v1 GENPYBIND_TAG_STADLS_VX_V1 {

namespace detail {

using InitGenerator GENPYBIND(opaque(true), expose_as(_InitGenerator)) = stadls::vx::detail::
    InitGenerator<stadls::vx::v1::PlaybackProgramBuilder, halco::hicann_dls::vx::v1::Coordinates>;

} // namespace detail

using DigitalInit = stadls::vx::
    DigitalInit<stadls::vx::v1::PlaybackProgramBuilder, halco::hicann_dls::vx::v1::Coordinates>;

/**
 * Generator for initialization required for typical experiments.
 * Uses the InitGenerator() to establish digital communication to the chip, and
 * further initializes the CapMem in a working state and selects internal bias currents for
 * synapses.
 */
class ExperimentInit
    : public stadls::vx::detail::
          InitGenerator<PlaybackProgramBuilder, halco::hicann_dls::vx::v1::Coordinates>
{
public:
	/** Default constructor. */
	ExperimentInit() SYMBOL_VISIBLE;

	/** Builder typedef (e.g. for usage in generators). */
	typedef PlaybackProgramBuilder Builder;

	/** Set common neuron backend with clocks enabled.
	 * If clocks are disabled, it may behave strangely. */
	typedef halco::common::typed_array<
	    haldls::vx::CommonNeuronBackendConfig,
	    halco::hicann_dls::vx::v1::CommonNeuronBackendConfigOnDLS>
	    common_neuron_backend_config_type GENPYBIND(opaque(false));
	common_neuron_backend_config_type common_neuron_backend_config;

	/** Set ColumnCorrelationQuad/Switch connections. */
	typedef halco::common::typed_array<
	    haldls::vx::v1::ColumnCorrelationQuad,
	    halco::hicann_dls::vx::v1::ColumnCorrelationQuadOnDLS>
	    column_correlation_quad_type GENPYBIND(opaque(false));
	column_correlation_quad_type column_correlation_quad_config;

	/** Set ColumnCurrentQuad/Switch connections. */
	typedef halco::common::typed_array<
	    haldls::vx::v1::ColumnCurrentQuad,
	    halco::hicann_dls::vx::v1::ColumnCurrentQuadOnDLS>
	    column_current_quad_type GENPYBIND(opaque(false));
	column_current_quad_type column_current_quad_config;

	/** Set initial CapMem config.
	 * By default, a value of zero is written to all cells. */
	typedef halco::common::
	    typed_array<haldls::vx::v1::CapMemBlock, halco::hicann_dls::vx::v1::CapMemBlockOnDLS>
	        capmem_block_type GENPYBIND(opaque(false));
	capmem_block_type capmem_config;

	typedef typename stadls::vx::detail::InitGenerator<
	    PlaybackProgramBuilder,
	    halco::hicann_dls::vx::v1::Coordinates>::Result Result;

private:
	friend auto stadls::vx::generate<ExperimentInit>(ExperimentInit const&);

	/**
	 * Generate PlaybackProgramBuilder.
	 * @return PlaybackGeneratorReturn instance with sequence embodied and specified Result value
	 */
	PlaybackGeneratorReturn<Result> generate() const SYMBOL_VISIBLE;
};

namespace detail {

struct GENPYBIND(expose_as(ExperimentInit), inline_base("*ExperimentInit*")) PyExperimentInit
    : public ExperimentInit
    , public PlaybackGenerator
{
	virtual pybind11::tuple generate() const override
	{
		return stadls::vx::detail::py_generate_impl(static_cast<ExperimentInit>(*this));
	}
};

// TODO: we can't use the alias above, cf. https://github.com/kljohann/genpybind/issues/32
struct GENPYBIND(expose_as(DigitalInit), inline_base("*DigitalInit*")) PyDigitalInit
    : public /*DigitalInit*/ stadls::vx::DigitalInit<
          stadls::vx::v1::PlaybackProgramBuilder,
          halco::hicann_dls::vx::v1::Coordinates>
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
