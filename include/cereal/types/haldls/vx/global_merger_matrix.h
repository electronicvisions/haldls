#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx {

struct GlobalMergerMatrixOutputConfig;
struct GlobalMergerMatrixInputDropCounter;
struct GlobalMergerMatrixOutputEventCounter;
struct GlobalMergerMatrixNode;
struct NeuronLabelToGL1EventLUTEntry;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::GlobalMergerMatrixOutputConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::GlobalMergerMatrixInputDropCounter& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::GlobalMergerMatrixOutputEventCounter& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::GlobalMergerMatrixNode& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::NeuronLabelToGL1EventLUTEntry& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::GlobalMergerMatrixOutputConfig);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::GlobalMergerMatrixInputDropCounter);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::GlobalMergerMatrixOutputEventCounter);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::GlobalMergerMatrixNode);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::NeuronLabelToGL1EventLUTEntry);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_global_merger_matrix)
