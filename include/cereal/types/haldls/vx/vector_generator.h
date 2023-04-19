#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx {

struct VectorGeneratorControl;
struct VectorGeneratorLUTEntry;
struct VectorGeneratorNotificationAddress;
struct VectorGeneratorFIFOWord;
struct VectorGeneratorTrigger;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::VectorGeneratorControl& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::VectorGeneratorLUTEntry& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::VectorGeneratorNotificationAddress& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::VectorGeneratorFIFOWord& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::VectorGeneratorTrigger& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::VectorGeneratorControl)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::VectorGeneratorLUTEntry)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::VectorGeneratorNotificationAddress)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::VectorGeneratorFIFOWord)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::VectorGeneratorTrigger)

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_vector_generator)
