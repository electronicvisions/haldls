#include "cereal/types/haldls/vx/routing_table_entry.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "haldls/vx/routing_table_entry.h"

namespace cereal {

template <class Archive>
void serialize(Archive& ar, haldls::vx::OutputRoutingTableEntry& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable));
	ar(CEREAL_NVP(value.m_label));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::InputRoutingTableEntry& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable));
	ar(CEREAL_NVP(value.m_label));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::OutputRoutingTableEntry)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::InputRoutingTableEntry)

CEREAL_REGISTER_TYPE(haldls::vx::OutputRoutingTableEntry)
CEREAL_REGISTER_TYPE(haldls::vx::InputRoutingTableEntry)

CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::OutputRoutingTableEntry)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::InputRoutingTableEntry)

CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::OutputRoutingTableEntry)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::InputRoutingTableEntry)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_routing_table_entry)