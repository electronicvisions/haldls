#include <string>

#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/cerealization.h"
#include "haldls/vx/madc.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/print.h"

#include "fisch/vx/jtag.h"

namespace haldls {
namespace vx {


MADCControl::MADCControl() :
    m_enable_iconv_amplifier(false),
    m_enable_pre_amplifier(false),
    m_enable_continuous_sampling(false),
    m_enable_power_down_after_sampling(false),
    m_stop_recording(false),
    m_start_recording(false),
    m_wake_up(false)
{}

bool MADCControl::get_enable_iconv_amplifier() const
{
	return m_enable_iconv_amplifier;
}

void MADCControl::set_enable_iconv_amplifier(bool const value)
{
	m_enable_iconv_amplifier = value;
}

bool MADCControl::get_enable_pre_amplifier() const
{
	return m_enable_pre_amplifier;
}

void MADCControl::set_enable_pre_amplifier(bool const value)
{
	m_enable_pre_amplifier = value;
}

bool MADCControl::get_enable_continuous_sampling() const
{
	return m_enable_continuous_sampling;
}

void MADCControl::set_enable_continuous_sampling(bool const value)
{
	m_enable_continuous_sampling = value;
}

bool MADCControl::get_enable_power_down_after_sampling() const
{
	return m_enable_power_down_after_sampling;
}

void MADCControl::set_enable_power_down_after_sampling(bool const value)
{
	m_enable_power_down_after_sampling = value;
}

bool MADCControl::get_stop_recording() const
{
	return m_stop_recording;
}

void MADCControl::set_stop_recording(bool const value)
{
	m_stop_recording = value;
}

bool MADCControl::get_start_recording() const
{
	return m_start_recording;
}

void MADCControl::set_start_recording(bool const value)
{
	m_start_recording = value;
}

bool MADCControl::get_wake_up() const
{
	return m_wake_up;
}

void MADCControl::set_wake_up(bool const value)
{
	m_wake_up = value;
}

template <typename AddressT>
std::array<AddressT, MADCControl::write_config_size_in_words> MADCControl::write_addresses(
    coordinate_type const& /* coord */) const
{
	return {AddressT(madc_base_address)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    MADCControl::write_config_size_in_words>
MADCControl::write_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord) const;

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipAddress, MADCControl::write_config_size_in_words>
    MADCControl::write_addresses<halco::hicann_dls::vx::OmnibusChipAddress>(
        coordinate_type const& coord) const;


template <typename AddressT>
std::array<AddressT, MADCControl::read_config_size_in_words> MADCControl::read_addresses(
    coordinate_type const& /* coord */) const
{
	return {};
}

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, MADCControl::read_config_size_in_words>
    MADCControl::read_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
        coordinate_type const& coord) const;

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipAddress, MADCControl::read_config_size_in_words>
    MADCControl::read_addresses<halco::hicann_dls::vx::OmnibusChipAddress>(
        coordinate_type const& coord) const;

namespace {

struct MADCControlBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t wake_up                           :  1;  //       0
			uint32_t start_recording                   :  1;  //       1
			uint32_t stop_recording                    :  1;  //       2
			uint32_t enable_power_down_after_sampling  :  1;  //       3
			uint32_t enable_continuous_sampling        :  1;  //       4
			uint32_t enable_pre_amplifier              :  1;  //       5
			uint32_t enable_iconv_amplifier            :  1;  //       6
			uint32_t                                   : 25;  // 31 -  7
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "Sizes of union types should match.");
	} u;

	MADCControlBitfield() { u.raw = 0u; }

	MADCControlBitfield(uint32_t data) { u.raw = data; }
};

} // anonymous namespace

template <typename WordT>
std::array<WordT, MADCControl::write_config_size_in_words> MADCControl::encode() const
{
	MADCControlBitfield bitfield;

	bitfield.u.m.enable_iconv_amplifier = m_enable_iconv_amplifier;
	bitfield.u.m.enable_pre_amplifier = m_enable_pre_amplifier;
	bitfield.u.m.enable_continuous_sampling = m_enable_continuous_sampling;
	bitfield.u.m.enable_power_down_after_sampling = m_enable_power_down_after_sampling;
	bitfield.u.m.stop_recording = m_stop_recording;
	bitfield.u.m.start_recording = m_start_recording;
	bitfield.u.m.wake_up = m_wake_up;

	return {WordT(fisch::vx::OmnibusData(bitfield.u.raw))};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, MADCControl::write_config_size_in_words>
    MADCControl::encode<fisch::vx::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE std::array<fisch::vx::OmnibusChip, MADCControl::write_config_size_in_words>
MADCControl::encode<fisch::vx::OmnibusChip>() const;

template <typename WordT>
void MADCControl::decode(
    std::array<WordT, MADCControl::read_config_size_in_words> const& /* data */)
{}

template SYMBOL_VISIBLE void MADCControl::decode<fisch::vx::OmnibusChipOverJTAG>(
    std::array<fisch::vx::OmnibusChipOverJTAG, MADCControl::read_config_size_in_words> const& data);

template SYMBOL_VISIBLE void MADCControl::decode<fisch::vx::OmnibusChip>(
    std::array<fisch::vx::OmnibusChip, MADCControl::read_config_size_in_words> const& data);

std::ostream& operator<<(std::ostream& os, MADCControl const& config)
{
	print_words_for_each_backend<MADCControl>(os, config);
	// clang-format off
	os << "NAME\t\t\tVALUE\tDESCRIPTION" << std::endl
	   << std::boolalpha
	   << "enable_iconv_amplifier           \t" << config.m_enable_iconv_amplifier << std::endl
	   << "enable_pre_amplifier             \t" << config.m_enable_pre_amplifier << std::endl
	   << "enable_continuous_sampling       \t" << config.m_enable_continuous_sampling << std::endl
	   << "enable_power_down_after_sampling \t" << config.m_enable_power_down_after_sampling << std::endl
	   << "stop_recording                   \t" << config.m_stop_recording << std::endl
	   << "start_recording                  \t" << config.m_start_recording << std::endl
	   << "wake_up                          \t" << config.m_wake_up << std::endl;
	// clang-format on
	return os;
}

bool MADCControl::operator==(MADCControl const& other) const
{
	return (
	    m_enable_iconv_amplifier == other.get_enable_iconv_amplifier() &&
	    m_enable_pre_amplifier == other.get_enable_pre_amplifier() &&
	    m_enable_continuous_sampling == other.get_enable_continuous_sampling() &&
	    m_enable_power_down_after_sampling == other.get_enable_power_down_after_sampling() &&
	    m_stop_recording == other.get_stop_recording() &&
	    m_start_recording == other.get_start_recording() && m_wake_up == other.get_wake_up());
}

bool MADCControl::operator!=(MADCControl const& other) const
{
	return !(*this == other);
}

template <class Archive>
void MADCControl::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_enable_iconv_amplifier));
	ar(CEREAL_NVP(m_enable_pre_amplifier));
	ar(CEREAL_NVP(m_enable_continuous_sampling));
	ar(CEREAL_NVP(m_enable_power_down_after_sampling));
	ar(CEREAL_NVP(m_stop_recording));
	ar(CEREAL_NVP(m_start_recording));
	ar(CEREAL_NVP(m_wake_up));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(MADCControl)

} // namespace vx
} // namespace haldls
