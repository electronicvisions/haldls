#pragma once
#include "cereal/types/haldls/cereal.h"

namespace stadls::vx::detail {

template <typename DoneType>
struct Dumper;

} // namespace stadls::vx::detail

namespace cereal {

template <typename Archive, typename DoneType>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, stadls::vx::detail::Dumper<DoneType>& value, std::uint32_t const version);

} // namespace cereal
