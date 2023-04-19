#include "cereal/types/haldls/vx/systime.h"

#include "cereal/types/haldls/cereal.tcc"
#include "halco/common/cerealization_geometry.h"
#include "haldls/vx/systime.h"

namespace cereal {

template <class Archive>
void serialize(Archive& ar, haldls::vx::SystimeSyncBase& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_value));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::SystimeSync& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_do_sync));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SystimeSyncBase)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SystimeSync)
CEREAL_REGISTER_TYPE(haldls::vx::SystimeSyncBase)
CEREAL_REGISTER_TYPE(haldls::vx::SystimeSync)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::SystimeSyncBase)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::SystimeSync)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_systime)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::SystimeSyncBase)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::SystimeSync)
