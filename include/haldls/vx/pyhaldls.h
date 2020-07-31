/* This header file contains additional code related to python bindings */
#include <vector>
#include <pybind11/pybind11.h>

#include "haldls/vx/genpybind.h"


namespace py = pybind11;

GENPYBIND_TAG_HALDLS_VX
GENPYBIND_MANUAL({
	parent.attr("__variant__") = "pybind11";
	parent->py::module::import("pyhalco_hicann_dls_vx");
	parent->py::module::import("pyfisch_vx");
})

#include "haldls/vx/barrier.h"
#include "haldls/vx/haldls.h"
#include "haldls/vx/pickle.h"
#include "hate/type_list.h"

namespace haldls::vx::detail {

#define PLAYBACK_CONTAINER(Name, Type) #Name,
static std::vector<std::string> const pickle_type_names = {
#include "haldls/vx/container.def"
    "Barrier"};

#define PLAYBACK_CONTAINER(Name, Type) Type,
typedef hate::type_list<
#include "haldls/vx/container.def"
    haldls::vx::Barrier>
    pickle_types;

py::list get_containers_list(py::module& m);

} // haldls::vx::detail

GENPYBIND_MANUAL({
	::haldls::vx::AddPickle<::haldls::vx::detail::pickle_types>::apply(
	    parent, ::haldls::vx::detail::pickle_type_names);

	parent.attr("containers") = [&parent]() {
		return haldls::vx::detail::get_containers_list(parent);
	}();
})
