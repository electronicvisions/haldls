#include "stadls/v2/genpybind.h"

GENPYBIND_TAG_STADLS_V2
GENPYBIND_MANUAL({
	parent.attr("__variant__") = "pybind11";
	parent->py::module::import("pyhalco_hicann_dls_v2");
	parent->py::module::import("pyhaldls_v2");
})

#include "experiment.h"
#include "local_board_control.h"
