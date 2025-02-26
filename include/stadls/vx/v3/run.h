#pragma once
#include "stadls/vx/genpybind.h"
#include "stadls/vx/run.h"

namespace stadls::vx {
namespace v3 GENPYBIND_TAG_STADLS_VX_V3 {

template <typename... Ts>
auto run(Ts&&... args) -> decltype(stadls::vx::run(std::forward<Ts>(args)...))
{
	return stadls::vx::run(std::forward<Ts>(args)...);
}

GENPYBIND_MANUAL({
	[[maybe_unused]] ::stadls::vx::detail::RunUnrollPyBind11Helper<
	    std::remove_cvref_t<::pyhxcomm::vx::ConnectionHandle>>
	    helper(parent);
})

} // namespace v3
} // namespace stadls::vx
