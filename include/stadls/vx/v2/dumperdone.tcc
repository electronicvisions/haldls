#include "stadls/vx/v2/dumperdone.h"

#include "halco/common/cerealization_geometry.h"
#include "haldls/cerealization.tcc"
#include "hate/type_index.h"
#include <map>
#include <stdexcept>
#include <cereal/types/string.hpp>
#include <cereal/types/utility.hpp>

namespace stadls::vx::v2 {

template <typename Archive>
void DumperDone::serialize(Archive& ar, std::uint32_t const)
{
	static const std::map<std::string, void (*)(Archive&, coco_type&)> lookup = {
#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	std::make_pair(hate::full_name<Type>(), [](Archive& ar, coco_type& coco) {                     \
		auto& value = std::get<std::pair<typename Type::coordinate_type, Type>>(coco);             \
		ar(CEREAL_NVP(value));                                                                     \
	}),
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v2/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v2/container.def"
	    {"haldls::vx::Timer::Value",
	     [](Archive& ar, coco_type& coco) {
		     auto& value = std::get<
		         std::pair<typename haldls::vx::Timer::coordinate_type, haldls::vx::Timer::Value>>(
		         coco);
		     ar(CEREAL_NVP(value));
	     }},
	    {"haldls::vx::Barrier", [](Archive& ar, coco_type& coco) {
		     auto& value =
		         std::get<std::pair<halco::hicann_dls::vx::BarrierOnFPGA, haldls::vx::Barrier>>(
		             coco);
		     ar(CEREAL_NVP(value));
	     }}};

	size_t size = values.size();
	ar(CEREAL_NVP(size));
	values.resize(size);
	for (auto& value : values) {
		std::string name = std::visit(
		    [](auto const& v) { return hate::full_name<std::decay_t<decltype(v.first)>>(); },
		    value);
		ar(CEREAL_NVP(name));
		if constexpr (Archive::is_loading::value) {
			auto const load = lookup.find(name);
			if (load == lookup.end()) {
				throw std::runtime_error("Deserialization of CoCo of specified name not found.");
			}
			load->second(ar, value);
		} else {
			lookup.at(name)(ar, value);
		}
	}
}

} // namespace stadls::vx::v2
