#include "haldls/vx/v2/correlation.h"

#include "fisch/vx/word_access/type/jtag.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/cerealization.tcc"
#include "haldls/vx/omnibus_constants.h"

namespace haldls::vx::v2 {

CommonCorrelationConfig::CommonCorrelationConfig() :
    m_sense_delay(),
    m_reset_duration(),
    m_reset_fall_time(),
    m_reset_mode(ResetMode::normal),
    m_cadc_v_offset_assignment(
        {halco::hicann_dls::vx::v2::CapMemBlockOnHemisphere::left,
         halco::hicann_dls::vx::v2::CapMemBlockOnHemisphere::right})
{}


typename CommonCorrelationConfig::SenseDelay CommonCorrelationConfig::get_sense_delay() const
{
	return m_sense_delay;
}

void CommonCorrelationConfig::set_sense_delay(SenseDelay const value)
{
	m_sense_delay = value;
}

typename CommonCorrelationConfig::ResetDuration CommonCorrelationConfig::get_reset_duration() const
{
	return m_reset_duration;
}

void CommonCorrelationConfig::set_reset_duration(ResetDuration const value)
{
	m_reset_duration = value;
}

typename CommonCorrelationConfig::ResetFallTime CommonCorrelationConfig::get_reset_fall_time() const
{
	return m_reset_fall_time;
}

void CommonCorrelationConfig::set_reset_fall_time(ResetFallTime const value)
{
	m_reset_fall_time = value;
}

typename CommonCorrelationConfig::ResetMode CommonCorrelationConfig::get_reset_mode() const
{
	return m_reset_mode;
}

void CommonCorrelationConfig::set_reset_mode(ResetMode const value)
{
	m_reset_mode = value;
}

typename CommonCorrelationConfig::RampOffsets const&
CommonCorrelationConfig::get_cadc_v_offset_assignment() const
{
	return m_cadc_v_offset_assignment;
}

void CommonCorrelationConfig::set_cadc_v_offset_assignment(RampOffsets const& value)
{
	m_cadc_v_offset_assignment = value;
}

bool CommonCorrelationConfig::operator==(CommonCorrelationConfig const& other) const
{
	return m_sense_delay == other.m_sense_delay && m_reset_duration == other.m_reset_duration &&
	       m_reset_fall_time == other.m_reset_fall_time && m_reset_mode == other.m_reset_mode &&
	       m_cadc_v_offset_assignment == other.m_cadc_v_offset_assignment;
}

bool CommonCorrelationConfig::operator!=(CommonCorrelationConfig const& other) const
{
	return !(*this == other);
}

template <typename Archive>
void CommonCorrelationConfig::serialize(Archive& ar, std::uint32_t const version)
{
	ar(CEREAL_NVP(m_sense_delay));
	ar(CEREAL_NVP(m_reset_duration));
	ar(CEREAL_NVP(m_reset_fall_time));
	if (version == 0) {
		bool tmp = static_cast<bool>(m_reset_mode);
		ar(CEREAL_NVP_("m_reset_mode", tmp));
		m_reset_mode = ResetMode(static_cast<uint32_t>(tmp));
	} else {
		ar(CEREAL_NVP(m_reset_mode));
	}
	if (version > 0) {
		ar(CEREAL_NVP(m_cadc_v_offset_assignment));
	}
}

namespace {

struct CommonCorrelationConfigBitfield
{
	union
	{
		std::array<uint32_t, CommonCorrelationConfig::config_size_in_words> words;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t sense_delay          : 12;  // base address
			uint32_t /* unused */         : 20;
			uint32_t reset_duration       : 12;  // base + 1
			uint32_t /* unused */         : 20;
			uint32_t reset_fall_time      : 12;  // base + 2
			uint32_t /* unused */         : 20;
			uint32_t reset_mode           : 1;   // base + 3
			uint32_t select_voltage_right : 1;
			uint32_t select_voltage_left  : 1;
			uint32_t /* unused */         : 29;
		} m;
		// clang-format on
		static_assert(sizeof(words) == sizeof(m), "sizes of union types should match");
	} u;

	CommonCorrelationConfigBitfield()
	{
		u.words = {{0, 0, 0, 0}};
	}

	CommonCorrelationConfigBitfield(
	    std::array<uint32_t, CommonCorrelationConfig::config_size_in_words> data)
	{
		u.words[0] = data[0] & 0x0fff;
		u.words[1] = data[1] & 0x0fff;
		u.words[2] = data[2] & 0x0fff;
		u.words[3] = data[3] & 0x0007;
	}
};

} // anonymous namespace

