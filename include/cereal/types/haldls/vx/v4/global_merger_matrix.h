#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx::v4 {

struct GlobalMergerMatrixOutputConfig;
struct GlobalMergerMatrixInputDropCounter;
struct GlobalMergerMatrixOutputEventCounter;
struct GlobalMergerMatrixNode;
struct NeuronLabelToGL1EventLUTEntry;

} // namespace haldls::vx::v4

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::v4::GlobalMergerMatrixOutputConfig& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::v4::GlobalMergerMatrixInputDropCounter& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::v4::GlobalMergerMatrixOutputEventCounter& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::v4::GlobalMergerMatrixNode& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::v4::NeuronLabelToGL1EventLUTEntry& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v4::GlobalMergerMatrixOutputConfig);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v4::GlobalMergerMatrixInputDropCounter);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v4::GlobalMergerMatrixOutputEventCounter);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v4::GlobalMergerMatrixNode);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v4::NeuronLabelToGL1EventLUTEntry);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_v4_global_merger_matrix)
