#include "stadls/vx/v3/dumperdone.h"

#include "halco/common/cerealization_geometry.h"
#include "haldls/cerealization.tcc"
#include "hate/type_index.h"
#include <map>
#include <stdexcept>
#include <cereal/types/string.hpp>
#include <cereal/types/utility.hpp>

namespace stadls::vx::v3 {

namespace {

template <typename Archive, typename Coordinate, typename Container>
void serialize_coco(Archive& ar, DumperDone::coco_type& coco)
{
	typedef std::pair<Coordinate, Container> local_coco_type;
	if (!std::holds_alternative<local_coco_type>(coco)) {
		assert(Archive::is_loading::value);
		coco = local_coco_type();
	}
	auto& value = std::get<local_coco_type>(coco);
	ar(CEREAL_NVP(value));
}

} // namespace

template <typename Archive>
void DumperDone::serialize(Archive& ar, std::uint32_t const)
{
	static const std::map<std::string, void (*)(Archive&, coco_type&)> lookup = {
#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	std::make_pair(                                                                                \
	    hate::full_name<std::pair<typename Type::coordinate_type, Type>>(),                        \
	    &serialize_coco<Archive, typename Type::coordinate_type, Type>),
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v3/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v3/container.def"
	    {hate::full_name<
	         std::pair<typename haldls::vx::Timer::coordinate_type, haldls::vx::Timer::Value>>(),
	     &serialize_coco<
	         Archive, typename haldls::vx::Timer::coordinate_type, haldls::vx::Timer::Value>},
	    {hate::full_name<std::pair<halco::hicann_dls::vx::BarrierOnFPGA, haldls::vx::Barrier>>(),
	     &serialize_coco<Archive, halco::hicann_dls::vx::BarrierOnFPGA, haldls::vx::Barrier>},
	    {hate::full_name<std::pair<
	         halco::hicann_dls::vx::PollingOmnibusBlockOnFPGA, haldls::vx::PollingOmnibusBlock>>(),
	     &serialize_coco<
	         Archive, halco::hicann_dls::vx::PollingOmnibusBlockOnFPGA,
	         haldls::vx::PollingOmnibusBlock>}};

	size_t size = values.size();
	ar(CEREAL_NVP(size));
	values.resize(size);
	for (auto& value : values) {
		std::string name = std::visit(
		    [](auto const& v) { return hate::full_name<std::decay_t<decltype(v)>>(); }, value);
		ar(CEREAL_NVP(name));
		auto const serializer = lookup.find(name);
		if (serializer == lookup.end()) {
			throw std::runtime_error(
			    "Serializer of CoCo of specified name(" + name + ") not found.");
		}
		serializer->second(ar, value);
	}
}

} // namespace stadls::vx::v3
