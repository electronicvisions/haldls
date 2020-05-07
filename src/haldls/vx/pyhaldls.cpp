#include "haldls/vx/pyhaldls.h"

namespace haldls::vx::detail {

py::list get_containers_list(py::module& m)
{
	py::list ret;
#define PLAYBACK_CONTAINER(Name, Type) ret.append(m.attr(#Name));
#include "haldls/vx/container.def"
	return ret;
}

} // namespace haldls::vx::detail
