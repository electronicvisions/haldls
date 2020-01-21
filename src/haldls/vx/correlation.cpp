#include <cctype>
#include <iomanip>
#include <utility>

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "haldls/cerealization.h"
#include "haldls/vx/correlation.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/print.h"

namespace haldls {
namespace vx {

CommonCorrelationConfig::CommonCorrelationConfig() :
    m_sense_delay(),
    m_reset_duration(),
    m_reset_fall_time(),
    m_reset_mode(false)
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

bool CommonCorrelationConfig::get_reset_mode() const
{
	return m_reset_mode;
}

void CommonCorrelationConfig::set_reset_mode(bool const value)
{
	m_reset_mode = value;
}

bool CommonCorrelationConfig::operator==(CommonCorrelationConfig const& other) const
{
	return m_sense_delay == other.m_sense_delay && m_reset_duration == other.m_reset_duration &&
	       m_reset_fall_time == other.m_reset_fall_time && m_reset_mode == other.m_reset_mode;
}

bool CommonCorrelationConfig::operator!=(CommonCorrelationConfig const& other) const
{
	return !(*this == other);
}

template <typename Archive>
void CommonCorrelationConfig::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_sense_delay));
	ar(CEREAL_NVP(m_reset_duration));
	ar(CEREAL_NVP(m_reset_fall_time));
	ar(CEREAL_NVP(m_reset_mode));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CommonCorrelationConfig)

namespace {

struct CommonCorrelationConfigBitfield
{
	union
	{
		std::array<uint32_t, CommonCorrelationConfig::config_size_in_words> words;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t sense_delay      : 12;  // base address
			uint32_t /* unused */     : 20;
			uint32_t reset_duration   : 12;  // base + 1
			uint32_t /* unused */     : 20;
			uint32_t reset_fall_time  : 12;  // base + 2
			uint32_t /* unused */     : 20;
			uint32_t reset_mode       :  1;  // base + 3
			uint32_t /* unused */     : 31;
		} m;
		// clang-format on
		static_assert(sizeof(words) == sizeof(m), "sizes of union types should match");
	} u;

	CommonCorrelationConfigBitfield() { u.words = {{0, 0, 0, 0}}; }

	CommonCorrelationConfigBitfield(
	    std::array<uint32_t, CommonCorrelationConfig::config_size_in_words> data)
	{
		u.words[0] = data[0] & 0x0fff;
		u.words[1] = data[1] & 0x0fff;
		u.words[2] = data[2] & 0x0fff;
		u.words[3] = data[3] & 0x0001;
	}
};

} // anonymous namespace

HALDLS_VX_DEFAULT_OSTREAM_OP(CommonCorrelationConfig)

template <typename AddressT>
std::array<AddressT, CommonCorrelationConfig::config_size_in_words>
CommonCorrelationConfig::addresses(coordinate_type const& coord) const
{
	return {AddressT(correlation_config_base_addresses.at(coord.toEnum())),
	        AddressT(correlation_config_base_addresses.at(coord.toEnum()) + 1),
	        AddressT(correlation_config_base_addresses.at(coord.toEnum()) + 2),
	        AddressT(correlation_config_base_addresses.at(coord.toEnum()) + 3)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CommonCorrelationConfig::config_size_in_words>
CommonCorrelationConfig::addresses(coordinate_type const& coord) const;
template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusChipAddress, CommonCorrelationConfig::config_size_in_words>
    CommonCorrelationConfig::addresses(coordinate_type const& coord) const;

template <typename WordT>
std::array<WordT, CommonCorrelationConfig::config_size_in_words> CommonCorrelationConfig::encode()
    const
{
	CommonCorrelationConfigBitfield bitfield;
	bitfield.u.m.sense_delay = m_sense_delay;
	bitfield.u.m.reset_duration = m_reset_duration;
	bitfield.u.m.reset_fall_time = m_reset_fall_time;
	bitfield.u.m.reset_mode = m_reset_mode;

	std::array<WordT, CommonCorrelationConfig::config_size_in_words> data;
	std::transform(
	    bitfield.u.words.begin(), bitfield.u.words.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(fisch::vx::OmnibusData(w)); });
	return data;
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, CommonCorrelationConfig::config_size_in_words>
    CommonCorrelationConfig::encode() const;
template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChip, CommonCorrelationConfig::config_size_in_words>
    CommonCorrelationConfig::encode() const;

template <typename WordT>
void CommonCorrelationConfig::decode(
    std::array<WordT, CommonCorrelationConfig::config_size_in_words> const& data)
{
	std::array<uint32_t, CommonCorrelationConfig::config_size_in_words> raw_data;
	std::transform(
	    data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w.get(); });
	CommonCorrelationConfigBitfield bitfield(raw_data);
	m_sense_delay = SenseDelay(bitfield.u.m.sense_delay);
	m_reset_duration = ResetDuration(bitfield.u.m.reset_duration);
	m_reset_fall_time = ResetFallTime(bitfield.u.m.reset_fall_time);
	m_reset_mode = bitfield.u.m.reset_mode;
}

template SYMBOL_VISIBLE void CommonCorrelationConfig::decode(
    std::array<fisch::vx::OmnibusChipOverJTAG, CommonCorrelationConfig::config_size_in_words> const&
        data);
template SYMBOL_VISIBLE void CommonCorrelationConfig::decode(
    std::array<fisch::vx::OmnibusChip, CommonCorrelationConfig::config_size_in_words> const& data);

} // namespace vx
} // namespace haldls
