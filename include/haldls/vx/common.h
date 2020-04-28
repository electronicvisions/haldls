#pragma once

#include "hate/empty.h"
#include <type_traits>
#include <utility>

namespace haldls {
namespace vx {

namespace detail {

/// \brief Implementation detail of the visit_preorder() free function (q.v.).
/// \tparam ContainerT Non-const-specified type of the container.
/// \note This class needs to be specialized for non-leaf-node containers, i.e. those that
///       contain other containers.
template <class ContainerT>
struct VisitPreorderImpl
{
	static_assert(
	    ContainerT::is_leaf_node::value,
	    "visit_preorder needs to be specialized for non-leaf-node container");

	/// @tparam ContainerU Type of the container, should be equal to \c ContainerT up to a
	///         const-specifier or hate::Empty<ContainerT>.
	/// @tparam CoordinateU Type of the container, should be equal to \c ContainerT up to a
	///         const-specifier or hate::Empty<CoordinateT>.
	template <typename ContainerU, typename CoordinateU, typename VisitorT>
	static void call(ContainerU& config, CoordinateU const& coord, VisitorT&& visitor)
	{
		visitor(coord, config);
	}
}; // VisitPreorderImpl

} // namespace detail

/// \brief Apply the specified visitor to all containers in a hierarchy by doing a
///        pre-order tree traversal.
template <class ContainerT, class CoordinateT, class VisitorT>
void visit_preorder(ContainerT& config, CoordinateT const& coord, VisitorT&& visitor)
{
	static_assert(
	    std::is_same_v<
	        typename hate::remove_empty_t<ContainerT>::coordinate_type,
	        hate::remove_empty_t<CoordinateT>>,
	    "coordinate type does not match container type");
	detail::VisitPreorderImpl<std::remove_cv_t<hate::remove_empty_t<ContainerT>>>::call(
	    config, coord, std::forward<VisitorT>(visitor));
}

} // namespace vx
} // namespace haldls
