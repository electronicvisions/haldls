#pragma once
#include "cereal/types/haldls/cereal.h"
#include "cereal/types/haldls/vx/capmem.h"
#include <cereal/types/polymorphic.hpp>

namespace halco::hicann_dls::vx::v3 {

struct Coordinates;

} // namespace halco::hicann_dls::vx::v3

namespace haldls::vx {

template <typename Coordinates>
struct CapMemCell;

template <typename Coordinates>
struct CapMemBlock;

template <typename Coordinates>
struct CapMemBlockConfig;

namespace v3 {

typedef haldls::vx::CapMemCell<halco::hicann_dls::vx::v3::Coordinates> CapMemCell;
typedef haldls::vx::CapMemBlock<halco::hicann_dls::vx::v3::Coordinates> CapMemBlock;
typedef haldls::vx::CapMemBlockConfig<halco::hicann_dls::vx::v3::Coordinates> CapMemBlockConfig;
struct ReferenceGeneratorConfig;

} // namespace v3

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::v3::ReferenceGeneratorConfig& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v3::CapMemCell);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v3::CapMemBlock);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v3::CapMemBlockConfig);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v3::ReferenceGeneratorConfig);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_v3_capmem)
