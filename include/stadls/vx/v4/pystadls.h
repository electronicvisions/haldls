/* This header file contains additional code related to python bindings */
#include <pybind11/pybind11.h>

#include "cereal/types/stadls/vx/v4/stadls.h"
#include "stadls/vx/genpybind.h"

namespace py = pybind11;

GENPYBIND_TAG_STADLS_VX_V4
GENPYBIND_MANUAL({
	parent.attr("__variant__") = "pybind11";
	parent->py::module::import("pyhalco_hicann_dls_vx_v4");
	parent->py::module::import("pyhaldls_vx_v4");
	parent->py::module::import("pylola_vx_v4");
})

#include "stadls/vx/v4/stadls.h"
