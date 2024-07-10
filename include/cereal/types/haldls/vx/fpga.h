#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx {

struct FPGADeviceDNA;
struct EventRecordingConfig;
struct InstructionTimeoutConfig;
struct SystimeCorrectionBarrierConfig;
struct ExternalPPUMemoryByte;
struct ExternalPPUMemoryQuad;
struct ExternalPPUDRAMMemoryByte;
struct ExternalPPUDRAMMemoryQuad;
struct SpikeIOConfig;
struct SpikeIOInputRoute;
struct SpikeIOOutputRoute;
struct FPGASystimeSyncLastAsicSystime;
struct FPGASystimeSyncLastRTT;
struct FPGASystimeSyncActiveState;
struct FPGASystimeSyncNumRetries;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::FPGADeviceDNA& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::EventRecordingConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::InstructionTimeoutConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SystimeCorrectionBarrierConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExternalPPUMemoryByte& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExternalPPUMemoryQuad& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExternalPPUDRAMMemoryByte& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExternalPPUDRAMMemoryQuad& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SpikeIOConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SpikeIOInputRoute& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SpikeIOOutputRoute& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::FPGASystimeSyncLastAsicSystime& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::FPGASystimeSyncLastRTT& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::FPGASystimeSyncActiveState& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::FPGASystimeSyncNumRetries& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::FPGADeviceDNA)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::EventRecordingConfig)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::InstructionTimeoutConfig)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SystimeCorrectionBarrierConfig)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExternalPPUMemoryByte)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExternalPPUMemoryQuad)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExternalPPUDRAMMemoryByte)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExternalPPUDRAMMemoryQuad)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SpikeIOConfig)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SpikeIOInputRoute)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SpikeIOOutputRoute)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::FPGASystimeSyncLastAsicSystime)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::FPGASystimeSyncLastRTT)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::FPGASystimeSyncActiveState)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::FPGASystimeSyncNumRetries)

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_fpga)
