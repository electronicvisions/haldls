#include <cctype>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <utility>
#include <netinet/in.h>

#include "haldls/v2/ppu.h"
#include "hate/math.h"

#include <cereal/types/array.hpp>
#include <cereal/types/vector.hpp>
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/cerealization.h"

namespace haldls {
namespace v2 {

PPUMemoryWord::PPUMemoryWord() : PPUMemoryWord(PPUMemoryWord::Value(0)) {}

PPUMemoryWord::PPUMemoryWord(PPUMemoryWord::Value const& value) : m_value(value) {}

PPUMemoryWord::Value PPUMemoryWord::get_value() const
{
	return m_value;
}

void PPUMemoryWord::set_value(PPUMemoryWord::Value const& value)
{
	m_value = value;
}

bool PPUMemoryWord::operator==(PPUMemoryWord const& other) const
{
	return m_value == other.get_value();
}

bool PPUMemoryWord::operator!=(PPUMemoryWord const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, PPUMemoryWord const& pmw)
{
	using namespace hate::math;
	uint32_t w = static_cast<uint32_t>(pmw.get_value());
	std::stringstream out;
	out << std::showbase << std::internal << std::setfill('0') << std::hex
	    << std::setw(round_up_integer_division(num_bits(PPUMemoryWord::Value::max), 4)) << w;
	os << out.str();
	return os;
}

std::array<hardware_address_type, PPUMemoryWord::config_size_in_words> PPUMemoryWord::addresses(coordinate_type const& word) const
{
	return {{static_cast<hardware_address_type>(word)}};
}

std::array<hardware_word_type, PPUMemoryWord::config_size_in_words> PPUMemoryWord::encode() const
{
	return {{static_cast<hardware_word_type>(get_value())}};
}

void PPUMemoryWord::decode(std::array<hardware_word_type, PPUMemoryWord::config_size_in_words> const& data)
{
	set_value(Value(data[0]));
}

template <class Archive>
void PPUMemoryWord::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(PPUMemoryWord)

PPUMemoryBlock::PPUMemoryBlock() : m_words(halco::hicann_dls::v2::PPUMemoryWordOnDLS::size) {}

PPUMemoryBlock::PPUMemoryBlock(size_type const& size) : m_words(size.value())
{}

PPUMemoryBlock::words_type const& PPUMemoryBlock::get_words() const
{
	return m_words;
}

void PPUMemoryBlock::set_words(words_type const& words)
{
	if (words.size() != size()) {
		std::stringstream ss;
		ss << "given vector size(" << words.size() << ") does not match container size(" << size()
		   << ")";
		throw std::range_error(ss.str());
	}
	m_words = words;
}

PPUMemoryWord& PPUMemoryBlock::at(size_t index)
{
	if (index >= size()) {
		std::stringstream ss;
		ss << "given index(" << index << ") not in range(0 -> " << size() - 1 << ")";
		throw std::out_of_range(ss.str());
	}
	return m_words.at(index);
}

PPUMemoryWord const& PPUMemoryBlock::at(size_t index) const
{
	if (index >= size()) {
		std::stringstream ss;
		ss << "given index(" << index << ") not in range(0 -> " << size() - 1 << ")";
		throw std::out_of_range(ss.str());
	}
	return m_words.at(index);
}

PPUMemoryWord& PPUMemoryBlock::operator[](size_t index)
{
	return at(index);
}

PPUMemoryWord const& PPUMemoryBlock::operator[](size_t index) const
{
	return at(index);
}

PPUMemoryBlock PPUMemoryBlock::get_subblock(size_t begin, size_type length) const
{
	if (begin + length > size()) {
		std::stringstream ss;
		ss << "subblock from index " << begin << " of size " << length
		   << " larger than block size of " << size();
		throw std::out_of_range(ss.str());
	}
	PPUMemoryBlock subblock(length);
	for (size_t i = 0; i < length; ++i) {
		subblock.m_words.at(i) = m_words.at(i + begin);
	}
	return subblock;
}

void PPUMemoryBlock::set_subblock(size_t begin, PPUMemoryBlock const& subblock)
{
	if (begin + subblock.size() > size()) {
		std::stringstream ss;
		ss << "subblock from index " << begin << " of size " << subblock.size()
		   << " larger than block size of " << size();
		throw std::out_of_range(ss.str());
	}
	for (size_t i = 0; i < subblock.size(); ++i) {
		m_words.at(i + begin) = subblock.m_words.at(i);
	}
}

halco::hicann_dls::v2::PPUMemoryBlockSize PPUMemoryBlock::size() const
{
	return halco::hicann_dls::v2::PPUMemoryBlockSize(m_words.size());
}

bool PPUMemoryBlock::operator==(PPUMemoryBlock const& other) const
{
	return (m_words == other.get_words());
}

bool PPUMemoryBlock::operator!=(PPUMemoryBlock const& other) const
{
	return !(*this == other);
}

std::string PPUMemoryBlock::to_string() const
{
	std::stringstream ss;
	for (auto x : m_words) {
		// access single characters in word
		// endianess-flip due to different endianess on PPU
		uint32_t const w = ntohl(static_cast<uint32_t>((x.get_value())));
		char const* c = reinterpret_cast<char const*>(&w);
		for (size_t i = 0; i < sizeof(uint32_t); ++i) {
			// discard non-printable characters
			if (isprint(c[i]) or isspace(c[i])) {
				ss << c[i];
			}
		}
	}
	return ss.str();
}

template <class Archive>
void PPUMemoryBlock::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_words));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(PPUMemoryBlock)

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
	return m_words.at(pos.value()).get_value();
}

