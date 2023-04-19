#include "cereal/types/haldls/vx/vector_generator.h"

#include "cereal/types/haldls/cereal.tcc"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/vx/vector_generator.h"

namespace cereal {

template <class Archive>
void serialize(Archive& ar, haldls::vx::VectorGeneratorControl& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_source_count));
	ar(CEREAL_NVP(value.m_resend_count));
	ar(CEREAL_NVP(value.m_source_waits));
	ar(CEREAL_NVP(value.m_sources));
	ar(CEREAL_NVP(value.m_event_pack));
	ar(CEREAL_NVP(value.m_enable_ignore_zeros));
	ar(CEREAL_NVP(value.m_lookup_table_entry_start));
	ar(CEREAL_NVP(value.m_notification));
	ar(CEREAL_NVP(value.m_trigger));
	ar(CEREAL_NVP(value.m_signal));
}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::VectorGeneratorLUTEntry& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_value));
}

template <typename Archive>
void serialize(
    Archive& ar, haldls::vx::VectorGeneratorNotificationAddress& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_value));
}

template <typename Archive>
void serialize(Archive&, haldls::vx::VectorGeneratorTrigger&, std::uint32_t const)
{}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::VectorGeneratorFIFOWord& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_values));
	ar(CEREAL_NVP(value.m_last));
	ar(CEREAL_NVP(value.m_enable));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::VectorGeneratorControl)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::VectorGeneratorLUTEntry)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::VectorGeneratorNotificationAddress)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::VectorGeneratorTrigger)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::VectorGeneratorFIFOWord)
CEREAL_REGISTER_TYPE(haldls::vx::VectorGeneratorControl)
CEREAL_REGISTER_TYPE(haldls::vx::VectorGeneratorLUTEntry)
CEREAL_REGISTER_TYPE(haldls::vx::VectorGeneratorNotificationAddress)
CEREAL_REGISTER_TYPE(haldls::vx::VectorGeneratorFIFOWord)
CEREAL_REGISTER_TYPE(haldls::vx::VectorGeneratorTrigger)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::VectorGeneratorControl)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::VectorGeneratorLUTEntry)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::VectorGeneratorNotificationAddress)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::VectorGeneratorFIFOWord)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::VectorGeneratorTrigger)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_vector_generator)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::VectorGeneratorControl)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::VectorGeneratorLUTEntry)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::VectorGeneratorNotificationAddress)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::VectorGeneratorFIFOWord)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::VectorGeneratorTrigger)
