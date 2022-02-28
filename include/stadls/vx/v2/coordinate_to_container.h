#pragma once
#include "haldls/vx/v2/container.h"
#include "hate/type_list.h"
#include "lola/vx/v2/container.h"

namespace stadls::vx::v2 {

/**
 * List of container types used to resolve a container type from a coordinate type.
 */
typedef hate::type_list<
#define PLAYBACK_CONTAINER(Name, Type) Type,
#include "haldls/vx/v2/container.def"
#define LAST_PLAYBACK_CONTAINER(Name, Type) Type
#define PLAYBACK_CONTAINER(Name, Type) LAST_PLAYBACK_CONTAINER(Name, Type),
#include "lola/vx/v2/container.def"
    >
    container_list;

/**
 * List of coordinate types sorted the same way as the container list.
 */
typedef hate::type_list<
#define PLAYBACK_CONTAINER(Name, Type) typename Type::coordinate_type,
#include "haldls/vx/v2/container.def"
#define LAST_PLAYBACK_CONTAINER(Name, Type) typename Type::coordinate_type
#define PLAYBACK_CONTAINER(Name, Type) LAST_PLAYBACK_CONTAINER(Name, Type),
#include "lola/vx/v2/container.def"
    >
    coordinate_list;

/**
 * Given a coordinate type, resolves the corresponding unique container type.
 * @tparam CoordinateT Coordinate type to resolve container type for
 */
template <typename CoordinateT>
struct CoordinateToContainer
{
	typedef typename hate::index_type_list_by_integer<
	    hate::index_type_list_by_type<CoordinateT, coordinate_list>::value,
	    container_list>::type type;
};

} // namespace stadls::vx::v2
