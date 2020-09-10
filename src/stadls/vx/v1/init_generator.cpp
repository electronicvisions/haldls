#include "stadls/vx/init_generator.tcc"

#include "stadls/vx/v1/init_generator.h"

template class stadls::vx::detail::
    InitGenerator<stadls::vx::v1::PlaybackProgramBuilder, halco::hicann_dls::vx::v1::Coordinates>;
template class stadls::vx::
    DigitalInit<stadls::vx::v1::PlaybackProgramBuilder, halco::hicann_dls::vx::v1::Coordinates>;

template std::ostream& stadls::vx::detail::operator<<(
    std::ostream&,
    stadls::vx::detail::InitGenerator<
        stadls::vx::detail::PlaybackProgramBuilderAdapter<
            fisch::vx::PlaybackProgramBuilder,
            stadls::vx::PlaybackProgram,
            stadls::vx::v1::CoordinateToContainer>,
        halco::hicann_dls::vx::v1::Coordinates> const&);

namespace stadls::vx::v1 {

ExperimentInit::ExperimentInit() :
    stadls::vx::detail::
        InitGenerator<PlaybackProgramBuilder, halco::hicann_dls::vx::v1::Coordinates>(),
    common_neuron_backend_config(),
    column_correlation_quad_config(),
    column_current_quad_config(),
    capmem_config()
{
	this->enable_capmem = true;
	for (auto const coord :
	     halco::common::iter_all<halco::hicann_dls::vx::v1::CommonNeuronBackendConfigOnDLS>()) {
		common_neuron_backend_config[coord].set_enable_clocks(true);
	}
}

PlaybackGeneratorReturn<ExperimentInit::Result> ExperimentInit::generate() const
{
	using namespace haldls::vx;
	using namespace halco::hicann_dls::vx::v1;
	using namespace halco::common;

	auto [builder, res] = stadls::vx::generate(
	    *static_cast<stadls::vx::detail::InitGenerator<
	        PlaybackProgramBuilder, halco::hicann_dls::vx::v1::Coordinates> const*>(this));

	// Write common neuron backend config
	for (auto coord : iter_all<CommonNeuronBackendConfigOnDLS>()) {
		builder.write(coord, common_neuron_backend_config[coord]);
	}

	// Set column correlation quad config
	for (auto coord : iter_all<ColumnCorrelationQuadOnDLS>()) {
		builder.write(coord, column_correlation_quad_config[coord]);
	}

	// Set column current quad config
	for (auto coord : iter_all<ColumnCurrentQuadOnDLS>()) {
		builder.write(coord, column_current_quad_config[coord]);
	}

	// Set capmem config for all blocks
	for (auto coord : iter_all<halco::hicann_dls::vx::v1::CapMemBlockOnDLS>()) {
		builder.write(coord, capmem_config[coord]);
	}

	return {std::move(builder), res};
}

} // namespace stadls::vx::v1
