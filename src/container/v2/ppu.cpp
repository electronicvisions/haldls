#include <utility>

#include "haldls/container/v2/ppu.h"

namespace haldls {
namespace container {
namespace v2 {

PPUMemoryWord::PPUMemoryWord() : PPUMemoryWord(PPUMemoryWord::Value(0)) {}

PPUMemoryWord::PPUMemoryWord(PPUMemoryWord::Value const& value) : m_value(value) {}

PPUMemoryWord::Value PPUMemoryWord::get() const
{
	return m_value;
}

void PPUMemoryWord::set(PPUMemoryWord::Value const& value)
{
	m_value = value;
}

bool PPUMemoryWord::operator==(PPUMemoryWord const& other) const
{
	return m_value == other.get();
}

bool PPUMemoryWord::operator!=(PPUMemoryWord const& other) const
{
	return !(*this == other);
}

std::array<hardware_address_type, PPUMemoryWord::config_size_in_words> PPUMemoryWord::addresses(coordinate_type const& word) const
{
	return {{static_cast<hardware_address_type>(word)}};
}

std::array<hardware_word_type, PPUMemoryWord::config_size_in_words> PPUMemoryWord::encode() const
{
	return {{static_cast<hardware_word_type>(get())}};
}

void PPUMemoryWord::decode(std::array<hardware_word_type, PPUMemoryWord::config_size_in_words> const& data)
{
	set(Value(data[0]));
}

PPUMemory::PPUMemory() : m_words() {}

PPUMemory::PPUMemory(words_type const& words) : m_words(words) {}


auto PPUMemory::get_words() const -> words_type
{
	return m_words;
}

void PPUMemory::set_words(words_type const& words)
{
	m_words = words;
}

PPUMemoryWord::Value PPUMemory::get_word(halco::hicann_dls::v2::PPUMemoryWordOnDLS const& pos) const
{
	return m_words.at(pos.value()).get();
}

void PPUMemory::set_word(
	halco::hicann_dls::v2::PPUMemoryWordOnDLS const& pos, PPUMemoryWord::Value const& value)
{
	m_words.at(pos.value()) = PPUMemoryWord(value);
}

bool PPUMemory::operator==(PPUMemory const& other) const
{
	return (m_words == other.get_words());
}

bool PPUMemory::operator!=(PPUMemory const& other) const
{
	return !(*this == other);
}

PPUControlRegister::PPUControlRegister()
	: m_inhibit_reset(false), m_force_clock_on(false), m_force_clock_off(false)
{}

bool PPUControlRegister::get_inhibit_reset() const
{
	return m_inhibit_reset;
}

void PPUControlRegister::set_inhibit_reset(bool const value)
{
	m_inhibit_reset = value;
}

bool PPUControlRegister::get_force_clock_on() const
{
	return m_force_clock_on;
}

void PPUControlRegister::set_force_clock_on(bool const value)
{
	m_force_clock_on = value;
}

bool PPUControlRegister::get_force_clock_off() const
{
	return m_force_clock_off;
}

void PPUControlRegister::set_force_clock_off(bool const value)
{
	m_force_clock_off = value;
}

bool PPUControlRegister::operator==(PPUControlRegister const& other) const
{
	return m_inhibit_reset == other.get_inhibit_reset() && m_force_clock_on == other.get_force_clock_on() &&
		   m_force_clock_off == other.get_force_clock_off();
}

bool PPUControlRegister::operator!=(PPUControlRegister const& other) const
{
	return !(*this == other);
}

namespace {

struct PPUControlRegisterBitfield {
	union {
		hardware_word_type raw;
		// clang-format off
		struct __attribute__((packed)) {
			hardware_word_type inhibit_reset   :  1; // 0
			hardware_word_type force_clock_on  :  1; // 1
			hardware_word_type force_clock_off :  1; // 2
			hardware_word_type                 : 29; // 3-32
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	PPUControlRegisterBitfield() {
		u.raw = 0u;
	}

	PPUControlRegisterBitfield(hardware_word_type data) {
		u.raw = data & 0b0111ul;
	}
};

} // namespace

std::array<hardware_address_type, PPUControlRegister::config_size_in_words> PPUControlRegister::addresses(coordinate_type const& /*unique*/) const
{
	return {{0x00020000ul}};
}

std::array<hardware_word_type, PPUControlRegister::config_size_in_words> PPUControlRegister::encode() const
{
	PPUControlRegisterBitfield bitfield;
	bitfield.u.m.inhibit_reset = m_inhibit_reset;
	bitfield.u.m.force_clock_on = m_force_clock_on;
	bitfield.u.m.force_clock_off = m_force_clock_off;
	return {{bitfield.u.raw}};
}

void PPUControlRegister::decode(std::array<hardware_word_type, PPUControlRegister::config_size_in_words> const& data)
{
	PPUControlRegisterBitfield bitfield(data[0]);
	m_inhibit_reset = bitfield.u.m.inhibit_reset;
	m_force_clock_on = bitfield.u.m.force_clock_on;
	m_force_clock_off = bitfield.u.m.force_clock_off;
}

PPUStatusRegister::PPUStatusRegister() : m_sleep(false) {}

bool PPUStatusRegister::get_sleep() const
{
	return m_sleep;
}

bool PPUStatusRegister::operator==(PPUStatusRegister const& other) const
{
	return m_sleep == other.m_sleep;
}

bool PPUStatusRegister::operator!=(PPUStatusRegister const& other) const
{
	return !(*this == other);
}

std::array<hardware_address_type, PPUStatusRegister::config_size_in_words> PPUStatusRegister::addresses(coordinate_type const& /*unique*/) const
{
	return {{0x00020001ul}};
}

std::array<hardware_word_type, PPUStatusRegister::config_size_in_words> PPUStatusRegister::encode() const
{
	return {{m_sleep}};
}

void PPUStatusRegister::decode(std::array<hardware_word_type, PPUStatusRegister::config_size_in_words> const& data)
{
	m_sleep = bool(data[0]);
}

} // namespace v2
} // namespace container
} // namespace haldls
