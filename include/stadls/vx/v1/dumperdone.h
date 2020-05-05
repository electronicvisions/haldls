#pragma once

#include "halco/hicann-dls/vx/barrier.h"
#include "haldls/cerealization.h"
#include "haldls/vx/barrier.h"
#include "haldls/vx/timer.h"
#include "haldls/vx/v1/container.h"
#include "hate/visibility.h"
#include "lola/vx/v1/container.h"
#include "stadls/vx/genpybind.h"
#include <utility>
#include <variant>
#include <vector>

#if defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)
#include "haldls/vx/pickle.h"
#include <pybind11/pybind11.h>
namespace py = pybind11;
#endif

namespace stadls::vx GENPYBIND_TAG_STADLS_VX_V1 {

namespace v1 GENPYBIND_TAG_STADLS_VX_V1 {

struct GENPYBIND(visible) DumperDone
{
	/**
	 * Variant over all coordinate/container pairs
	 */
	typedef std::variant<
#define PLAYBACK_CONTAINER(Name, Type) std::pair<typename Type::coordinate_type, Type>,
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v1/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v1/container.def"
	    std::pair<typename haldls::vx::Timer::coordinate_type, haldls::vx::Timer::Value>,
	    std::pair<halco::hicann_dls::vx::BarrierOnFPGA, haldls::vx::Barrier>,
	    std::
	        pair<halco::hicann_dls::vx::PollingOmnibusBlockOnFPGA, haldls::vx::PollingOmnibusBlock>>
	    coco_type;
	typedef std::vector<coco_type> values_type;

	DumperDone() = default;

	values_type values{};

	bool operator==(DumperDone const& other) const SYMBOL_VISIBLE;
	bool operator!=(DumperDone const& other) const SYMBOL_VISIBLE;

	// clang-format off
	GENPYBIND_MANUAL({
		parent.def("__len__", [](GENPYBIND_PARENT_TYPE const& v) { return v.values.size(); })
			.def("tolist", [](GENPYBIND_PARENT_TYPE const& v) {
				py::list ret;
				for (auto const& vv : v.values) {
					py::object item;
					std::visit(
						[&item](auto const& vvv) {
							item = py::make_tuple(std::get<0>(vvv), std::get<1>(vvv));
						},
						vv);
					ret.append(item);
				}
				return ret;
				});
	})
	// clang-format on


private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t);
};

} // namespace v1

GENPYBIND_MANUAL({
	haldls::vx::AddPickle<hate::type_list<stadls::vx::v1::DumperDone>>::apply(
	    parent, {"DumperDone"});

	::haldls::vx::WrapToFromFunctions<stadls::vx::v1::DumperDone>::apply(parent);
})

} // namespace stadls::vx

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::v1::DumperDone)
