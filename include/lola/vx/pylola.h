/* This header file contains additional code related to python bindings */
#include <vector>
#include <pybind11/pybind11.h>

#include "lola/vx/genpybind.h"

namespace py = pybind11;

GENPYBIND_TAG_LOLA_VX
GENPYBIND_MANUAL({
	parent.attr("__variant__") = "pybind11";
	parent->py::module::import("pyhalco_hicann_dls_vx");
	parent->py::module::import("pyhaldls_vx");
})

#include "haldls/cerealization.h"
#include "haldls/vx/pickle.h"
#include "lola/vx/cerealization.h"
#include "lola/vx/lola.h"


namespace lola::vx::detail {

#define PLAYBACK_CONTAINER(Name, Type) #Name,
#define LAST_PLAYBACK_CONTAINER(Name, Type) #Name
static std::vector<std::string> const pickle_type_names = {
#include "lola/vx/container.def"
};

#define PLAYBACK_CONTAINER(Name, Type) Type,
#define LAST_PLAYBACK_CONTAINER(Name, Type) Type
typedef hate::type_list<
#include "lola/vx/container.def"
    >
    pickle_types;

py::list get_containers_list(py::module& m);

} // namespace lola::vx::detail

GENPYBIND_MANUAL({
	::haldls::vx::AddPickle<::lola::vx::detail::pickle_types>::apply(
	    parent, ::lola::vx::detail::pickle_type_names);

	parent.attr("containers") = [&parent]() {
		return lola::vx::detail::get_containers_list(parent);
	}();

	::haldls::vx::WrapToFromFunctions<::lola::vx::detail::pickle_types>::apply(parent);
})
