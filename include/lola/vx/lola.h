#pragma once
#include "lola/vx/genpybind.h"

GENPYBIND_TAG_LOLA_VX
GENPYBIND_MANUAL({
	parent.attr("__variant__") = "pybind11";
	parent->py::module::import("pyhalco_hicann_dls_vx");
	parent->py::module::import("pyhaldls_vx");
})

#include "lola/vx/container.h"
