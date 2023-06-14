#include "cereal/types/haldls/vx/timer.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/haldls/cereal.tcc"
#include "haldls/vx/timer.h"

namespace cereal {

template <class Archive>
void serialize(Archive& ar, haldls::vx::Timer& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_value));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::Timer)
CEREAL_REGISTER_TYPE(haldls::vx::Timer)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::Timer)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::Timer)

CEREAL_REGISTER_TYPE(haldls::vx::Timer::Value)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::BlockUntil, haldls::vx::Timer::Value)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::Timer::Value)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_timer)
