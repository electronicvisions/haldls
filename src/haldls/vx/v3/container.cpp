#include "haldls/vx/v3/container.h"

#include "haldls/vx/container.h"

namespace haldls::vx::v3 {

#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	static_assert(std::is_base_of_v<::haldls::vx::Container, Type>, #Type);                        \
	static_assert(                                                                                 \
	    std::is_base_of_v<::haldls::vx::Container::Coordinate, Type::coordinate_type>, #Type);
#include "haldls/vx/v3/container.def"

} // namespace haldls::vx::v3
