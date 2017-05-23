#include "haldls/container/v2/rate_counter.h"
#include "halco/hicann-dls/v2/coordinates.h"

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

} // namespace v2
} // namespace container
} // namespace haldls
