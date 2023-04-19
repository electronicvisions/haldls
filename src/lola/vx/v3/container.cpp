#include "lola/vx/v3/container.h"

#include "haldls/vx/container.h"

namespace lola::vx::v3 {

#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	static_assert(std::is_base_of_v<::haldls::vx::Container, Type>, #Type);                        \
	static_assert(                                                                                 \
	    std::is_base_of_v<::haldls::vx::Container::Coordinate, Type::coordinate_type>, #Type);
#include "lola/vx/v3/container.def"

} // namespace lola::vx::v3
