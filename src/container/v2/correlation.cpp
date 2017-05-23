#include "haldls/container/v2/correlation.h"

namespace haldls {
namespace container {
namespace v2 {

CorrelationConfig::CorrelationConfig() : m_sense_delay(0), m_reset_delay_1(0), m_reset_delay_2(0) {}

CorrelationConfig::Delay CorrelationConfig::get_sense_delay() const
{
	return m_sense_delay;
}

void CorrelationConfig::set_sense_delay(CorrelationConfig::Delay const& value)
{
	m_sense_delay = value;
}

CorrelationConfig::Delay CorrelationConfig::get_reset_delay_1() const
{
	return m_reset_delay_1;
}

void CorrelationConfig::set_reset_delay_1(CorrelationConfig::Delay const& value)
{
	m_reset_delay_1 = value;
}

CorrelationConfig::Delay CorrelationConfig::get_reset_delay_2() const
{
	return m_reset_delay_2;
}

void CorrelationConfig::set_reset_delay_2(CorrelationConfig::Delay const& value)
{
	m_reset_delay_2 = value;
}

bool CorrelationConfig::operator==(CorrelationConfig const& other) const
{
	return get_sense_delay() == other.get_sense_delay() &&
		   get_reset_delay_1() == other.get_reset_delay_1() &&
		   get_reset_delay_2() == other.get_reset_delay_2();
}

bool CorrelationConfig::operator!=(CorrelationConfig const& other) const
{
	return !(*this == other);
}

} // namespace v2
} // namespace container
} // namespace haldls
