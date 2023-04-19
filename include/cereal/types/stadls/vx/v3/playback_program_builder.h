#pragma once
#include "cereal/types/haldls/cereal.h"
#include "cereal/types/stadls/vx/playback_program_builder.h"
#include <cereal/types/polymorphic.hpp>

namespace fisch::vx {

struct PlaybackProgramBuilder;

} // namespace fisch::vx

namespace stadls::vx {

struct PlaybackProgram;

namespace detail {

template <typename BuilderStorage, typename DoneType>
struct PlaybackProgramBuilderAdapter;

} // namespace detail

namespace v3 {

typedef stadls::vx::detail::
    PlaybackProgramBuilderAdapter<fisch::vx::PlaybackProgramBuilder, stadls::vx::PlaybackProgram>
        PlaybackProgramBuilder;

} // namespace v3

} // namespace stadls::vx

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(stadls::vx::v3::PlaybackProgramBuilder)

CEREAL_FORCE_DYNAMIC_INIT(stadls_vx_v3_playback_program_builder)
