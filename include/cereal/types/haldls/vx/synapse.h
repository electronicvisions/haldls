#pragma once
#include "cereal/types/haldls/cereal.h"
#include "haldls/vx/synapse.h"
#include <cereal/types/polymorphic.hpp>

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CommonSynramConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SynapseWeightQuad& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SynapseLabelQuad& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SynapseCorrelationCalibQuad& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SynapseQuad& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ColumnCorrelationQuad& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ColumnCurrentQuad& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CorrelationReset& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SynapseBiasSelection& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CommonSynramConfig)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SynapseWeightQuad)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SynapseLabelQuad)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SynapseCorrelationCalibQuad)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SynapseQuad)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::ColumnCorrelationQuad::ColumnCorrelationSwitch)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ColumnCorrelationQuad)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::ColumnCurrentQuad::ColumnCurrentSwitch)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ColumnCurrentQuad)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CorrelationReset)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SynapseBiasSelection)

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_synapse)
