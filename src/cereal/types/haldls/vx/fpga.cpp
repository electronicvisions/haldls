#include "cereal/types/haldls/vx/fpga.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "haldls/vx/fpga.h"

namespace cereal {

template <class Archive>
void serialize(Archive& ar, haldls::vx::FPGASystimeSyncNumRetries& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_value));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::FPGASystimeSyncLastRTT& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_value));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::FPGASystimeSyncLastAsicSystime& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_value));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::FPGASystimeSyncActiveState& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_active_state));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::FPGADeviceDNA& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_value));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::EventRecordingConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable_event_recording));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::InstructionTimeoutConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_value));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::SystimeCorrectionBarrierConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable_interrupt));
}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::ExternalPPUMemoryByte& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_value));
}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::ExternalPPUMemoryQuad& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_quad));
	ar(CEREAL_NVP(value.m_enables));
}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::SpikeIOConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_data_rate_scaler));
	ar(CEREAL_NVP(value.m_enable_tx));
	ar(CEREAL_NVP(value.m_enable_rx));
	ar(CEREAL_NVP(value.m_enable_internal_loopback));
}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::SpikeIOInputRoute& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_target));
}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::SpikeIOOutputRoute& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_target));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::FPGASystimeSyncNumRetries)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::FPGASystimeSyncLastRTT)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::FPGASystimeSyncLastAsicSystime)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::FPGASystimeSyncActiveState)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::FPGADeviceDNA)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::EventRecordingConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::InstructionTimeoutConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SystimeCorrectionBarrierConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExternalPPUMemoryByte)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExternalPPUMemoryQuad)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SpikeIOConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SpikeIOInputRoute)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SpikeIOOutputRoute)
CEREAL_REGISTER_TYPE(haldls::vx::FPGADeviceDNA)
CEREAL_REGISTER_TYPE(haldls::vx::EventRecordingConfig)
CEREAL_REGISTER_TYPE(haldls::vx::InstructionTimeoutConfig)
CEREAL_REGISTER_TYPE(haldls::vx::SystimeCorrectionBarrierConfig)
CEREAL_REGISTER_TYPE(haldls::vx::ExternalPPUMemoryByte)
CEREAL_REGISTER_TYPE(haldls::vx::ExternalPPUMemoryQuad)
CEREAL_REGISTER_TYPE(haldls::vx::SpikeIOConfig)
CEREAL_REGISTER_TYPE(haldls::vx::SpikeIOInputRoute)
CEREAL_REGISTER_TYPE(haldls::vx::SpikeIOOutputRoute)
CEREAL_REGISTER_TYPE(haldls::vx::FPGASystimeSyncLastAsicSystime)
CEREAL_REGISTER_TYPE(haldls::vx::FPGASystimeSyncLastRTT)
CEREAL_REGISTER_TYPE(haldls::vx::FPGASystimeSyncActiveState)
CEREAL_REGISTER_TYPE(haldls::vx::FPGASystimeSyncNumRetries)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::FPGADeviceDNA)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::EventRecordingConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::InstructionTimeoutConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::SystimeCorrectionBarrierConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::ExternalPPUMemoryByte)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::ExternalPPUMemoryQuad)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::SpikeIOConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::SpikeIOInputRoute)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::SpikeIOOutputRoute)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::FPGASystimeSyncLastAsicSystime)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::FPGASystimeSyncLastRTT)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::FPGASystimeSyncActiveState)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::FPGASystimeSyncNumRetries)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_fpga)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::FPGADeviceDNA)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::EventRecordingConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::InstructionTimeoutConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::SystimeCorrectionBarrierConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::ExternalPPUMemoryByte)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::ExternalPPUMemoryQuad)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::SpikeIOConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::SpikeIOInputRoute)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::SpikeIOOutputRoute)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::FPGASystimeSyncLastAsicSystime)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::FPGASystimeSyncLastRTT)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::FPGASystimeSyncActiveState)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::FPGASystimeSyncNumRetries)
