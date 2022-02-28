#pragma once
#include "stadls/vx/genpybind.h"
#include "stadls/vx/reinit_stack_entry.h"

namespace stadls::vx::v3 GENPYBIND_TAG_STADLS_VX_V3 {

typedef stadls::vx::ReinitStackEntry ReinitStackEntry;

GENPYBIND_MANUAL({
	using namespace ::stadls::vx;

	pybind11::class_<ReinitStackEntry> wrapped(parent, "ReinitStackEntry");

	[[maybe_unused]] ::pyhxcomm::vx::reinit_stack_entry_unroll_helper<decltype(wrapped)> helper{
	    wrapped};

	wrapped.def("pop", &ReinitStackEntry::pop);
	wrapped.def(
	    "set", &ReinitStackEntry::set, pybind11::arg("pbmem"), pybind11::arg("enforce") = true);
})

} // namespace stadls::vx::v3
