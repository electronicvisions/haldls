#include "haldls/vx/timer.h"

#include "fisch/vx/constants.h"
#include "haldls/vx/block_until.tcc"
#include "haldls/vx/container.tcc"

namespace haldls::vx {

Timer::Value const Timer::Value::fpga_clock_cycles_per_us{fisch::vx::fpga_clock_cycles_per_us};

std::array<halco::hicann_dls::vx::WaitUntilOnFPGA, Timer::Value::write_config_size_in_words>
Timer::Value::write_addresses(coordinate_type const& /* coord */)
{
	return {halco::hicann_dls::vx::WaitUntilOnFPGA()};
}

std::array<fisch::vx::word_access_type::WaitUntil, Timer::Value::write_config_size_in_words>
Timer::Value::encode() const
{
	return {fisch::vx::word_access_type::WaitUntil(value())};
}


Timer::Timer(Value const value) : m_value(value) {}

void Timer::set(Value const value)
{
	m_value = value;
}

Timer::Value Timer::get() const
{
	return m_value;
}

bool Timer::operator==(Timer const& other) const
{
	return m_value == other.m_value;
}

bool Timer::operator!=(Timer const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, Timer const& config)
{
	os << "Timer(" << config.m_value << ")";
	return os;
}

std::array<halco::hicann_dls::vx::TimerOnDLS, Timer::config_size_in_words> Timer::addresses(
    coordinate_type const& coord)
{
	return {coord};
}

std::array<fisch::vx::word_access_type::Timer, Timer::config_size_in_words> Timer::encode() const
{
	return {fisch::vx::word_access_type::Timer()};
}

void Timer::decode(
    std::array<fisch::vx::word_access_type::Timer, Timer::config_size_in_words> const& data)
{
	m_value = Value(data[0]);
}

} // namespace haldls::vx

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::Timer)
EXPLICIT_INSTANTIATE_HALDLS_BLOCK_UNTIL_BASE(haldls::vx::Timer::Value)
