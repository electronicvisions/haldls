#pragma once
#include "stadls/vx/dumper.h"
#include "stadls/vx/genpybind.h"

#include "haldls/vx/v2/container.h"
#include "lola/vx/v2/container.h"

#if defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)
// #include "haldls/cerealization.h"
#include "haldls/vx/pickle.h"
namespace py = pybind11;
#endif

#ifdef __GENPYBIND_GENERATED__
// Needed for manual wrapping (pickling) of Dumper::done_type
#include <cereal/types/utility.hpp>
#include <cereal/types/variant.hpp>
#include <cereal/types/vector.hpp>
#include "halco/common/cerealization_geometry.h"
#include "lola/vx/cerealization.h"
#endif

namespace stadls::vx GENPYBIND_TAG_STADLS_VX_V2 {

namespace v2 GENPYBIND_TAG_STADLS_VX_V2 {

struct GENPYBIND(visible) DumperDone
{
	/**
	 * Variant over all coordinate/container pairs
	 */
	typedef std::variant<
#define PLAYBACK_CONTAINER(Name, Type) std::pair<typename Type::coordinate_type, Type>,
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v2/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v2/container.def"
	    std::pair<typename haldls::vx::Timer::coordinate_type, haldls::vx::Timer::Value>,
	    std::pair<halco::hicann_dls::vx::BarrierOnFPGA, haldls::vx::Barrier>>
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

// Dumper is a detail, its template parameter DumperDone isn't.
// The latter is used by the PlaybackProgramBuilderDumper API.
// Hence, we don't wrap the detail, but only the public API type.
using Dumper GENPYBIND(hidden) = stadls::vx::detail::Dumper<DumperDone>;

GENPYBIND_MANUAL({
	haldls::vx::AddPickle<hate::type_list<stadls::vx::v2::DumperDone>>::apply(
	    parent, {"DumperDone"});

	::haldls::vx::WrapToFromFunctions<stadls::vx::v2::DumperDone>::apply(parent);
})

} // namespace v2

extern template class SYMBOL_VISIBLE stadls::vx::detail::Dumper<stadls::vx::v2::DumperDone>;

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	extern template SYMBOL_VISIBLE void stadls::vx::v2::Dumper::write<Type>(                       \
	    typename Type::coordinate_type const&, Type const&);                                       \
	extern template SYMBOL_VISIBLE stadls::vx::PlaybackProgram::ContainerTicket<Type>              \
	stadls::vx::v2::Dumper::read<Type>(typename Type::coordinate_type const&);
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v2/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v2/container.def"

} // namespace stadls::vx

PYBIND11_MAKE_OPAQUE(stadls::vx::v2::Dumper::done_type)
