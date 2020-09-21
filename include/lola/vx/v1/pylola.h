/* This header file contains additional code related to python bindings */
#include <vector>
#include <pybind11/pybind11.h>

#include "haldls/cerealization.tcc"
#include "lola/vx/genpybind.h"

namespace py = pybind11;

GENPYBIND_TAG_LOLA_VX_V1
GENPYBIND_MANUAL({
	parent.attr("__variant__") = "pybind11";
	parent->py::module::import("pyhalco_hicann_dls_vx_v1");
	parent->py::module::import("pyhaldls_vx_v1");
})

#include "haldls/cerealization.h"
#include "haldls/vx/pickle.h"
#include "lola/vx/cerealization.h"
#include "lola/vx/v1/lola.h"


namespace lola::vx::v1::detail {

#define PLAYBACK_CONTAINER(Name, Type) #Name,
#define LAST_PLAYBACK_CONTAINER(Name, Type) #Name
static std::vector<std::string> const pickle_type_names = {
#include "lola/vx/v1/container.def"
};

#define PLAYBACK_CONTAINER(Name, Type) Type,
#define LAST_PLAYBACK_CONTAINER(Name, Type) Type
typedef hate::type_list<
#include "lola/vx/v1/container.def"
    >
    pickle_types;

py::list get_containers_list(py::module& m);

} // namespace lola::vx::v1::detail

GENPYBIND_MANUAL({
	::haldls::vx::AddPickle<::lola::vx::v1::detail::pickle_types>::apply(
	    parent, ::lola::vx::v1::detail::pickle_type_names);

	parent.attr("containers") = [&parent]() {
		return lola::vx::v1::detail::get_containers_list(parent);
	}();

	::haldls::vx::WrapToFromFunctions<::lola::vx::v1::detail::pickle_types>::apply(parent);
})
