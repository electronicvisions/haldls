#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx {

struct EventSwitchSource;
struct EventSwitchReadout;
struct EventSwitchConfig;
struct ExtollSpikeCommBucketTriggerConfig;
struct ExtollSpikeCommBucketDestinationConfig;
struct ExtollSpikeCommBucketNumPktsSent;
struct ExtollSpikeCommBucketCounterReset;
struct ExtollSpikeCommRouterLookupConfig;
struct ExtollSpikeCommRouterConfig;
struct ExtollSpikeCommRouterEventLossDisabled;
struct ExtollSpikeCommRouterEventLossMisconf;
struct ExtollSpikeCommRouterEventsRouted;
struct ExtollSpikeCommRouterCounterReset;
struct ExtollSpikeCommDecoderNumEventsReceived;
struct ExtollSpikeCommDecoderCounterReset;
struct ExtollSpikeCommTimestampDelayConfig;
struct ExtollSpikeCommTimestampDelayNumEventsReceived;
struct ExtollSpikeCommTimestampDelayEventLossFull;
struct ExtollSpikeCommTimestampDelayEventLossExpired;
struct ExtollSpikeCommTimestampDelayCounterReset;
struct ExtollBarrierTriggerReached;
struct ExtollBarrierReleased;
struct ExtollBarrierConfig;
struct ExtollInterruptControl;
struct ExtollInterruptConfig;
struct ExtollBarrierInterruptInportErrorCount;
struct ExtollBarrierInterruptInportCounterReset;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::EventSwitchSource& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::EventSwitchReadout& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::EventSwitchConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommBucketTriggerConfig& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommBucketTriggerConfig& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommBucketDestinationConfig& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommBucketDestinationConfig& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommBucketNumPktsSent& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommBucketCounterReset& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommRouterLookupConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommRouterConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommRouterEventLossDisabled& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommRouterEventLossMisconf& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommRouterEventsRouted& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommRouterCounterReset& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommDecoderNumEventsReceived& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommDecoderCounterReset& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommTimestampDelayConfig& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommTimestampDelayNumEventsReceived& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommTimestampDelayEventLossFull& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommTimestampDelayEventLossExpired& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommTimestampDelayCounterReset& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollBarrierTriggerReached& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollBarrierReleased& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollBarrierConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollInterruptControl& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollInterruptConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollBarrierInterruptInportErrorCount& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollBarrierInterruptInportCounterReset& value,
    std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::EventSwitchSource)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::EventSwitchReadout)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::EventSwitchConfig)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommBucketTriggerConfig)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommBucketDestinationConfig)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommBucketNumPktsSent)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommBucketCounterReset)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommRouterLookupConfig)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommRouterConfig)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommRouterEventLossDisabled)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommRouterEventLossMisconf)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommRouterEventsRouted)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommRouterCounterReset)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommDecoderNumEventsReceived)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommDecoderCounterReset)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommTimestampDelayConfig)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommTimestampDelayNumEventsReceived)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommTimestampDelayEventLossFull)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommTimestampDelayEventLossExpired)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommTimestampDelayCounterReset)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollBarrierTriggerReached)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollBarrierReleased)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollBarrierConfig)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollInterruptControl)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollInterruptConfig)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollBarrierInterruptInportErrorCount)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollBarrierInterruptInportCounterReset)

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_extoll)
