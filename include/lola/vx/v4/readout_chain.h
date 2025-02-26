#pragma once
#define CHIP_REVISION 4
#include "lola/vx/readout_chain.h"
#undef CHIP_REVISION

namespace lola::vx {
namespace v4 GENPYBIND_TAG_LOLA_VX_V4 {

GENPYBIND(postamble)
GENPYBIND_MANUAL({
	using namespace lola::vx::v4;
	{
		auto attr = parent.attr("ReadoutChain").attr("PadMux");
		auto const to_str = [](ReadoutChain::PadMux const& self) {
			std::stringstream ss;
			ss << self;
			return ss.str();
		};
		auto ism = parent->py::is_method(attr);
		attr.attr("__str__") = parent->py::cpp_function(to_str, ism);
	}
	{
		auto attr = parent.attr("ReadoutChain").attr("InputMux");
		auto const to_str = [](ReadoutChain::InputMux const& self) {
			std::stringstream ss;
			ss << self;
			return ss.str();
		};
		auto ism = parent->py::is_method(attr);
		attr.attr("__str__") = parent->py::cpp_function(to_str, ism);
	}
	{
		auto attr = parent.attr("ReadoutChain").attr("BufferToPad");
		auto const to_str = [](ReadoutChain::BufferToPad const& self) {
			std::stringstream ss;
			ss << self;
			return ss.str();
		};
		auto ism = parent->py::is_method(attr);
		attr.attr("__str__") = parent->py::cpp_function(to_str, ism);
	}
})

} // v4
} // lola::vx
