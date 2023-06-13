#include "cereal/types/haldls/vx/extoll.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "haldls/vx/extoll.h"
#include <cereal/types/array.hpp>
#include <cereal/types/vector.hpp>

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::EventSwitchSource& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_source));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::EventSwitchReadout& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_systime));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::EventSwitchConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_interrupt_armed));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommBucketTriggerConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_pulse_timeout));
	ar(CEREAL_NVP(value.m_packet_timeout));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommBucketDestinationConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_destination_node_id));
	ar(CEREAL_NVP(value.m_enable_multicast));
	ar(CEREAL_NVP(value.m_destination_delay));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommBucketNumPktsSent& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_packet_count));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommBucketNumEvtsRcvd& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_event_count));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive&, haldls::vx::ExtollSpikeCommBucketCounterReset&, std::uint32_t const)
{}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommRouterLookupConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_entry_valid));
	ar(CEREAL_NVP(value.m_destination_event));
	ar(CEREAL_NVP(value.m_bucket_id));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommRouterConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable_routing));
	ar(CEREAL_NVP(value.m_enable_drop_if_routing_disabled));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommRouterEventLossDisabled& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_lost_event_count));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommRouterEventLossMisconf& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_lost_event_count));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommRouterEventsRouted& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_event_count));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive&, haldls::vx::ExtollSpikeCommRouterCounterReset&, std::uint32_t const)
{}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommDecoderNumEventsReceived& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_event_count));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive&, haldls::vx::ExtollSpikeCommDecoderCounterReset&, std::uint32_t const)
{}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommTimestampDelayConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_forward_immediately));
	ar(CEREAL_NVP(value.m_timestamp_to_systime));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommTimestampDelayNumEventsReceived& value,
    std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_event_count));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommTimestampDelayEventLossFull& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_event_count));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommTimestampDelayEventLossExpired& value,
    std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_event_count));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive&, haldls::vx::ExtollSpikeCommTimestampDelayCounterReset&, std::uint32_t const)
{}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive&, haldls::vx::ExtollBarrierTriggerReached&, std::uint32_t const)
{}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollBarrierReleased& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_released_0));
	ar(CEREAL_NVP(value.m_released_1));
	ar(CEREAL_NVP(value.m_released_active));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollBarrierConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable));
	ar(CEREAL_NVP(value.m_child_nodes));
	ar(CEREAL_NVP(value.m_parent_nodes));
	ar(CEREAL_NVP(value.m_host_count));
	ar(CEREAL_NVP(value.m_enable_reset));
	ar(CEREAL_NVP(value.m_client_count));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollInterruptControl& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_operation_trigger));
	ar(CEREAL_NVP(value.m_interrupt));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollInterruptConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable));
	ar(CEREAL_NVP(value.m_child_nodes));
	ar(CEREAL_NVP(value.m_delay_count));
	ar(CEREAL_NVP(value.m_enable_reset));
	ar(CEREAL_NVP(value.m_enable_measure));
	ar(CEREAL_NVP(value.m_measure_counter));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollBarrierInterruptInportErrorCount& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_error_count));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive&, haldls::vx::ExtollBarrierInterruptInportCounterReset&, std::uint32_t const)
{}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::EventSwitchSource)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::EventSwitchReadout)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::EventSwitchConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommBucketTriggerConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommBucketDestinationConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommBucketNumPktsSent)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommBucketNumEvtsRcvd)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommBucketCounterReset)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommRouterLookupConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommRouterConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommRouterEventLossDisabled)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommRouterEventLossMisconf)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommRouterEventsRouted)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommRouterCounterReset)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommDecoderNumEventsReceived)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommDecoderCounterReset)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommTimestampDelayConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(
    haldls::vx::ExtollSpikeCommTimestampDelayNumEventsReceived)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommTimestampDelayEventLossFull)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(
    haldls::vx::ExtollSpikeCommTimestampDelayEventLossExpired)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollSpikeCommTimestampDelayCounterReset)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollBarrierTriggerReached)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollBarrierReleased)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollBarrierConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollInterruptControl)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollInterruptConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollBarrierInterruptInportErrorCount)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ExtollBarrierInterruptInportCounterReset)
