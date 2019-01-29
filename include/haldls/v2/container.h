#include "haldls/v2/genpybind.h"

GENPYBIND_TAG_HALDLS_V2
GENPYBIND_MANUAL({
	parent.attr("__variant__") = "pybind11";
	parent->py::module::import("pyhalco_hicann_dls_v2");
})

#include "board.h"
#include "capmem.h"
#include "chip.h"
#include "common.h"
#include "correlation.h"
#include "dac.h"
#include "playback.h"
#include "neuron.h"
#include "ppu.h"
#include "rate_counter.h"
#include "spike.h"
#include "synapse.h"
#include "synapsedriver.h"
