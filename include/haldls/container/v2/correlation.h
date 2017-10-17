#pragma once

#include <ostream>

#include "halco/common/genpybind.h"
#include "halco/hicann-dls/v2/coordinates.h"

#include "haldls/common/visibility.h"
#include "haldls/container/v2/common.h"

namespace haldls {
namespace container {
namespace v2 GENPYBIND(tag(haldls_container_v2)) {

class GENPYBIND(visible) CorrelationConfig
{
public:
	typedef halco::common::Unique coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) Delay
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

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 3;
	std::array<hardware_address_type, config_size_in_words> addresses(
		coordinate_type const& unique) const HALDLS_VISIBLE GENPYBIND(hidden);
	std::array<hardware_word_type, config_size_in_words> encode() const HALDLS_VISIBLE GENPYBIND(hidden);
	void decode(std::array<hardware_word_type, config_size_in_words> const& data) HALDLS_VISIBLE GENPYBIND(hidden);

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
