#pragma once

#include <ostream>

#include "halco/hicann-dls/v2/coordinates.h"

#include "haldls/common/visibility.h"

namespace haldls {
namespace container {
namespace v2 {

class CorrelationConfig
{
public:
	struct Delay
		: public halco::common::detail::RantWrapper<Delay, uint_fast16_t, 4095, 0>
	{
		constexpr explicit Delay(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};

	CorrelationConfig() HALDLS_VISIBLE;

	Delay get_sense_delay() const HALDLS_VISIBLE;
	void set_sense_delay(Delay const& value) HALDLS_VISIBLE;

	Delay get_reset_delay_1() const HALDLS_VISIBLE;
	void set_reset_delay_1(Delay const& value) HALDLS_VISIBLE;

	Delay get_reset_delay_2() const HALDLS_VISIBLE;
	void set_reset_delay_2(Delay const& value) HALDLS_VISIBLE;

	bool operator==(CorrelationConfig const& other) const HALDLS_VISIBLE;
	bool operator!=(CorrelationConfig const& other) const HALDLS_VISIBLE;

private:
	Delay m_sense_delay;
	Delay m_reset_delay_1;
	Delay m_reset_delay_2;
};

} // namespace v2
} // namespace container
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::CorrelationConfig::Delay)

} // namespace std
