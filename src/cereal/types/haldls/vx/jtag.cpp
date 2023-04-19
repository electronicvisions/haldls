#include "cereal/types/haldls/vx/jtag.h"

#include "cereal/types/haldls/cereal.tcc"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_heap_array.h"
#include "haldls/vx/jtag.h"
#include <cereal/types/array.hpp>
#include <cereal/types/vector.hpp>

namespace cereal {

template <class Archive>
void serialize(Archive& ar, haldls::vx::JTAGClockScaler& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_value));
}

template <class Archive>
void serialize(Archive&, haldls::vx::ResetJTAGTap&, std::uint32_t const)
{}

template <class Archive>
void serialize(Archive& ar, haldls::vx::JTAGIdCode& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_version));
	ar(CEREAL_NVP(value.m_part_number));
	ar(CEREAL_NVP(value.m_manufacturer_id));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::JTAGClockScaler)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ResetJTAGTap)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::JTAGIdCode)
CEREAL_REGISTER_TYPE(haldls::vx::JTAGClockScaler)
CEREAL_REGISTER_TYPE(haldls::vx::ResetJTAGTap)
CEREAL_REGISTER_TYPE(haldls::vx::JTAGIdCode)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::JTAGClockScaler)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::ResetJTAGTap)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::JTAGIdCode)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_jtag)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::JTAGClockScaler)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::ResetJTAGTap)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::JTAGIdCode)
