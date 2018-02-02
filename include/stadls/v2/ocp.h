#pragma once

#include "halco/common/genpybind.h"

#include "haldls/container/v2/board.h"

namespace rw_api {
class FlyspiCom;
}

namespace stadls {
namespace v2 GENPYBIND(tag(stadls_v2))
{
	template <class T>
	void ocp_write(
		rw_api::FlyspiCom & com, typename T::coordinate_type const& coord, T const& container);
	template <class T>
	T ocp_read(rw_api::FlyspiCom & com, typename T::coordinate_type const& coord);

#ifdef __GENPYBIND__
// Explicit declaration of template functions for all valid ocp container types.
#define OCP_CONTAINER(Type)                                                                        \
	extern template void ocp_write<Type>(                                                          \
		rw_api::FlyspiCom&, Type::coordinate_type const&, Type const&);                            \
	extern template Type ocp_read<Type>(rw_api::FlyspiCom&, Type::coordinate_type const&);

	OCP_CONTAINER(haldls::container::v2::Board)
	OCP_CONTAINER(haldls::container::v2::FlyspiProgramAddress)
	OCP_CONTAINER(haldls::container::v2::FlyspiProgramSize)
	OCP_CONTAINER(haldls::container::v2::FlyspiResultAddress)
	OCP_CONTAINER(haldls::container::v2::FlyspiResultSize)
	OCP_CONTAINER(haldls::container::v2::FlyspiState)
	OCP_CONTAINER(haldls::container::v2::FlyspiControl)
	OCP_CONTAINER(haldls::container::v2::FlyspiConfig)
	OCP_CONTAINER(haldls::container::v2::FlyspiException)

#undef OCP_CONTAINER
#endif // __GENPYBIND__

} // namespace v2
} // namespace stadls
