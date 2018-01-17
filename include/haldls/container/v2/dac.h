#pragma once

#include <ostream>

#include "halco/common/genpybind.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/v2/coordinates.h"

#include "haldls/common/visibility.h"
#include "haldls/container/v2/common.h"


namespace haldls {
namespace container {
namespace v2 GENPYBIND(tag(haldls_container_v2)) {

/// \brief DAC chip located on the baseboard.
/// There are two instances on the baseboard: \c DAC12 (1.2V) and \c DAC25 (2.5V).  Both
/// are configured via I2C by the FPGA.
class GENPYBIND(visible) DAC
{
public:
	typedef halco::hicann_dls::v2::DACOnBoard coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) Value
		: public halco::common::detail::RantWrapper<Value, uint_fast16_t, 4095, 0>
	{
		constexpr explicit Value(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};

	struct GENPYBIND(inline_base("*")) Channel
		: public halco::common::detail::RantWrapper<Channel, uint_fast16_t, 7, 0>
	{
		constexpr explicit Channel(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};

	DAC() HALDLS_VISIBLE;

	Value get(Channel const& channel) const HALDLS_VISIBLE;
	void set(Channel const& channel, Value const& value) HALDLS_VISIBLE;

	bool operator==(DAC const& other) const HALDLS_VISIBLE;
	bool operator!=(DAC const& other) const HALDLS_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1 + Channel::size;
	std::array<ocp_address_type, config_size_in_words> write_addresses(
		coordinate_type const& dac) const HALDLS_VISIBLE GENPYBIND(hidden);
	std::array<ocp_address_type, 0> read_addresses(coordinate_type const& dac) const HALDLS_VISIBLE
		GENPYBIND(hidden);
	std::array<ocp_word_type, config_size_in_words> encode(coordinate_type const& dac) const
		HALDLS_VISIBLE GENPYBIND(hidden);
	void decode(coordinate_type const& dac, std::array<ocp_word_type, 0> const& words)
		HALDLS_VISIBLE GENPYBIND(hidden);

private:
	halco::common::typed_array<Value, Channel> m_channels;
};

} // namespace v2
} // namespace container
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::DAC::Value)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::DAC::Channel)

} // namespace std
