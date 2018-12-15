#include "lola/v2/genpybind.h"

GENPYBIND_TAG_LOLA_V2
GENPYBIND_MANUAL({
	parent.attr("__variant__") = "pybind11";
	parent->py::module::import("pyhalco_hicann_dls_v2");
	parent->py::module::import("pyhaldls_v2");
})

#include "lola/v2/ppu.h"
