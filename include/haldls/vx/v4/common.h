#pragma once
#include "haldls/vx/common.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx::v4 GENPYBIND_TAG_HALDLS_VX_V4 {

template <typename... Ts>
auto visit_preorder(Ts&&... args) -> decltype(haldls::vx::visit_preorder(std::forward<Ts>(args)...))
{
	return haldls::vx::visit_preorder(std::forward<Ts>(args)...);
}

} // namespace haldls::vx::v4