CEREAL_REGISTER_TYPE(haldls::vx::EventSwitchSource)
CEREAL_REGISTER_TYPE(haldls::vx::EventSwitchReadout)
CEREAL_REGISTER_TYPE(haldls::vx::EventSwitchConfig)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollSpikeCommBucketTriggerConfig)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollSpikeCommBucketDestinationConfig)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollSpikeCommBucketNumPktsSent)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollSpikeCommBucketNumEvtsRcvd)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollSpikeCommBucketCounterReset)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollSpikeCommRouterLookupConfig)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollSpikeCommRouterConfig)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollSpikeCommRouterEventLossDisabled)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollSpikeCommRouterEventLossMisconf)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollSpikeCommRouterEventsRouted)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollSpikeCommRouterCounterReset)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollSpikeCommDecoderNumEventsReceived)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollSpikeCommDecoderCounterReset)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollSpikeCommTimestampDelayConfig)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollSpikeCommTimestampDelayNumEventsReceived)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollSpikeCommTimestampDelayEventLossFull)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollSpikeCommTimestampDelayEventLossExpired)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollSpikeCommTimestampDelayCounterReset)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollBarrierTriggerReached)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollBarrierReleased)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollBarrierConfig)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollInterruptControl)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollInterruptConfig)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollBarrierInterruptInportErrorCount)
CEREAL_REGISTER_TYPE(haldls::vx::ExtollBarrierInterruptInportCounterReset)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::EventSwitchSource)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::EventSwitchReadout)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::EventSwitchConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::ExtollSpikeCommBucketTriggerConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::ExtollSpikeCommBucketDestinationConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::ExtollSpikeCommBucketNumPktsSent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::ExtollSpikeCommBucketNumEvtsRcvd)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::ExtollSpikeCommBucketCounterReset)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::ExtollSpikeCommRouterLookupConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::ExtollSpikeCommRouterConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::ExtollSpikeCommRouterEventLossDisabled)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::ExtollSpikeCommRouterEventLossMisconf)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::ExtollSpikeCommRouterEventsRouted)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::ExtollSpikeCommRouterCounterReset)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::ExtollSpikeCommDecoderNumEventsReceived)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::ExtollSpikeCommDecoderCounterReset)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::ExtollSpikeCommTimestampDelayConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::ExtollSpikeCommTimestampDelayNumEventsReceived)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::ExtollSpikeCommTimestampDelayEventLossFull)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::ExtollSpikeCommTimestampDelayEventLossExpired)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::ExtollSpikeCommTimestampDelayCounterReset)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::ExtollBarrierTriggerReached)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::ExtollBarrierReleased)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::ExtollBarrierConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::ExtollInterruptControl)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::ExtollInterruptConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::ExtollBarrierInterruptInportErrorCount)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::ExtollBarrierInterruptInportCounterReset)

CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::EventSwitchSource)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::EventSwitchReadout)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::EventSwitchConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::ExtollSpikeCommBucketTriggerConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::ExtollSpikeCommBucketDestinationConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::ExtollSpikeCommBucketNumPktsSent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::ExtollSpikeCommBucketNumEvtsRcvd)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::ExtollSpikeCommBucketCounterReset)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::ExtollSpikeCommRouterLookupConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::ExtollSpikeCommRouterConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::ExtollSpikeCommRouterEventLossDisabled)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::ExtollSpikeCommRouterEventLossMisconf)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::ExtollSpikeCommRouterEventsRouted)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::ExtollSpikeCommRouterCounterReset)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::ExtollSpikeCommDecoderNumEventsReceived)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::ExtollSpikeCommDecoderCounterReset)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::ExtollSpikeCommTimestampDelayConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::ExtollSpikeCommTimestampDelayNumEventsReceived)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::ExtollSpikeCommTimestampDelayEventLossFull)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::ExtollSpikeCommTimestampDelayEventLossExpired)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::ExtollSpikeCommTimestampDelayCounterReset)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::ExtollBarrierTriggerReached)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::ExtollBarrierReleased)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::ExtollBarrierConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::ExtollInterruptControl)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::ExtollInterruptConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::ExtollBarrierInterruptInportErrorCount)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::ExtollBarrierInterruptInportCounterReset)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_extoll)
