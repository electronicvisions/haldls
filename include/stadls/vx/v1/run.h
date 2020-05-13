#pragma once
#include "stadls/vx/genpybind.h"
#include "stadls/vx/run.h"

namespace stadls::vx::v1 GENPYBIND_TAG_STADLS_VX_V1 {

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

} // namespace stadls::vx::v1
