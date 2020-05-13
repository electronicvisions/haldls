#include "stadls/vx/dumper.h"

#include <cereal/types/utility.hpp>
#include <cereal/types/variant.hpp>
#include <cereal/types/vector.hpp>

#include "halco/common/cerealization_geometry.h"
#include "haldls/cerealization.h"
#include "lola/vx/cerealization.h"

namespace stadls::vx::detail {

template <typename DoneType>
void Dumper<DoneType>::block_until(
    typename haldls::vx::Timer::coordinate_type const& coord, haldls::vx::Timer::Value time)
{
	m_dumpit.values.push_back(std::make_pair(coord, time));
}

template <typename DoneType>
void Dumper<DoneType>::block_until(
    halco::hicann_dls::vx::BarrierOnFPGA const& coord, haldls::vx::Barrier barrier)
{
	m_dumpit.values.push_back(std::make_pair(coord, barrier));
}

template <typename DoneType>
template <typename ContainerT>
void Dumper<DoneType>::write(typename ContainerT::coordinate_type const& coord, ContainerT const& config)
{
	m_dumpit.values.push_back(std::make_pair(coord, config));
}

template <typename DoneType>
template <typename ContainerT>
PlaybackProgram::ContainerTicket<ContainerT>
Dumper<DoneType>::read(typename ContainerT::coordinate_type const&)
{
	throw std::runtime_error("Dumper does not support reads.");
}

template <typename DoneType>
typename Dumper<DoneType>::done_type Dumper<DoneType>::done()
{
	decltype(m_dumpit) ret;
	ret = std::move(m_dumpit);
	return ret;
}

template <typename DoneType>
void Dumper<DoneType>::merge_back(Dumper& other)
{
	m_dumpit.values.insert(
	    m_dumpit.values.end(), other.m_dumpit.values.begin(), other.m_dumpit.values.end());
	other.m_dumpit.values.clear();
}

template <typename DoneType>
void Dumper<DoneType>::merge_front(Dumper& other)
{
	Dumper::done_type tmp;
	std::swap(tmp, other.m_dumpit);
	std::copy(m_dumpit.values.begin(), m_dumpit.values.end(), std::back_inserter(tmp.values));
	std::swap(m_dumpit, tmp);
}

template <typename DoneType>
void Dumper<DoneType>::copy_back(Dumper const& other)
{
	std::copy(
	    other.m_dumpit.values.begin(), other.m_dumpit.values.end(),
	    std::back_inserter(m_dumpit.values));
}

template <typename DoneType>
bool Dumper<DoneType>::empty() const
{
	return m_dumpit.values.empty();
}

template <typename DoneType>
std::ostream& operator<<(std::ostream& os, Dumper<DoneType> const& builder)
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

template <typename DoneType>
template <typename Archive>
void Dumper<DoneType>::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_dumpit));
}

} // namespace stadls::vx
