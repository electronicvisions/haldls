#pragma once
#include "haldls/vx/extoll.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx::v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using EventSwitchSource GENPYBIND(visible) = haldls::vx::EventSwitchSource;

using ExtollSpikeCommBucketTriggerConfig GENPYBIND(visible) =
    haldls::vx::ExtollSpikeCommBucketTriggerConfig;
using ExtollSpikeCommBucketDestinationConfig GENPYBIND(visible) =
    haldls::vx::ExtollSpikeCommBucketDestinationConfig;
using ExtollSpikeCommBucketNumPktsSent GENPYBIND(visible) =
    haldls::vx::ExtollSpikeCommBucketNumPktsSent;
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

} // namespace haldls::vx::v3
