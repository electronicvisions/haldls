#include "stadls/vx/dumper.h"

#include <cereal/types/utility.hpp>
#include <cereal/types/variant.hpp>
#include <cereal/types/vector.hpp>

#include "halco/common/cerealization_geometry.h"
#include "haldls/cerealization.h"
#include "lola/vx/cerealization.h"

namespace stadls::vx {

bool DumperDone::operator==(DumperDone const& other) const
{
	return values == other.values;
}

bool DumperDone::operator!=(DumperDone const& other) const
{
	return !(*this == other);
}

template <typename Archive>
void DumperDone::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(values));
}

void Dumper::block_until(
    typename haldls::vx::Timer::coordinate_type const& coord, haldls::vx::Timer::Value time)
{
	m_dumpit.values.push_back(std::make_pair(coord, time));
}

void Dumper::block_until(
    halco::hicann_dls::vx::BarrierOnFPGA const& coord, haldls::vx::Barrier barrier)
{
	m_dumpit.values.push_back(std::make_pair(coord, barrier));
}

template <typename ContainerT>
void Dumper::write(typename ContainerT::coordinate_type const& coord, ContainerT const& config)
{
	m_dumpit.values.push_back(std::make_pair(coord, config));
}

template <typename ContainerT>
PlaybackProgram::ContainerTicket<ContainerT> Dumper::read(ContainerT::coordinate_type const&)
{
	throw std::runtime_error("Dumper does not support reads.");
}

Dumper::done_type Dumper::done()
{
	decltype(m_dumpit) ret;
	ret = std::move(m_dumpit);
	return ret;
}

void Dumper::merge_back(Dumper& other)
{
	m_dumpit.values.insert(
	    m_dumpit.values.end(), other.m_dumpit.values.begin(), other.m_dumpit.values.end());
	other.m_dumpit.values.clear();
}

void Dumper::merge_front(Dumper& other)
{
	Dumper::done_type tmp;
	std::swap(tmp, other.m_dumpit);
	std::copy(m_dumpit.values.begin(), m_dumpit.values.end(), std::back_inserter(tmp.values));
	std::swap(m_dumpit, tmp);
}

void Dumper::copy_back(Dumper const& other)
{
	std::copy(
	    other.m_dumpit.values.begin(), other.m_dumpit.values.end(),
	    std::back_inserter(m_dumpit.values));
}

bool Dumper::empty() const
{
	return m_dumpit.values.empty();
}

std::ostream& operator<<(std::ostream& os, Dumper const& builder)
{
	for (auto const& coco_variant : builder.m_dumpit.values) {
		std::visit(
		    [&os](auto const& coco_variant) {
			    os << std::get<0>(coco_variant) << " " << std::get<1>(coco_variant) << "\n";
		    },
		    coco_variant);
	}
	return os;
}

template <typename Archive>
void Dumper::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_dumpit));
}

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	template void Dumper::write<Type>(                                                             \
	    typename Type::coordinate_type const& coord, Type const& config);                          \
	template PlaybackProgram::ContainerTicket<Type> Dumper::read(                                  \
	    typename Type::coordinate_type const& coord);
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/container.def"

} // namespace stadls::vx

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::DumperDone)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::Dumper)
