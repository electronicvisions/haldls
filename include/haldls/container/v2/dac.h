#pragma once

#include <ostream>

#include "halco/common/typed_array.h"
#include "halco/hicann-dls/v2/coordinates.h"

#include "haldls/common/visibility.h"


namespace haldls {
namespace container {
namespace v2 {

/// \brief DAC chip located on the baseboard.
/// There are two instances on the baseboard: \c DAC12 (1.2V) and \c DAC25 (2.5V).  Both
/// are configured via I2C by the FPGA.
class DAC
{
public:
	struct Value
		: public halco::common::detail::RantWrapper<Value, uint_fast16_t, 4095, 0>
	{
		constexpr explicit Value(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};

	struct Channel
		: public halco::common::detail::RantWrapper<Channel, uint_fast16_t, 7, 0>
	{
		constexpr explicit Channel(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};

	DAC() HALDLS_VISIBLE;

	Value get(Channel const& channel) const HALDLS_VISIBLE;
	void set(Channel const& channel, Value const& value) HALDLS_VISIBLE;

	bool operator==(DAC const& other) const HALDLS_VISIBLE;
	bool operator!=(DAC const& other) const HALDLS_VISIBLE;

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
