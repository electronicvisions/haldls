/* This header file contains additional code related to python bindings */
#include <vector>
#include <pybind11/pybind11.h>

#include "haldls/vx/genpybind.h"


namespace py = pybind11;

GENPYBIND_TAG_HALDLS_VX_V1
GENPYBIND_MANUAL({
	parent.attr("__variant__") = "pybind11";
	parent->py::module::import("pyhalco_hicann_dls_vx_v1");
	parent->py::module::import("pyfisch_vx");
})

#include "haldls/cerealization.h"
#include "haldls/vx/pickle.h"
#include "haldls/vx/v1/barrier.h"
#include "haldls/vx/v1/haldls.h"
#include "hate/type_list.h"

namespace haldls::vx::v1::detail {

#define PLAYBACK_CONTAINER(Name, Type) #Name,
static std::vector<std::string> const pickle_type_names = {
#include "haldls/vx/v1/container.def"
    "Barrier"};

#define PLAYBACK_CONTAINER(Name, Type) Type,
typedef hate::type_list<
#include "haldls/vx/v1/container.def"
    haldls::vx::Barrier>
    pickle_types;

py::list get_containers_list(py::module& m);

} // haldls::vx::v1::detail

GENPYBIND_MANUAL({
	::haldls::vx::AddPickle<::haldls::vx::v1::detail::pickle_types>::apply(
	    parent, ::haldls::vx::v1::detail::pickle_type_names);

	parent.attr("containers") = [&parent]() {
		return haldls::vx::v1::detail::get_containers_list(parent);
	}();

	::haldls::vx::WrapToFromFunctions<::haldls::vx::v1::detail::pickle_types>::apply(parent);
})
