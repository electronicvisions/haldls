#pragma once
#include "stadls/vx/genpybind.h"
#include "stadls/vx/reinit_stack_entry.h"

namespace stadls::vx {
namespace v3 GENPYBIND_TAG_STADLS_VX_V3 {

typedef stadls::vx::ReinitStackEntry ReinitStackEntry;

GENPYBIND_MANUAL({
	using namespace ::stadls::vx;

	pybind11::class_<ReinitStackEntry> wrapped(parent, "ReinitStackEntry");

	[[maybe_unused]] ::pyhxcomm::vx::reinit_stack_entry_unroll_helper<decltype(wrapped)> helper{
	    wrapped};

	wrapped.def("pop", &ReinitStackEntry::pop);
	wrapped.def(
	    "set",
	    [](ReinitStackEntry& self, PlaybackProgram const& pbmem_request,
	       std::optional<PlaybackProgram> const& pbmem_snapshot,
	       bool enforce) { return self.set(pbmem_request, pbmem_snapshot, enforce); },
	    pybind11::arg("pbmem_request"), pybind11::arg("pbmem_snapshot") = std::nullopt,
	    pybind11::arg("enforce") = true);
})

} // namespace v3
} // namespace stadls::vx
