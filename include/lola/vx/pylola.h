/* This header file contains additional code related to python bindings */
#include <pybind11/pybind11.h>

#include "lola/vx/genpybind.h"

namespace py = pybind11;

GENPYBIND_TAG_LOLA_VX
GENPYBIND_MANUAL({
	parent.attr("__variant__") = "pybind11";
	parent->py::module::import("pyhalco_hicann_dls_vx");
	parent->py::module::import("pyhaldls_vx");
})

#include "lola/vx/lola.h"

namespace lola::vx::detail {
py::list get_containers_list(py::module& m);
} // namespace lola::vx::detail

GENPYBIND_MANUAL({
	parent.attr("containers") = [&parent]() {
		return lola::vx::detail::get_containers_list(parent);
	}();
})
