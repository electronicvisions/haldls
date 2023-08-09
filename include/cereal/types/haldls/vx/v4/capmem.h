#pragma once
#include "cereal/types/haldls/cereal.h"
#include "cereal/types/haldls/vx/capmem.h"
#include <cereal/types/polymorphic.hpp>

namespace halco::hicann_dls::vx::v4 {

struct Coordinates;

} // namespace halco::hicann_dls::vx::v4

namespace haldls::vx {

template <typename Coordinates>
struct CapMemCell;

template <typename Coordinates>
struct CapMemBlock;

template <typename Coordinates>
struct CapMemBlockConfig;

namespace v4 {

typedef haldls::vx::CapMemCell<halco::hicann_dls::vx::v4::Coordinates> CapMemCell;
typedef haldls::vx::CapMemBlock<halco::hicann_dls::vx::v4::Coordinates> CapMemBlock;
typedef haldls::vx::CapMemBlockConfig<halco::hicann_dls::vx::v4::Coordinates> CapMemBlockConfig;
struct ReferenceGeneratorConfig;

} // namespace v4

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::v4::ReferenceGeneratorConfig& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v4::CapMemCell);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v4::CapMemBlock);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v4::CapMemBlockConfig);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v4::ReferenceGeneratorConfig);

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_v4_capmem)
