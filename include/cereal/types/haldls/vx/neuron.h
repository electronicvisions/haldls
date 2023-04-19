#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx {

struct NeuronSRAMTimingConfig;
struct NeuronBackendSRAMTimingConfig;
struct CommonNeuronBackendConfig;
struct NeuronReset;
struct BlockPostPulse;
struct SpikeCounterRead;
struct SpikeCounterReset;

template <typename Coordinates>
struct NeuronBackendConfig;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::NeuronSRAMTimingConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::NeuronBackendSRAMTimingConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CommonNeuronBackendConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::NeuronReset& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::BlockPostPulse& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SpikeCounterRead& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SpikeCounterReset& value, std::uint32_t const version);

template <typename Archive, typename Coordinates>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::NeuronBackendConfig<Coordinates>& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::NeuronSRAMTimingConfig);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::NeuronBackendSRAMTimingConfig);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CommonNeuronBackendConfig);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::NeuronReset);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::BlockPostPulse);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SpikeCounterRead);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SpikeCounterReset);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_neuron)
