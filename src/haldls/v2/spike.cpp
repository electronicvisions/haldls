#include "haldls/v2/spike.h"

#include <tuple>

#include "halco/common/cerealization_geometry.h"
#include "haldls/cerealization.h"

namespace haldls {
namespace v2 {

PlaybackSpike::PlaybackSpike() : m_time(0), m_source_address(0), m_synapse_driver(0) {}

PlaybackSpike::PlaybackSpike(
	hardware_time_type const time,
	SynapseBlock::Synapse::Address const& source_address,
	halco::hicann_dls::v2::SynapseDriverOnDLS const& synapse_driver)
	: m_time(time), m_source_address(source_address), m_synapse_driver(synapse_driver)
{}

hardware_time_type PlaybackSpike::get_time() const
{
	return m_time;
}

SynapseBlock::Synapse::Address PlaybackSpike::get_source_address() const
{
	return m_source_address;
}

halco::hicann_dls::v2::SynapseDriverOnDLS PlaybackSpike::get_synapse_driver() const
{
	return m_synapse_driver;
}

bool PlaybackSpike::operator==(PlaybackSpike const& other) const
{
	// clang-format off
	return (
		m_time == other.get_time() &&
		m_source_address == other.get_source_address() &&
		m_synapse_driver == other.get_synapse_driver());
	// clang-format on
}

bool PlaybackSpike::operator!=(PlaybackSpike const& other) const
{
	return !(*this == other);
}

bool PlaybackSpike::operator<(PlaybackSpike const& other) const
{
	return std::make_tuple(m_time, m_synapse_driver, m_source_address) <
		   std::make_tuple(
			   other.get_time(), other.get_synapse_driver(), other.get_source_address());
}

bool PlaybackSpike::operator>(PlaybackSpike const& other) const
{
	return other < *this;
}

bool PlaybackSpike::operator<=(PlaybackSpike const& other) const
{
	return !(*this > other);
}

bool PlaybackSpike::operator>=(PlaybackSpike const& other) const
{
	return !(*this < other);
}

std::ostream& operator<<(std::ostream& os, PlaybackSpike const& spike)
{
	return os << "PlaybackSpike(" << spike.get_time() << ", " << spike.get_source_address() << ", "
			  << spike.get_synapse_driver() << ")";
}

template <class Archive>
void PlaybackSpike::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_time));
	ar(CEREAL_NVP(m_source_address));
	ar(CEREAL_NVP(m_synapse_driver));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(PlaybackSpike)

RecordedSpike::RecordedSpike() : m_time(0), m_neuron(0) {}

RecordedSpike::RecordedSpike(
	hardware_time_type const time, halco::hicann_dls::v2::NeuronOnDLS const& neuron)
	: m_time(time), m_neuron(neuron)
{}

hardware_time_type RecordedSpike::get_time() const
{
	return m_time;
}

halco::hicann_dls::v2::NeuronOnDLS RecordedSpike::get_neuron() const
{
	return m_neuron;
}

bool RecordedSpike::operator==(RecordedSpike const& other) const
{
	// clang-format off
	return (
		m_time == other.get_time() &&
		m_neuron == other.get_neuron());
	// clang-format on
}

bool RecordedSpike::operator!=(RecordedSpike const& other) const
{
	return !(*this == other);
}

bool RecordedSpike::operator<(RecordedSpike const& other) const
{
	return std::make_tuple(m_time, m_neuron) <
		   std::make_tuple(other.get_time(), other.get_neuron());
}

bool RecordedSpike::operator>(RecordedSpike const& other) const
{
	return other < *this;
}

bool RecordedSpike::operator<=(RecordedSpike const& other) const
{
	return !(*this > other);
}

bool RecordedSpike::operator>=(RecordedSpike const& other) const
{
	return !(*this < other);
}

std::ostream& operator<<(std::ostream& os, RecordedSpike const& spike)
{
	return os << "RecordedSpike(" << spike.get_time() << ", " << spike.get_neuron() << ")";
}

template <class Archive>
void RecordedSpike::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_time));
	ar(CEREAL_NVP(m_neuron));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(RecordedSpike)

} // namespace v2
} // namespace haldls
