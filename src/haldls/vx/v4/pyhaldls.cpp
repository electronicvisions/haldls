#include "haldls/vx/v4/pyhaldls.h"

namespace haldls::vx::v4::detail {

py::list get_containers_list(py::module& m)
{
	py::list ret;
#define PLAYBACK_CONTAINER(Name, Type) ret.append(m.attr(#Name));
#include "haldls/vx/v4/container.def"
	return ret;
}

} // namespace haldls::vx::v4::detail
