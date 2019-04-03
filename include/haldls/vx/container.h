#include "haldls/vx/genpybind.h"

GENPYBIND_TAG_HALDLS_VX
GENPYBIND_MANUAL({
	parent.attr("__variant__") = "pybind11";
	parent->py::module::import("pyhalco_hicann_dls_vx");
	parent->py::module::import("pyfisch_vx");
})

#include "haldls/vx/jtag.h"
#include "haldls/vx/playback.h"
#include "haldls/vx/ppu.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/spi.h"
#include "haldls/vx/timer.h"
