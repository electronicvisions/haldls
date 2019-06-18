#include "haldls/v2/dac.h"

#include "bitter/bitter.h"

#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/cerealization.h"

namespace haldls {
namespace v2 {

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
	std::bitset<2> const vdd = 0b00;
	/// \brief Controls whether the reference for each of the two groups is buffered.
	std::bitset<2> const buffered = 0b11;
	/// \brief Controls the output range of the DAC.
	/// \note The gain bits should never be different from 0 as this could damage the
	///       hardware.
	std::bitset<2> const gain = 0b00;
};

std::uint32_t spi_via_ocp_encode(
	halco::hicann_dls::v2::DACOnBoard const& target,
	std::bitset<16> const& data,
	bool emit_addresses)
{
	{
		using halco::hicann_dls::v2::DACOnBoard;
		assert(DACOnBoard::DAC_25_DECIVOLT.value() == 0);
		assert(DACOnBoard::DAC_12_DECIVOLT.value() == 1);
		assert(DACOnBoard::min == 0);
		assert(DACOnBoard::max == 1);
	}
	std::bitset<2> addr = target.value();

	// vvv ------8<----------- (legacy code copied from frickel-dls)
	std::bitset<8> upper = bit::crop<8>(data, 8);
	std::bitset<8> lower = bit::crop<8>(data);

	std::bitset<16> a = bit::concat(
		std::bitset<4>(4), // a[15:12] = 4'b0100
		                   // NOTE(Christian): This selects the ocp spi module on the fpga.
		std::bitset<2>(3), // a[11:10] = 2'b11
		addr,              // a[9:8]
		upper              // a[7:0]
	);
	// ^^^ ------8<-----------

	return emit_addresses ? a.to_ulong() : lower.to_ulong();
}

std::array<std::uint32_t, DAC::config_size_in_words> address_encode_impl(
	halco::hicann_dls::v2::DACOnBoard const& dac,
	DAC const& config,
	bool emit_addresses)
{
	std::array<std::uint32_t, DAC::config_size_in_words> result;
	{
		DACControlConfig ctrl;
		// vvv ------8<----------- (legacy code copied from frickel-dls)
		std::bitset<16> data = bit::concat(
			std::bitset<1>(1), // not data / control
			std::bitset<2>(0), // control bits
			std::bitset<7>(0), // not used
			ctrl.gain, ctrl.buffered, ctrl.vdd);
		// ^^^ ------8<-----------

		result[0] = spi_via_ocp_encode(dac, data, emit_addresses);
	}

	for (auto const channel : halco::common::iter_all<DAC::Channel>()) {
		size_t const idx = 1 + channel;

		// vvv ------8<----------- (legacy code copied from frickel-dls)
		std::bitset<16> data = bit::concat(
			std::bitset<1>(0),  // not data / control
			std::bitset<3>(channel),
			std::bitset<12>(config.get(channel)));
		// ^^^ ------8<-----------

		result.at(idx) = spi_via_ocp_encode(dac, data, emit_addresses);
	}
	return result;
}

} // namespace

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

auto DAC::write_addresses(coordinate_type const& dac) const
	-> std::array<ocp_address_type, config_size_in_words>
{
	std::array<ocp_address_type, config_size_in_words> result;
	auto data = address_encode_impl(dac, *this, /*emit_addresses=*/true);
	for (size_t ii = 0; ii < config_size_in_words; ++ii) {
		result[ii].value = data[ii];
	}
	return result;
}

auto DAC::read_addresses(coordinate_type const& /*dac*/) const -> std::array<ocp_address_type, 0>
{
	return {{}};
}

auto DAC::encode(coordinate_type const& dac) const
	-> std::array<ocp_word_type, config_size_in_words>
{
	std::array<ocp_word_type, config_size_in_words> result;
	auto data = address_encode_impl(dac, *this, /*emit_addresses=*/false);
	for (size_t ii = 0; ii < config_size_in_words; ++ii) {
		result[ii].value = data[ii];
	}
	return result;
}

void DAC::decode(coordinate_type const& /*dac*/, std::array<ocp_word_type, 0> const& /*words*/)
{
	// Not implemented or supported (see task #2695)
}

template <class Archive>
void DAC::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_channels));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(DAC)

} // namespace v2
} // namespace haldls
