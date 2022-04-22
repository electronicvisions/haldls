/* This header file contains additional code related to python bindings */
#include <pybind11/pybind11.h>

#include "stadls/vx/genpybind.h"

namespace py = pybind11;

GENPYBIND_TAG_STADLS_VX_V2
GENPYBIND_MANUAL({
	parent.attr("__variant__") = "pybind11";
	parent->py::module::import("pyhalco_hicann_dls_vx_v2");
	parent->py::module::import("pyhaldls_vx_v2");
	parent->py::module::import("pylola_vx_v2");
})

#include "stadls/vx/v2/stadls.h"
