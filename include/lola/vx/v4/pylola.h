/* This header file contains additional code related to python bindings */
#include "lola/vx/genpybind.h"
#include <vector>
#include <pybind11/pybind11.h>

namespace py = pybind11;

GENPYBIND_TAG_LOLA_VX_V4
GENPYBIND_MANUAL({
	parent.attr("__variant__") = "pybind11";
	parent->py::module::import("pyhalco_hicann_dls_vx_v4");
	parent->py::module::import("pyhaldls_vx_v4");
})

#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/lola/vx/v4/lola.h"
#include "haldls/vx/pickle.h"
#include "lola/vx/v4/lola.h"
#include <cereal/macros.hpp>


namespace lola::vx::v4::detail {

#define PLAYBACK_CONTAINER(Name, Type) #Name,
#define LAST_PLAYBACK_CONTAINER(Name, Type) #Name
static std::vector<std::string> const pickle_type_names = {
#include "lola/vx/v4/container.def"
};

#define PLAYBACK_CONTAINER(Name, Type) Type,
#define LAST_PLAYBACK_CONTAINER(Name, Type) Type
typedef hate::type_list<
#include "lola/vx/v4/container.def"
    >
    pickle_types;

py::list get_containers_list(py::module& m);

} // namespace lola::vx::v4::detail

GENPYBIND_MANUAL({
	::haldls::vx::AddPickle<::lola::vx::v4::detail::pickle_types>::apply(
	    parent, ::lola::vx::v4::detail::pickle_type_names);

	parent.attr("containers") = [&parent]() {
		return lola::vx::v4::detail::get_containers_list(parent);
	}();

	::haldls::vx::WrapToFromFunctions<::lola::vx::v4::detail::pickle_types>::apply(parent);
})
