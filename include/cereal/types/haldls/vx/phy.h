#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx {

struct PhyConfigFPGA;
struct PhyConfigChip;
struct CommonPhyConfigFPGA;
struct CommonPhyConfigChip;
struct PhyStatus;

namespace detail {

struct PhyConfigBase;

} // namespace detail

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PhyConfigFPGA& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PhyConfigChip& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CommonPhyConfigFPGA& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CommonPhyConfigChip& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PhyStatus& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::detail::PhyConfigBase& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::detail::PhyConfigBase);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PhyConfigFPGA);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PhyConfigChip);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CommonPhyConfigFPGA);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CommonPhyConfigChip);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PhyStatus);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_phy)
