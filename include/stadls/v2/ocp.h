#pragma once

#include "stadls/v2/genpybind.h"

#include "haldls/v2/board.h"

namespace rw_api {
class FlyspiCom;
}

namespace stadls {
namespace v2 GENPYBIND_TAG_STADLS_V2 {

std::vector<haldls::v2::ocp_word_type> ocp_read(
	rw_api::FlyspiCom & com, std::vector<haldls::v2::ocp_address_type> const& addresses);

void ocp_write(
	rw_api::FlyspiCom & com, std::vector<haldls::v2::ocp_word_type> const& words,
	std::vector<haldls::v2::ocp_address_type> const& addresses);

template <class T>
T ocp_read_container(rw_api::FlyspiCom & com, typename T::coordinate_type const& coord);

template <class T>
void ocp_write_container(
	rw_api::FlyspiCom & com, typename T::coordinate_type const& coord, T const& container);

#ifdef __GENPYBIND__
// Explicit declaration of template functions for all valid ocp container types.
#define OCP_CONTAINER(Type)                                                                        \
	extern template void ocp_write_container<Type>(                                                \
		rw_api::FlyspiCom&, Type::coordinate_type const&, Type const&);                            \
	extern template Type ocp_read_container<Type>(rw_api::FlyspiCom&, Type::coordinate_type const&);

	OCP_CONTAINER(haldls::v2::Board)
	OCP_CONTAINER(haldls::v2::FlyspiProgramAddress)
	OCP_CONTAINER(haldls::v2::FlyspiProgramSize)
	OCP_CONTAINER(haldls::v2::FlyspiResultAddress)
	OCP_CONTAINER(haldls::v2::FlyspiResultSize)
	OCP_CONTAINER(haldls::v2::FlyspiState)
	OCP_CONTAINER(haldls::v2::FlyspiControl)
	OCP_CONTAINER(haldls::v2::FlyspiConfig)
	OCP_CONTAINER(haldls::v2::FlyspiException)

#undef OCP_CONTAINER
#endif // __GENPYBIND__

} // namespace v2
} // namespace stadls
