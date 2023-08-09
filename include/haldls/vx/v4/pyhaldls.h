/* This header file contains additional code related to python bindings */
#include <vector>
#include <pybind11/pybind11.h>

#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/haldls/vx/v4/haldls.h"
#include "haldls/vx/genpybind.h"


namespace py = pybind11;

GENPYBIND_TAG_HALDLS_VX_V4
GENPYBIND_MANUAL({
	parent.attr("__variant__") = "pybind11";
	parent->py::module::import("pyhalco_hicann_dls_vx_v4");
	parent->py::module::import("pyfisch_vx_v4");
})

#include "haldls/vx/pickle.h"
#include "haldls/vx/v4/barrier.h"
#include "haldls/vx/v4/haldls.h"
#include "hate/type_list.h"
#include <cereal/macros.hpp>

namespace haldls::vx::v4::detail {

#define PLAYBACK_CONTAINER(Name, Type) #Name,
static std::vector<std::string> const pickle_type_names = {
#include "haldls/vx/v4/container.def"
    "Barrier"};

#define PLAYBACK_CONTAINER(Name, Type) Type,
typedef hate::type_list<
#include "haldls/vx/v4/container.def"
    haldls::vx::Barrier>
    pickle_types;

py::list get_containers_list(py::module& m);

} // haldls::vx::v4::detail

GENPYBIND_MANUAL({
	::haldls::vx::AddPickle<::haldls::vx::v4::detail::pickle_types>::apply(
	    parent, ::haldls::vx::v4::detail::pickle_type_names);

	parent.attr("containers") = [&parent]() {
		return haldls::vx::v4::detail::get_containers_list(parent);
	}();

	::haldls::vx::WrapToFromFunctions<::haldls::vx::v4::detail::pickle_types>::apply(parent);
})
