#include "lola/vx/v4/pylola.h"

namespace lola::vx::v4::detail {

py::list get_containers_list(py::module& m)
{
	py::list ret;
#define PLAYBACK_CONTAINER(Name, Type) ret.append(m.attr(#Name));
#include "lola/vx/v4/container.def"
	return ret;
}

} // namespace lola::vx::v4::detail
