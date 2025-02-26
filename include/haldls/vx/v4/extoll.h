#pragma once
#include "haldls/vx/extoll.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx { namespace v4 GENPYBIND_TAG_HALDLS_VX_V4 {

using EventSwitchSource GENPYBIND(visible) = haldls::vx::EventSwitchSource;
using EventSwitchReadout GENPYBIND(visible) = haldls::vx::EventSwitchReadout;
using EventSwitchConfig GENPYBIND(visible) = haldls::vx::EventSwitchConfig;

using ExtollSpikeCommBucketTriggerConfig GENPYBIND(visible) =
    haldls::vx::ExtollSpikeCommBucketTriggerConfig;
using ExtollSpikeCommBucketDestinationConfig GENPYBIND(visible) =
    haldls::vx::ExtollSpikeCommBucketDestinationConfig;
using ExtollSpikeCommBucketNumPktsSent GENPYBIND(visible) =
    haldls::vx::ExtollSpikeCommBucketNumPktsSent;
using ExtollSpikeCommBucketNumEvtsRcvd GENPYBIND(visible) =
    haldls::vx::ExtollSpikeCommBucketNumEvtsRcvd;
using ExtollSpikeCommBucketCounterReset GENPYBIND(visible) =
    haldls::vx::ExtollSpikeCommBucketCounterReset;
using ExtollSpikeCommRouterLookupConfig GENPYBIND(visible) =
    haldls::vx::ExtollSpikeCommRouterLookupConfig;
using ExtollSpikeCommRouterConfig GENPYBIND(visible) = haldls::vx::ExtollSpikeCommRouterConfig;
using ExtollSpikeCommRouterEventLossDisabled GENPYBIND(visible) =
    haldls::vx::ExtollSpikeCommRouterEventLossDisabled;
using ExtollSpikeCommRouterEventLossMisconf GENPYBIND(visible) =
    haldls::vx::ExtollSpikeCommRouterEventLossMisconf;
using ExtollSpikeCommRouterEventsRouted GENPYBIND(visible) =
    haldls::vx::ExtollSpikeCommRouterEventsRouted;
using ExtollSpikeCommRouterCounterReset GENPYBIND(visible) =
    haldls::vx::ExtollSpikeCommRouterCounterReset;
using ExtollSpikeCommDecoderNumEventsReceived GENPYBIND(visible) =
    haldls::vx::ExtollSpikeCommDecoderNumEventsReceived;
using ExtollSpikeCommDecoderCounterReset GENPYBIND(visible) =
    haldls::vx::ExtollSpikeCommDecoderCounterReset;
using ExtollSpikeCommTimestampDelayConfig GENPYBIND(visible) =
    haldls::vx::ExtollSpikeCommTimestampDelayConfig;
using ExtollSpikeCommTimestampDelayNumEventsReceived GENPYBIND(visible) =
    haldls::vx::ExtollSpikeCommTimestampDelayNumEventsReceived;
using ExtollSpikeCommTimestampDelayEventLossFull GENPYBIND(visible) =
    haldls::vx::ExtollSpikeCommTimestampDelayEventLossFull;
using ExtollSpikeCommTimestampDelayEventLossExpired GENPYBIND(visible) =
    haldls::vx::ExtollSpikeCommTimestampDelayEventLossExpired;
using ExtollSpikeCommTimestampDelayCounterReset GENPYBIND(visible) =
    haldls::vx::ExtollSpikeCommTimestampDelayCounterReset;


using ExtollBarrierTriggerReached GENPYBIND(visible) = haldls::vx::ExtollBarrierTriggerReached;
using ExtollBarrierReleased GENPYBIND(visible) = haldls::vx::ExtollBarrierReleased;
using ExtollBarrierConfig GENPYBIND(visible) = haldls::vx::ExtollBarrierConfig;
using ExtollInterruptControl GENPYBIND(visible) = haldls::vx::ExtollInterruptControl;
using ExtollInterruptConfig GENPYBIND(visible) = haldls::vx::ExtollInterruptConfig;

using ExtollBarrierInterruptInportErrorCount GENPYBIND(visible) =
    haldls::vx::ExtollBarrierInterruptInportErrorCount;
using ExtollBarrierInterruptInportCounterReset GENPYBIND(visible) =
    haldls::vx::ExtollBarrierInterruptInportCounterReset;

} // namespace v4
} // namespace haldls::vx
