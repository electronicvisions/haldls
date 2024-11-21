#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx {

struct CapMemSRAMTimingConfig;

template <typename Coordinates>
struct CapMemCell;

template <typename Coordinates>
struct CapMemBlock;

template <typename Coordinates>
struct CapMemBlockConfig;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CapMemSRAMTimingConfig& value, std::uint32_t const version);

template <typename Archive, typename Coordinates>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CapMemCell<Coordinates>& value, std::uint32_t const version);

template <typename Archive, typename Coordinates>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CapMemBlock<Coordinates>& value, std::uint32_t const version);

template <typename Archive, typename Coordinates>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CapMemBlockConfig<Coordinates>& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CapMemSRAMTimingConfig)

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_capmem)
