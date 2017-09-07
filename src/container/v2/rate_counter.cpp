#include "haldls/container/v2/rate_counter.h"

#include "halco/common/iter_all.h"
#include "halco/hicann-dls/v2/coordinates.h"

using namespace halco::common;
using namespace halco::hicann_dls::v2;

namespace haldls {
namespace container {
namespace v2 {

RateCounter::RateCounter()
	: m_counts(), m_neuron_enable(), m_clear_on_read(false), m_fire_interrupt(false)
{}

RateCounter::Count RateCounter::get_count(halco::hicann_dls::v2::NeuronOnDLS const& neuron) const
{
	return m_counts.at(neuron);
}

bool RateCounter::get_neuron_enable(halco::hicann_dls::v2::NeuronOnDLS const& neuron) const
{
	return m_neuron_enable.at(neuron);
}

void RateCounter::set_neuron_enable(
	halco::hicann_dls::v2::NeuronOnDLS const& neuron, bool const value)
{
	m_neuron_enable.at(neuron) = value;
}

bool RateCounter::get_fire_interrupt() const
{
	return m_fire_interrupt;
}

void RateCounter::set_fire_interrupt(bool const value)
{
	m_fire_interrupt = value;
}

bool RateCounter::get_clear_on_read() const
{
	return m_clear_on_read;
}

void RateCounter::set_clear_on_read(bool const value)
{
	m_clear_on_read = value;
}

bool RateCounter::operator==(RateCounter const& other) const
{
	// clang-format off
	return (
		m_counts == other.m_counts &&
		m_neuron_enable == other.m_neuron_enable &&
		m_fire_interrupt == other.m_fire_interrupt &&
		m_clear_on_read == other.m_clear_on_read);
	// clang-format on
}

bool RateCounter::operator!=(RateCounter const& other) const
{
	return !(*this == other);
}

std::array<hardware_address_type, RateCounter::config_size_in_words> RateCounter::addresses(coordinate_type const& /*unique*/) const
{
	hardware_address_type const base_address = 0x1e000000;
	std::array<hardware_address_type, config_size_in_words> result;
	std::iota(result.begin(), result.end(), base_address);
	return result;
}

std::array<hardware_word_type, RateCounter::config_size_in_words> RateCounter::encode() const
{
	std::array<hardware_word_type, config_size_in_words> result;
	std::bitset<NeuronOnDLS::size> neuron_enable;
	for (auto const neuron : iter_all<NeuronOnDLS>()) {
		neuron_enable.set(neuron.value(), m_neuron_enable[neuron]);
		result.at(neuron.value()) = m_counts[neuron].value();
	}
	result.at(NeuronOnDLS::size) = neuron_enable.to_ulong();
	result.at(NeuronOnDLS::size + 1) =
		(static_cast<hardware_word_type>(m_clear_on_read) << 1 |
		 static_cast<hardware_word_type>(m_fire_interrupt));
	return result;
}

void RateCounter::decode(std::array<hardware_word_type, config_size_in_words> const& data)
{
	std::bitset<NeuronOnDLS::size> neuron_enable(data.at(NeuronOnDLS::size));
	for (auto const neuron : iter_all<NeuronOnDLS>()) {
		m_neuron_enable[neuron] = neuron_enable[neuron.value()];
		m_counts[neuron] = Count(data.at(neuron.value()));
	}
	hardware_word_type const switches = data.at(NeuronOnDLS::size + 1);
	m_clear_on_read = switches & 0b0010ul;
	m_fire_interrupt = switches & 0b0001ul;
}

} // namespace v2
} // namespace container
} // namespace haldls
