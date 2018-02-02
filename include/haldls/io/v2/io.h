#include "halco/common/genpybind.h"

GENPYBIND(tag(haldls_io_v2))
GENPYBIND_MANUAL({
	parent.attr("__variant__") = "pybind11";
	parent->py::module::import("pyhalco_hicann_dls_v2");
	parent->py::module::import("pyhaldls_container_v2");
})

#include "experiment.h"
