#include "stadls/vx/genpybind.h"

GENPYBIND_TAG_STADLS_VX
GENPYBIND_MANUAL({
	parent.attr("__variant__") = "pybind11";
	parent->py::module::import("pyhalco_hicann_dls_vx");
	parent->py::module::import("pyhaldls_vx");
})

#include "stadls/vx/playback_executor.h"
