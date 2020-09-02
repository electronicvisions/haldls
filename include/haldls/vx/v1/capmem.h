#pragma once
#include "halco/hicann-dls/vx/v1/coordinates.h"
#include "haldls/vx/capmem.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx {
CAPMEM_EXTERN_TEMPLATE(halco::hicann_dls::vx::v1::Coordinates)
} // namespace haldls::vx

namespace std {
CAPMEM_HALCO_GEOMETRY_HASH(halco::hicann_dls::vx::v1::Coordinates)
} // namespace std

namespace haldls::vx::v1 GENPYBIND_TAG_HALDLS_VX_V1 {

using CapMemCell GENPYBIND(opaque) = haldls::vx::CapMemCell<halco::hicann_dls::vx::v1::Coordinates>;

GENPYBIND_MANUAL({
	auto cls = pybind11::class_<::haldls::vx::v1::CapMemCell::value_type>(
	    parent, "_CapMemCell_value_type");
	cls.def(
	       pybind11::init<::haldls::vx::v1::CapMemCell::Value>(),
	       pybind11::arg("value") = ::haldls::vx::v1::CapMemCell::Value(0))
	    .def(
	        pybind11::init<::haldls::vx::v1::CapMemCell::DisableRefresh>(), pybind11::arg("value"));
	parent.attr("CapMemCell").attr("value_type") = parent.attr("_CapMemCell_value_type");
})

using CapMemBlock GENPYBIND(opaque) =
    haldls::vx::CapMemBlock<halco::hicann_dls::vx::v1::Coordinates>;
using CapMemBlockConfig GENPYBIND(opaque) =
    haldls::vx::CapMemBlockConfig<halco::hicann_dls::vx::v1::Coordinates>;
using ReferenceGeneratorConfig GENPYBIND(visible) = haldls::vx::ReferenceGeneratorConfig;

} // namespace haldls::vx::v1
