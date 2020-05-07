/* This header file contains additional code related to python bindings */
#include <pybind11/pybind11.h>

#include "stadls/vx/genpybind.h"

namespace py = pybind11;

GENPYBIND_TAG_STADLS_VX
GENPYBIND_MANUAL({
	parent.attr("__variant__") = "pybind11";
	parent->py::module::import("pyhalco_hicann_dls_vx");
	parent->py::module::import("pyhaldls_vx");
})

#include "stadls/vx/stadls.h"
