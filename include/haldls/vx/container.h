#pragma once

#include "haldls/vx/arq.h"
#include "haldls/vx/background.h"
#include "haldls/vx/cadc.h"
#include "haldls/vx/capmem.h"
#include "haldls/vx/correlation.h"
#include "haldls/vx/event.h"
#include "haldls/vx/fpga.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/madc.h"
#include "haldls/vx/neuron.h"
#include "haldls/vx/null_payload_readable.h"
#include "haldls/vx/padi.h"
#include "haldls/vx/perftest.h"
#include "haldls/vx/phy.h"
#include "haldls/vx/pll.h"
#include "haldls/vx/ppu.h"
#include "haldls/vx/readout.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/routing_crossbar.h"
#include "haldls/vx/spi.h"
#include "haldls/vx/sram_controller.h"
#include "haldls/vx/synapse.h"
#include "haldls/vx/synapse_driver.h"
#include "haldls/vx/systime.h"
#include "haldls/vx/timer.h"

#include "haldls/vx/genpybind.h"
#include "hate/visibility.h"

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

template <typename T>
std::string to_json(T const&) GENPYBIND(visible);

template <typename T>
void from_json(T&, std::string const&) GENPYBIND(visible);

// clang-format off
#define PLAYBACK_CONTAINER(Name, Type) \
extern template std::string to_json<Type>(Type const&); \
extern template void from_json(Type&, std::string const&);
// clang-format on
#include "haldls/vx/container.def"

} // namespace vx
} // namespace haldls
