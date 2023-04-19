#include "cereal/types/haldls/vx/padi.h"

#include "cereal/types/haldls/cereal.tcc"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/vx/padi.h"

namespace cereal {

template <class Archive>
void serialize(Archive& ar, haldls::vx::PADIEvent& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_fire_bus));
	ar(CEREAL_NVP(value.m_event_address));
	ar(CEREAL_NVP(value.m_row_select_address));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::CommonPADIBusConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable_spl1));
	ar(CEREAL_NVP(value.m_enable_extended_timing));
	ar(CEREAL_NVP(value.m_dacen_pulse_extension));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::CommonSTPConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable_recovery_clock));
	ar(CEREAL_NVP(value.m_recovery_clock_speed));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PADIEvent)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CommonPADIBusConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CommonSTPConfig)
CEREAL_REGISTER_TYPE(haldls::vx::PADIEvent)
CEREAL_REGISTER_TYPE(haldls::vx::CommonPADIBusConfig)
CEREAL_REGISTER_TYPE(haldls::vx::CommonSTPConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::PADIEvent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::CommonPADIBusConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::CommonSTPConfig)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_padi)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::PADIEvent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::CommonPADIBusConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::CommonSTPConfig)
