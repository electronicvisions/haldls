#include "haldls/container/v2/dac.h"

namespace {

/// \brief Control bits affecting all channels.
/// The chip used on the current baseboard is a \c AD5328ARUZ.
/// \see http://www.analog.com/media/en/technical-documentation/data-sheets/AD5308_5318_5328.pdf
/// As described on p. 17 of the data sheet the eight DACs provided by this chip are
/// organized in two groups, which provide individual control bits.
/// Thus there are two gain bits, two bits controlling the buffering of the reference and two
/// bits for controlling whether VDD should be used as a reference.
/// Both gain bits \e need to be zero and the buf bits \e can be either but KS recommends
/// setting them to one.  The VDD bits have to be set to zero.
struct DACControlConfig
{
	/// \brief Controls whether VDD should be used as the reference.
	std::bitset<2> const m_vdd = 0b00;
	/// \brief Controls whether the reference for each of the two groups is buffered.
	std::bitset<2> const m_buffered = 0b11;
	/// \brief Controls the output range of the DAC.
	/// \note The gain bits should never be different from 0 as this could damage the
	///       hardware.
	std::bitset<2> const m_gain = 0b00;
};

} // namespace

namespace haldls {
namespace container {
namespace v2 {

DAC::DAC() : m_channels() {}

DAC::Value DAC::get(DAC::Channel const& channel) const
{
	return m_channels.at(channel);
}

void DAC::set(DAC::Channel const& channel, DAC::Value const& value)
{
	m_channels.at(channel) = value;
}

bool DAC::operator==(DAC const& other) const
{
	return m_channels == other.m_channels;
}

bool DAC::operator!=(DAC const& other) const
{
	return !(*this == other);
}

} // namespace v2
} // namespace container
} // namespace haldls
