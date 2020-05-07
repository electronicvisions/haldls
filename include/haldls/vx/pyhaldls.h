/* This header file contains additional code related to python bindings */
#include <pybind11/pybind11.h>

#include "haldls/vx/genpybind.h"

namespace py = pybind11;

GENPYBIND_TAG_HALDLS_VX
GENPYBIND_MANUAL({
	parent.attr("__variant__") = "pybind11";
	parent->py::module::import("pyhalco_hicann_dls_vx");
	parent->py::module::import("pyfisch_vx");
})

#include "haldls/vx/haldls.h"

namespace haldls::vx::detail {
py::list get_containers_list(py::module& m);
} // namespace haldls::vx::detail

GENPYBIND_MANUAL({
	parent.attr("containers") = [&parent]() {
		return haldls::vx::detail::get_containers_list(parent);
	}();
})
