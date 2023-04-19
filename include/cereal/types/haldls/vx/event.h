#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx {

struct SpikePack1ToChip;
struct SpikePack2ToChip;
struct SpikePack3ToChip;
struct SpikeFromChip;
struct MADCSampleFromChip;
struct HighspeedLinkNotification;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SpikePack1ToChip& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SpikePack2ToChip& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SpikePack3ToChip& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SpikeFromChip& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::MADCSampleFromChip& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::HighspeedLinkNotification& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SpikePack1ToChip);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SpikePack2ToChip);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SpikePack3ToChip);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SpikeFromChip);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::MADCSampleFromChip);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::HighspeedLinkNotification);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_event)
