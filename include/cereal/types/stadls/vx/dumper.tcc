#pragma once
#include "cereal/types/haldls/cereal.h"
#include "cereal/types/stadls/vx/dumper.h"


namespace cereal {

template <typename Archive, typename DoneType>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, stadls::vx::detail::Dumper<DoneType>& value, std::uint32_t const /* version */)
{
	ar(CEREAL_NVP_("m_dumpit", value.m_dumpit));
}

} // namespace cereal
