#include "haldls/vx/timer.h"

#include "fisch/vx/constants.h"
#include "halco/common/cerealization_geometry.h"
#include "haldls/cerealization.tcc"

namespace haldls::vx {

Timer::Value const Timer::Value::fpga_clock_cycles_per_us{fisch::vx::fpga_clock_cycles_per_us};

Timer::Timer(Value const value) : m_value(value) {}

void Timer::set(Value const value)
{
	m_value = value;
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

std::array<halco::hicann_dls::vx::TimerOnDLS, Timer::write_config_size_in_words>
Timer::write_addresses(coordinate_type const& coord)
{
	return {coord};
}

std::array<halco::hicann_dls::vx::TimerOnDLS, Timer::read_config_size_in_words>
Timer::read_addresses(coordinate_type const& /*coord*/)
{
	return {};
}

std::array<fisch::vx::word_access_type::Timer, Timer::write_config_size_in_words> Timer::encode()
    const
{
	return {fisch::vx::word_access_type::Timer()};
}

void Timer::decode(std::array<
                   fisch::vx::word_access_type::Timer,
                   Timer::read_config_size_in_words> const& /*data*/)
{}

template <class Archive>
void Timer::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(Timer)

} // namespace haldls::vx

CEREAL_CLASS_VERSION(haldls::vx::Timer, 0)
