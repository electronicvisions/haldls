#pragma once
#include <type_traits>

namespace haldls::vx::detail {

/**
 * Construct a container instance from a given coordinate.
 * This function needs to be specialized for all containers, for which default construction
 * does not suffice to match the given coordinate.
 * @tparam CoordinateT Coordinate type
 * @param coord Coordinate value
 */
template <typename CoordinateT, typename ContainerT>
ContainerT coordinate_to_container(CoordinateT const&)
{
	static_assert(std::is_same_v<typename ContainerT::coordinate_type, CoordinateT>);
	return ContainerT();
}

} // namespace haldls::vx::detail
