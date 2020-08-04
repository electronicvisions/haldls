#include "haldls/vx/timer.h"

#include "fisch/vx/constants.h"
#include "halco/common/cerealization_geometry.h"
#include "haldls/cerealization.h"
#include "haldls/vx/print.h"

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

HALDLS_VX_DEFAULT_OSTREAM_OP(Timer)

std::array<halco::hicann_dls::vx::TimerOnDLS, Timer::config_size_in_words> Timer::addresses(
    coordinate_type const& coord)
{
	return {coord};
}

std::array<fisch::vx::Timer, Timer::config_size_in_words> Timer::encode() const
{
	return {fisch::vx::Timer{fisch::vx::Timer::Value{m_value.value()}}};
}

void Timer::decode(std::array<fisch::vx::Timer, Timer::config_size_in_words> const& /*data*/) {}

template <class Archive>
void Timer::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(Timer)

} // namespace haldls::vx

CEREAL_CLASS_VERSION(haldls::vx::Timer, 0)
