#pragma once

#include "haldls/vx/encodable.h"
#include "hate/visibility.h"
#include "lola/vx/v4/chip.h"
#include "stadls/vx/genpybind.h"
#include <memory>
#include <vector>
#include <cereal/macros.hpp>

#if defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)
#include "haldls/vx/pickle.h"
#include <pybind11/pybind11.h>
namespace py = pybind11;
#endif

namespace stadls::vx::v4 {

struct DumperDone;

} // namespace stadls::vx::v4

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, stadls::vx::v4::DumperDone& value, std::uint32_t const version);

} // namespace cereal

namespace stadls {
namespace vx GENPYBIND_TAG_STADLS_VX_V4 {

namespace v4 GENPYBIND_TAG_STADLS_VX_V4 {

struct GENPYBIND(visible) DumperDone
{
	/**
	 * Variant over all coordinate/container pairs
	 */
	typedef std::pair<
	    std::unique_ptr<haldls::vx::Encodable::Coordinate>,
	    std::unique_ptr<haldls::vx::Encodable>>
	    coco_type;
	typedef std::vector<coco_type> values_type;

	DumperDone() = default;

	DumperDone(DumperDone const&) = delete;
	DumperDone(DumperDone&&) = default;
	DumperDone& operator=(DumperDone const&) = delete;
	DumperDone& operator=(DumperDone&&) = default;

	values_type values GENPYBIND(hidden);

	GENPYBIND_MANUAL({
		auto const get_values = [parent](GENPYBIND_PARENT_TYPE const& self) {
			pybind11::list ret;
			for (auto const& value : self.values) {
				if (!value.first || !value.second) {
					throw std::logic_error("Unexpected access to moved-from object.");
				}
				ret.append(py::make_tuple(value.first->clone(), value.second->clone_encodable()));
			}
			return ret;
		};
		auto const set_values = [parent](
		                            GENPYBIND_PARENT_TYPE& self, pybind11::list const& new_values) {
			stadls::vx::v4::DumperDone::values_type tmp_values;
			for (auto const& value : new_values) {
				if (!py::isinstance<py::tuple>(value)) {
					throw std::runtime_error("Expected tuple.");
				}
				auto const* const coord =
				    py::cast<halco::common::Coordinate const*>(py::cast<py::tuple>(value)[0]);
				auto const* const config =
				    py::cast<haldls::vx::Encodable const*>(py::cast<py::tuple>(value)[1]);
				if (coord == nullptr || config == nullptr) {
					throw py::cast_error("Casted to nullptr.");
				}
				tmp_values.push_back(std::make_pair(coord->clone(), config->clone_encodable()));
			}
			self.values = std::move(tmp_values);
		};
		parent.def_property("values", get_values, set_values);
	})

	bool operator==(DumperDone const& other) const SYMBOL_VISIBLE;
	bool operator!=(DumperDone const& other) const SYMBOL_VISIBLE;

	// clang-format off
	GENPYBIND_MANUAL({
		parent.def("__len__", [](GENPYBIND_PARENT_TYPE const& v) { return v.values.size(); })
			.def("tolist", [](GENPYBIND_PARENT_TYPE const& v) {
				py::list ret;
				for (auto const& vv : v.values) {
					py::object item = py::make_tuple(
							    std::get<0>(vv)->clone(), std::get<1>(vv)->clone_encodable());
					ret.append(item);
				}
				return ret;
				});
	})
	// clang-format on

	/**
	 * Remove entries which contain block_until commands.
	 */
	void remove_block_until() SYMBOL_VISIBLE;

	/**
	 * For each coordinate value only retain the latest command.
	 * Order between different container types is not preserved.
	 */
	void squash() SYMBOL_VISIBLE;

private:
	template <typename Archive>
	friend void ::cereal::serialize(Archive& ar, DumperDone& value, std::uint32_t);
};


/**
 * Convert the dumper result to a Chip config.
 * This conversion is not bijective.
 * @param dumperdone Dumper result
 */
lola::vx::v4::Chip GENPYBIND(visible) convert_to_chip(DumperDone const& dumperdone) SYMBOL_VISIBLE;

/**
 * Convert the dumper result to a Chip config.
 * This conversion is not bijective.
 * @param dumperdone Dumper result
 * @param previous Optional previous ChipConfig to apply changes to
 */
lola::vx::v4::Chip GENPYBIND(visible) convert_to_chip(
    DumperDone const& dumperdone, lola::vx::v4::Chip const& previous) SYMBOL_VISIBLE;

} // namespace v4

GENPYBIND_MANUAL({
	haldls::vx::AddPickle<hate::type_list<stadls::vx::v4::DumperDone>>::apply(
	    parent, {"DumperDone"});

	::haldls::vx::WrapToFromFunctions<stadls::vx::v4::DumperDone>::apply(parent);
})

} // namespace vx
} // namespace stadls
