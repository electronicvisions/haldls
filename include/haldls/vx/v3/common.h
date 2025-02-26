#pragma once
#include "haldls/vx/common.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx { namespace v3 GENPYBIND_TAG_HALDLS_VX_V3 {

template <typename... Ts>
auto visit_preorder(Ts&&... args) -> decltype(haldls::vx::visit_preorder(std::forward<Ts>(args)...))
{
	return haldls::vx::visit_preorder(std::forward<Ts>(args)...);
}

} // namespace v3
} // namespace haldls::vx
