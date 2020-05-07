#include "lola/vx/pylola.h"

namespace lola::vx::detail {

py::list get_containers_list(py::module& m)
{
	py::list ret;
#define PLAYBACK_CONTAINER(Name, Type) ret.append(m.attr(#Name));
#include "lola/vx/container.def"
	return ret;
}

} // namespace lola::vx::detail
