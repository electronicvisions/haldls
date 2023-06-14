#include "cereal/types/haldls/vx/reset.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/haldls/cereal.tcc"
#include "haldls/vx/reset.h"

namespace cereal {

template <class Archive>
void serialize(Archive& ar, haldls::vx::ResetChip& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_value));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ResetChip)
CEREAL_REGISTER_TYPE(haldls::vx::ResetChip)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::ResetChip)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_reset)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::ResetChip)
