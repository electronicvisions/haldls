#include "haldls/v2/rate_counter.h"

#include <numeric>

#include "halco/common/iter_all.h"
#include "halco/hicann-dls/v2/coordinates.h"

using namespace halco::common;
using namespace halco::hicann_dls::v2;

namespace haldls {
namespace v2 {

RateCounterConfig::RateCounterConfig()
	: m_enable_neuron(), m_clear_on_read(false), m_fire_interrupt(false)
{}

bool RateCounterConfig::get_enable_neuron(halco::hicann_dls::v2::NeuronOnDLS const& neuron) const
{
	return m_enable_neuron.at(neuron);
}

void RateCounterConfig::set_enable_neuron(
	halco::hicann_dls::v2::NeuronOnDLS const& neuron, bool const value)
{
	m_enable_neuron.at(neuron) = value;
}

bool RateCounterConfig::get_enable_fire_interrupt() const
{
	return m_fire_interrupt;
}

void RateCounterConfig::set_enable_fire_interrupt(bool const value)
{
	m_fire_interrupt = value;
}

bool RateCounterConfig::get_enable_clear_on_read() const
{
	return m_clear_on_read;
}

void RateCounterConfig::set_enable_clear_on_read(bool const value)
{
	m_clear_on_read = value;
}

bool RateCounterConfig::operator==(RateCounterConfig const& other) const
{
	// clang-format off
	return (
		m_enable_neuron == other.m_enable_neuron &&
		m_fire_interrupt == other.m_fire_interrupt &&
		m_clear_on_read == other.m_clear_on_read);
	// clang-format on
}

bool RateCounterConfig::operator!=(RateCounterConfig const& other) const
{
	return !(*this == other);
}

std::array<hardware_address_type, RateCounterConfig::config_size_in_words> RateCounterConfig::addresses(coordinate_type const& /*unique*/) const
{
	hardware_address_type const base_address = 0x1e000020;
	std::array<hardware_address_type, config_size_in_words> result;
	std::iota(result.begin(), result.end(), base_address);
	return result;
}

std::array<hardware_word_type, RateCounterConfig::config_size_in_words> RateCounterConfig::encode() const
{
	std::array<hardware_word_type, config_size_in_words> result;
	std::bitset<NeuronOnDLS::size> enable_neuron;
	for (auto const neuron : iter_all<NeuronOnDLS>()) {
		enable_neuron.set(neuron.value(), m_enable_neuron[neuron]);
	}
	result.at(0) = enable_neuron.to_ulong();
	result.at(1) =
		(static_cast<hardware_word_type>(m_clear_on_read) << 1 |
		 static_cast<hardware_word_type>(m_fire_interrupt));
	return result;
}

void RateCounterConfig::decode(std::array<hardware_word_type, config_size_in_words> const& data)
{
	std::bitset<NeuronOnDLS::size> enable_neuron(data.at(0));
	for (auto const neuron : iter_all<NeuronOnDLS>()) {
		m_enable_neuron[neuron] = enable_neuron[neuron.value()];
	}
	hardware_word_type const switches = data.at(1);
	m_clear_on_read = switches & 0b0010ul;
	m_fire_interrupt = switches & 0b0001ul;
}

template <class Archive>
void RateCounterConfig::cerealize(Archive& ar)
{
	ar(CEREAL_NVP(m_enable_neuron));
	ar(CEREAL_NVP(m_clear_on_read));
	ar(CEREAL_NVP(m_fire_interrupt));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(RateCounterConfig)


RateCounterEntry::RateCounterEntry()
	: m_count()
{}

RateCounterEntry::Count RateCounterEntry::get_count() const
{
	return m_count;
}

bool RateCounterEntry::operator==(RateCounterEntry const& other) const
{
	return (m_count == other.m_count);
}

bool RateCounterEntry::operator!=(RateCounterEntry const& other) const
{
	return !(*this == other);
}

std::array<hardware_address_type, RateCounterEntry::config_size_in_words> RateCounterEntry::addresses(coordinate_type const& coord) const
{
	hardware_address_type const base_address = 0x1e000000;
	std::array<hardware_address_type, config_size_in_words> result;
	result.at(0) = base_address + coord.value();
	return result;
}

std::array<hardware_word_type, RateCounterEntry::config_size_in_words> RateCounterEntry::encode() const
{
	return {static_cast<hardware_word_type>(m_count)};
}

void RateCounterEntry::decode(std::array<hardware_word_type, config_size_in_words> const& data)
{
	m_count = Count(data.at(0));
}

template <class Archive>
void RateCounterEntry::cerealize(Archive& ar)
{
	ar(CEREAL_NVP(m_count));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(RateCounterEntry)


RateCounter::RateCounter()
	: m_counts()
{}

RateCounter::Count RateCounter::get_count(halco::hicann_dls::v2::NeuronOnDLS const& neuron) const
{
	return m_counts.at(neuron).get_count();
}

bool RateCounter::operator==(RateCounter const& other) const
{
	return (m_counts == other.m_counts);
}

bool RateCounter::operator!=(RateCounter const& other) const
{
	return !(*this == other);
}

template <class Archive>
void RateCounter::cerealize(Archive& ar)
{
	ar(CEREAL_NVP(m_counts));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(RateCounter)

} // namespace v2
} // namespace haldls
