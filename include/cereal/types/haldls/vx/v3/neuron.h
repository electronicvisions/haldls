#pragma once
#include "cereal/types/haldls/cereal.h"
#include "cereal/types/haldls/vx/neuron.h"
#include <cereal/types/polymorphic.hpp>

namespace halco::hicann_dls::vx::v3 {

struct Coordinates;

} // namespace halco::hicann_dls::vx::v3

namespace haldls::vx {

template <typename Coordinates>
struct NeuronBackendConfig;

namespace v3 {

struct NeuronConfig;
struct NeuronResetQuad;
typedef haldls::vx::NeuronBackendConfig<halco::hicann_dls::vx::v3::Coordinates> NeuronBackendConfig;

} // namespace v3

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::v3::NeuronConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::v3::NeuronResetQuad& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v3::NeuronConfig);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v3::NeuronResetQuad);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v3::NeuronBackendConfig);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_v3_neuron)
