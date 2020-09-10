#include "stadls/vx/init_generator.tcc"

#include "stadls/vx/v2/init_generator.h"

template class stadls::vx::detail::
    InitGenerator<stadls::vx::v2::PlaybackProgramBuilder, halco::hicann_dls::vx::v2::Coordinates>;
template class stadls::vx::
    ExperimentInit<stadls::vx::v2::PlaybackProgramBuilder, halco::hicann_dls::vx::v2::Coordinates>;
template class stadls::vx::
    DigitalInit<stadls::vx::v2::PlaybackProgramBuilder, halco::hicann_dls::vx::v2::Coordinates>;

template std::ostream& stadls::vx::detail::operator<<(
    std::ostream&,
    stadls::vx::detail::InitGenerator<
        stadls::vx::detail::PlaybackProgramBuilderAdapter<
            fisch::vx::PlaybackProgramBuilder,
            stadls::vx::PlaybackProgram,
            stadls::vx::v2::CoordinateToContainer>,
        halco::hicann_dls::vx::v2::Coordinates> const&);
