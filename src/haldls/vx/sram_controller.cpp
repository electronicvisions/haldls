#include <cctype>
#include <iomanip>
#include <utility>

#include "fisch/vx/word_access/type/jtag.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/sram_controller.h"

namespace haldls::vx::detail {

SRAMTimingConfig::SRAMTimingConfig() : m_read_delay(), m_address_setup_time(), m_write_width() {}

typename SRAMTimingConfig::ReadDelay SRAMTimingConfig::get_read_delay() const
{
	return m_read_delay;
}

void SRAMTimingConfig::set_read_delay(ReadDelay const value)
{
	m_read_delay = value;
}

typename SRAMTimingConfig::AddressSetupTime SRAMTimingConfig::get_address_setup_time() const
{
	return m_address_setup_time;
}

void SRAMTimingConfig::set_address_setup_time(AddressSetupTime const value)
{
	m_address_setup_time = value;
}

typename SRAMTimingConfig::WriteWidth SRAMTimingConfig::get_write_width() const
{
	return m_write_width;
}

void SRAMTimingConfig::set_write_width(WriteWidth const value)
{
	m_write_width = value;
}

bool SRAMTimingConfig::operator==(SRAMTimingConfig const& other) const
{
	return m_read_delay == other.m_read_delay &&
	       m_address_setup_time == other.m_address_setup_time &&
	       m_write_width == other.m_write_width;
}

bool SRAMTimingConfig::operator!=(SRAMTimingConfig const& other) const
{
	return !(*this == other);
}

namespace {

struct SRAMTimingConfigBitfield
{
	union
	{
		std::array<uint32_t, SRAMTimingConfig::config_size_in_words> words;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t read_delay         :  8;
			uint32_t /* unused */       : 24;
			uint32_t address_setup_time :  4;
			uint32_t write_width        :  4;
			uint32_t /* unused */       : 24;
		} m;
		// clang-format on
		static_assert(sizeof(words) == sizeof(m), "sizes of union types should match");
	} u;

	SRAMTimingConfigBitfield()
	{
		u.words = {{0, 0}};
	}

	SRAMTimingConfigBitfield(std::array<uint32_t, SRAMTimingConfig::config_size_in_words> data)
	{
		u.words[0] = data[0];
		u.words[1] = data[1];
	}
};

} // anonymous namespace

template <typename WordT>
std::array<WordT, SRAMTimingConfig::config_size_in_words> SRAMTimingConfig::encode() const
{
	SRAMTimingConfigBitfield bitfield;
	bitfield.u.m.address_setup_time = m_address_setup_time;
	bitfield.u.m.write_width = m_write_width;
	bitfield.u.m.read_delay = m_read_delay;

	std::array<WordT, SRAMTimingConfig::config_size_in_words> data;
	std::transform(
	    bitfield.u.words.begin(), bitfield.u.words.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(w); });
	return data;
}

template SYMBOL_VISIBLE std::
    array<fisch::vx::word_access_type::OmnibusChipOverJTAG, SRAMTimingConfig::config_size_in_words>
    SRAMTimingConfig::encode() const;
template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, SRAMTimingConfig::config_size_in_words>
    SRAMTimingConfig::encode() const;

template <typename WordT>
void SRAMTimingConfig::decode(std::array<WordT, SRAMTimingConfig::config_size_in_words> const& data)
{
	std::array<uint32_t, SRAMTimingConfig::config_size_in_words> raw_data;
	std::transform(data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w; });
	SRAMTimingConfigBitfield bitfield(raw_data);
	m_address_setup_time = AddressSetupTime(bitfield.u.m.address_setup_time);
	m_write_width = WriteWidth(bitfield.u.m.write_width);
	m_read_delay = ReadDelay(bitfield.u.m.read_delay);
}

template SYMBOL_VISIBLE void SRAMTimingConfig::decode(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        SRAMTimingConfig::config_size_in_words> const& data);
template SYMBOL_VISIBLE void SRAMTimingConfig::decode(
    std::array<fisch::vx::word_access_type::Omnibus, SRAMTimingConfig::config_size_in_words> const&
        data);

} // namespace haldls::vx::detail
