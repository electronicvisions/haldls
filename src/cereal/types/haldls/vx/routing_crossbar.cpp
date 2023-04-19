#include "cereal/types/haldls/vx/routing_crossbar.h"

#include "cereal/types/haldls/cereal.tcc"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/vx/routing_crossbar.h"

namespace cereal {

template <class Archive>
void serialize(Archive& ar, haldls::vx::CrossbarOutputConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable_slow));
	ar(CEREAL_NVP(value.m_enable_event_counter));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::CrossbarInputDropCounter& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_value));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::CrossbarOutputEventCounter& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_value));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::CrossbarNode& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_mask));
	ar(CEREAL_NVP(value.m_target));
	ar(CEREAL_NVP(value.m_enable_drop_counter));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CrossbarOutputConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CrossbarInputDropCounter)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CrossbarOutputEventCounter)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CrossbarNode)
CEREAL_REGISTER_TYPE(haldls::vx::CrossbarOutputConfig)
CEREAL_REGISTER_TYPE(haldls::vx::CrossbarInputDropCounter)
CEREAL_REGISTER_TYPE(haldls::vx::CrossbarOutputEventCounter)
CEREAL_REGISTER_TYPE(haldls::vx::CrossbarNode)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::CrossbarOutputConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::CrossbarInputDropCounter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::CrossbarOutputEventCounter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::CrossbarNode)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_routing_crossbar)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::CrossbarOutputConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::CrossbarInputDropCounter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::CrossbarOutputEventCounter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::CrossbarNode)
