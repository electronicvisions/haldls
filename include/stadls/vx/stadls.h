#include "stadls/vx/genpybind.h"

GENPYBIND_TAG_STADLS_VX
GENPYBIND_MANUAL({
	parent.attr("__variant__") = "pybind11";
	parent->py::module::import("pyhalco_hicann_dls_vx");
	parent->py::module::import("pyhaldls_vx");
})

#include "stadls/vx/init_generator.h"
#include "stadls/vx/playback_generator.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.h"
#include "stadls/vx/playback_program_executor.h"
