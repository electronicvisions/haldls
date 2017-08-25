#pragma once

#include <cstdint>
#include <type_traits>
#include <utility>

namespace haldls {
namespace container {
namespace v2 {

typedef std::uint32_t hardware_address_type;
typedef std::uint32_t hardware_word_type;
typedef std::uint64_t hardware_time_type;

namespace detail {

/// \brief Implementation detail of the visit_preorder() free function (q.v.).
/// \tparam ContainerT Non-const-specified type of the container.
/// \note This class needs to be specialized for non-leaf-node containers, i.e. those that
///       contain other containers.
template <class ContainerT>
struct VisitPreorderImpl {
	static_assert(
		ContainerT::is_leaf_node::value,
		"visit_preorder needs to be specialized for non-leaf-node container");

	/// @tparam ContainerU Type of the container, should be equal to \c ContainerT up to a
	///         const-specifier.
	template <typename ContainerU, typename VisitorT>
	static void call(
		ContainerU& config, typename ContainerT::coordinate_type const& coord, VisitorT&& visitor)
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
		std::is_same<typename ContainerT::coordinate_type, CoordinateT>::value,
		"coordinate type does not match container type");
	detail::VisitPreorderImpl<typename std::remove_cv<ContainerT>::type>::call(
		config, coord, std::forward<VisitorT>(visitor));
}

} // namespace v2
} // namespace container
} // namespace haldls