void PPUMemory::set_word(
	halco::hicann_dls::v2::PPUMemoryWordOnDLS const& pos, PPUMemoryWord::Value const& value)
{
	m_words.at(pos.value()) = PPUMemoryWord(value);
}

void PPUMemory::load_from_file(std::string const& filename)
{
	m_words.fill(PPUMemoryWord(PPUMemoryWord::Value(0)));

	std::vector<char> program_bytes;
	// read file
	try {
		std::ifstream in;
		in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		in.open(filename, std::ios::binary);

		std::istreambuf_iterator iter(in);
		std::copy(iter, std::istreambuf_iterator<char>(), std::back_inserter(program_bytes));
	} catch (std::ifstream::failure const& e) {
		std::stringstream ss;
		ss << "Error reading from file " << filename << ": " << e.what() << ".";
		throw std::runtime_error(ss.str());
	}

	// pad to multiple of word size
	while ((program_bytes.size() % sizeof(uint32_t)) != 0) {
		program_bytes.push_back(0);
	}

	if (program_bytes.size() > m_words.size() * sizeof(uint32_t)) {
		throw std::runtime_error("PPU program to be loaded too large for memory bounds.");
	}

	// convert to words
	uint32_t* iter = reinterpret_cast<uint32_t*>(program_bytes.data());
	std::vector<uint32_t> words(iter, iter + program_bytes.size() / sizeof(uint32_t));

	// correct endianness
	std::transform(words.begin(), words.end(), words.begin(), ntohl);

	// convert to PPUMemoryWords
	std::vector<PPUMemoryWord> ppu_memory_words(words.size());
	std::transform(words.begin(), words.end(), ppu_memory_words.begin(), [](uint32_t const x) {
		return PPUMemoryWord(PPUMemoryWord::Value(x));
	});

	std::copy(ppu_memory_words.cbegin(), ppu_memory_words.cend(), m_words.begin());
}

bool PPUMemory::operator==(PPUMemory const& other) const
{
	return (m_words == other.get_words());
}

bool PPUMemory::operator!=(PPUMemory const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, PPUMemory const& pm)
{
	int const words_per_line = 4;
	auto const words = pm.get_words();
	std::stringstream out;
	for (unsigned int i = 0; i < words.size(); i += words_per_line) {
		// break line and print address
		std::stringstream addr;
		addr << std::setfill('0') << std::hex << std::setw(6);
		addr << (i * 4) << ": ";
		// print halfwords in hex
		std::stringstream halfwords;
		halfwords << std::hex << std::internal << std::setfill('0');
		for (unsigned int j = 0; ((j < words_per_line) && (i + j < words.size())); j++) {
			uint32_t word = static_cast<uint32_t>(words[i + j].get_value());
			halfwords << std::setw(4) << (word >> 16) << " " << std::setw(4) << (word & 0xffff)
			          << " ";
		}
		// print as ascii
		std::stringstream ascii;
		for (unsigned int j = 0; ((j < words_per_line) && (i + j < words.size())); j++) {
			uint32_t word = ntohl(static_cast<uint32_t>(words[i + j].get_value()));
			char* chars = reinterpret_cast<char*>(&word);
			for (int k = 0; k < 4; k++) {
				ascii << (isprint(chars[k]) ? chars[k] : '.');
			}
		}
		out << addr.str() << halfwords.str() << ascii.str() << std::endl;
	}
	os << out.str();
	return os;
}

template <class Archive>
void PPUMemory::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_words));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(PPUMemory)

std::ostream& operator<<(std::ostream& os, PPUMemoryBlock const& pmb)
{
	int const words_per_line = 4;
	auto const words = pmb.get_words();
	std::stringstream out;
	for (unsigned int i = 0; i < words.size(); i += words_per_line) {
		// print halfwords in hex
		std::stringstream halfwords;
		halfwords << std::hex << std::internal << std::setfill('0');
		for (unsigned int j = 0; ((j < words_per_line) && (i + j < words.size())); j++) {
			uint32_t word = static_cast<uint32_t>(words[i + j].get_value());
			halfwords << std::setw(4) << (word >> 16) << " " << std::setw(4) << (word & 0xffff)
			          << " ";
		}
		// print as ascii
		std::stringstream ascii;
		for (unsigned int j = 0; ((j < words_per_line) && (i + j < words.size())); j++) {
			uint32_t word = ntohl(static_cast<uint32_t>(words[i + j].get_value()));
			char* chars = reinterpret_cast<char*>(&word);
			for (int k = 0; k < 4; k++) {
				ascii << (isprint(chars[k]) ? chars[k] : '.');
			}
		}
		out << halfwords.str() << ascii.str() << std::endl;
	}
	os << out.str();
	return os;
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

template <class Archive>
void PPUControlRegister::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_inhibit_reset));
	ar(CEREAL_NVP(m_force_clock_on));
	ar(CEREAL_NVP(m_force_clock_off));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(PPUControlRegister)

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

template <class Archive>
void PPUStatusRegister::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_sleep));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(PPUStatusRegister)


} // namespace v2
} // namespace haldls
