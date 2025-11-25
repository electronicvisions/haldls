#include "haldls/vx/routing_table_entry.h"

#include "fisch/vx/word_access/type/jtag.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/expand_word.h"
#include "haldls/vx/container.tcc"
#include "haldls/vx/omnibus_constants.h"

namespace haldls {
namespace vx {

OutputRoutingTableEntry::OutputRoutingTableEntry() : m_enable(false), m_label() {}

bool OutputRoutingTableEntry::get_enable() const
{
	return m_enable;
}

void OutputRoutingTableEntry::set_enable(bool const value)
{
	m_enable = value;
}

OutputRoutingTableEntry::Label OutputRoutingTableEntry::get_label() const
{
	return m_label;
}

void OutputRoutingTableEntry::set_label(OutputRoutingTableEntry::Label const& value)
{
	m_label = value;
}

bool OutputRoutingTableEntry::operator==(OutputRoutingTableEntry const& other) const
{
	return (m_enable == other.m_enable) && (m_label == other.m_label);
}

bool OutputRoutingTableEntry::operator!=(OutputRoutingTableEntry const& other) const
{
	return !(*this == other);
}

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, OutputRoutingTableEntry const& config)
{
	std::stringstream ss;
	ss << "OutputRoutingTableEntry(enable: " << std::boolalpha << config.m_enable
	   << ", label: " << config.m_label << ")";
	return (os << ss.str());
}
#endif

template <typename AddressT>
std::array<AddressT, OutputRoutingTableEntry::config_size_in_words>
OutputRoutingTableEntry::addresses(coordinate_type const& coordinate)
{
	uint32_t const base = output_routing_table_base_address + (coordinate * config_size_in_words);
	return {AddressT(base)};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, OutputRoutingTableEntry::config_size_in_words>
    OutputRoutingTableEntry::addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& coord);

namespace {

struct OutputRoutingTableEntryBitfield
{
	union
	{
		std::array<uint32_t, OutputRoutingTableEntry::config_size_in_words> raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t enable        :  1;
			uint32_t label         : 15;
			uint32_t /*unused*/    : 16;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	OutputRoutingTableEntryBitfield()
	{
		u.raw = {0u};
	}

	OutputRoutingTableEntryBitfield(
	    std::array<uint32_t, OutputRoutingTableEntry::config_size_in_words> data)
	{
		u.raw = data;
	}
};

} // namespace

template <typename WordT>
std::array<WordT, OutputRoutingTableEntry::config_size_in_words> OutputRoutingTableEntry::encode()
    const
{
	OutputRoutingTableEntryBitfield bitfield;
	bitfield.u.m.enable = m_enable;
	bitfield.u.m.label = m_label;

	return {WordT(bitfield.u.raw[0])};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, OutputRoutingTableEntry::config_size_in_words>
    OutputRoutingTableEntry::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void OutputRoutingTableEntry::decode(
    std::array<WordT, OutputRoutingTableEntry::config_size_in_words> const& data)
{
	OutputRoutingTableEntryBitfield bitfield;
	bitfield.u.raw[0] = data.at(0);

	m_enable = bitfield.u.m.enable;
	m_label = Label(bitfield.u.m.label);
}

template SYMBOL_VISIBLE void OutputRoutingTableEntry::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        OutputRoutingTableEntry::config_size_in_words> const& data);


InputRoutingTableEntry::InputRoutingTableEntry() : m_enable(false), m_label() {}

bool InputRoutingTableEntry::get_enable() const
{
	return m_enable;
}

void InputRoutingTableEntry::set_enable(bool const value)
{
	m_enable = value;
}

InputRoutingTableEntry::Label InputRoutingTableEntry::get_label() const
{
	return m_label;
}

void InputRoutingTableEntry::set_label(InputRoutingTableEntry::Label const& value)
{
	m_label = value;
}

bool InputRoutingTableEntry::operator==(InputRoutingTableEntry const& other) const
{
	return (m_enable == other.m_enable) && (m_label == other.m_label);
}

bool InputRoutingTableEntry::operator!=(InputRoutingTableEntry const& other) const
{
	return !(*this == other);
}

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, InputRoutingTableEntry const& config)
{
	std::stringstream ss;
	ss << "InputRoutingTableEntry(enable: " << std::boolalpha << config.m_enable
	   << ", label: " << config.m_label << ")";
	return (os << ss.str());
}
#endif

template <typename AddressT>
std::array<AddressT, InputRoutingTableEntry::config_size_in_words>
InputRoutingTableEntry::addresses(coordinate_type const& coordinate)
{
	uint32_t const base = input_routing_table_base_address + (coordinate * config_size_in_words);
	return {AddressT(base)};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, InputRoutingTableEntry::config_size_in_words>
    InputRoutingTableEntry::addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& coord);

namespace {

struct InputRoutingTableEntryBitfield
{
	union
	{
		std::array<uint32_t, InputRoutingTableEntry::config_size_in_words> raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t enable        :  1;
			uint32_t label         : 16;
			uint32_t /*unused*/    : 15;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	InputRoutingTableEntryBitfield()
	{
		u.raw = {0u};
	}

	InputRoutingTableEntryBitfield(
	    std::array<uint32_t, InputRoutingTableEntry::config_size_in_words> data)
	{
		u.raw = data;
	}
};

} // namespace

template <typename WordT>
std::array<WordT, InputRoutingTableEntry::config_size_in_words> InputRoutingTableEntry::encode()
    const
{
	InputRoutingTableEntryBitfield bitfield;
	bitfield.u.m.enable = m_enable;
	bitfield.u.m.label = m_label;

	return {WordT(bitfield.u.raw[0])};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, InputRoutingTableEntry::config_size_in_words>
    InputRoutingTableEntry::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void InputRoutingTableEntry::decode(
    std::array<WordT, InputRoutingTableEntry::config_size_in_words> const& data)
{
	InputRoutingTableEntryBitfield bitfield;
	bitfield.u.raw[0] = data.at(0);

	m_enable = bitfield.u.m.enable;
	m_label = Label(bitfield.u.m.label);
}

template SYMBOL_VISIBLE void InputRoutingTableEntry::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        InputRoutingTableEntry::config_size_in_words> const& data);


} // namespace vx
} // namespace haldls

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::OutputRoutingTableEntry)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::InputRoutingTableEntry)
