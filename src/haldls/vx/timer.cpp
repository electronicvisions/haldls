#include "haldls/vx/timer.h"
#include "haldls/cerealization.h"

namespace haldls::vx {

Timer::Timer() : m_value() {}

Timer::Timer(Value const value) : m_value()
{
	m_value.set(value);
}

void Timer::set(Value const value)
{
	m_value.set(value);
}

bool Timer::operator==(Timer const& other) const
{
	return m_value == other.m_value;
}

bool Timer::operator!=(Timer const& other) const
{
	return !(*this == other);
}

std::array<halco::hicann_dls::vx::TimerOnDLS, Timer::config_size_in_words> Timer::addresses(
    coordinate_type const& coord) const
{
	return {coord};
}

std::array<fisch::vx::Timer, Timer::config_size_in_words> Timer::encode() const
{
	return {m_value};
}

void Timer::decode(std::array<fisch::vx::Timer, Timer::config_size_in_words> const& /*data*/) {}

template <class Archive>
void Timer::cerealize(Archive& ar)
{
	ar(CEREAL_NVP(m_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(Timer)

} // namespace haldls::vx