std::ostream& operator<<(std::ostream& os, CommonCorrelationConfig const& config)
{
	std::stringstream ss;
	ss << "CommonCorrelationConfig(" << std::endl;
	ss << "\tSense delay:              \t" << config.m_sense_delay << std::endl;
	ss << "\tReset duration:           \t" << config.m_reset_duration << std::endl;
	ss << "\tReset fall time:          \t" << config.m_reset_fall_time << std::endl;
	ss << "\tReset mode:               \t" << config.m_reset_mode << std::endl;
	ss << "\tCADC offset voltage left: \t"
	   << config
	          .m_cadc_v_offset_assignment[halco::hicann_dls::vx::v2::CapMemBlockOnHemisphere::left]
	   << std::endl;
	;
	ss << "\tCADC offset voltage right: \t"
	   << config
	          .m_cadc_v_offset_assignment[halco::hicann_dls::vx::v2::CapMemBlockOnHemisphere::right]
	   << std::endl;
	ss << ")" << std::endl;

	return (os << ss.str());
}

std::ostream& operator<<(std::ostream& os, CommonCorrelationConfig::ResetMode const& config)
{
	return (
	    os
	    << (config == CommonCorrelationConfig::ResetMode::normal ? "normal" : "auto_calibrating"));
}

template <typename AddressT>
std::array<AddressT, CommonCorrelationConfig::config_size_in_words>
CommonCorrelationConfig::addresses(coordinate_type const& coord)
{
	return {
	    AddressT(correlation_config_base_addresses.at(coord.toEnum())),
	    AddressT(correlation_config_base_addresses.at(coord.toEnum()) + 1),
	    AddressT(correlation_config_base_addresses.at(coord.toEnum()) + 2),
	    AddressT(correlation_config_base_addresses.at(coord.toEnum()) + 3)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CommonCorrelationConfig::config_size_in_words>
CommonCorrelationConfig::addresses(coordinate_type const& coord);
template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, CommonCorrelationConfig::config_size_in_words>
    CommonCorrelationConfig::addresses(coordinate_type const& coord);

template <typename WordT>
std::array<WordT, CommonCorrelationConfig::config_size_in_words> CommonCorrelationConfig::encode()
    const
{
	CommonCorrelationConfigBitfield bitfield;
	bitfield.u.m.sense_delay = m_sense_delay;
	bitfield.u.m.reset_duration = m_reset_duration;
	bitfield.u.m.reset_fall_time = m_reset_fall_time;
	bitfield.u.m.reset_mode = static_cast<uint32_t>(m_reset_mode);

	bitfield.u.m.select_voltage_left =
	    m_cadc_v_offset_assignment[halco::hicann_dls::vx::v2::CapMemBlockOnHemisphere::left];
	if (m_cadc_v_offset_assignment[halco::hicann_dls::vx::v2::CapMemBlockOnHemisphere::right] ==
	    halco::hicann_dls::vx::v2::CapMemBlockOnHemisphere::right)
		bitfield.u.m.select_voltage_right = 0b0;
	else
		bitfield.u.m.select_voltage_right = 0b1;

	std::array<WordT, CommonCorrelationConfig::config_size_in_words> data;
	std::transform(
	    bitfield.u.words.begin(), bitfield.u.words.end(), data.begin(),
	    [](uint32_t const& w) { return WordT(w); });
	return data;
}

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    CommonCorrelationConfig::config_size_in_words>
CommonCorrelationConfig::encode() const;
template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, CommonCorrelationConfig::config_size_in_words>
    CommonCorrelationConfig::encode() const;

template <typename WordT>
void CommonCorrelationConfig::decode(
    std::array<WordT, CommonCorrelationConfig::config_size_in_words> const& data)
{
	std::array<uint32_t, CommonCorrelationConfig::config_size_in_words> raw_data;
	std::transform(data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w; });
	CommonCorrelationConfigBitfield bitfield(raw_data);
	m_sense_delay = SenseDelay(bitfield.u.m.sense_delay);
	m_reset_duration = ResetDuration(bitfield.u.m.reset_duration);
	m_reset_fall_time = ResetFallTime(bitfield.u.m.reset_fall_time);
	m_reset_mode = ResetMode(bitfield.u.m.reset_mode);

	m_cadc_v_offset_assignment[halco::hicann_dls::vx::v2::CapMemBlockOnHemisphere::left] =
	    halco::hicann_dls::vx::v2::CapMemBlockOnHemisphere(bitfield.u.m.select_voltage_left);
	if (bitfield.u.m.select_voltage_right)
		m_cadc_v_offset_assignment[halco::hicann_dls::vx::v2::CapMemBlockOnHemisphere::right] =
		    halco::hicann_dls::vx::v2::CapMemBlockOnHemisphere::left;
	else
		m_cadc_v_offset_assignment[halco::hicann_dls::vx::v2::CapMemBlockOnHemisphere::right] =
		    halco::hicann_dls::vx::v2::CapMemBlockOnHemisphere::right;
}

template SYMBOL_VISIBLE void CommonCorrelationConfig::decode(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        CommonCorrelationConfig::config_size_in_words> const& data);
template SYMBOL_VISIBLE void CommonCorrelationConfig::decode(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        CommonCorrelationConfig::config_size_in_words> const& data);

} // namespace haldls::vx::v2

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::v2::CommonCorrelationConfig)
