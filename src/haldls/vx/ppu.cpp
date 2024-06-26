#include "haldls/vx/ppu.h"

#include <cctype>
#include <fstream>
#include <iomanip>
#include <utility>

#include "fisch/vx/word_access/type/jtag.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/expand_word.h"
#include "haldls/vx/container.tcc"
#include "haldls/vx/omnibus_constants.h"
#include "hate/indent.h"
#include "hate/math.h"

#ifndef __ppu__
#include <netinet/in.h>
#else
unsigned long ntohl(unsigned long const value)
{
	return value;
}
#endif

namespace haldls {
namespace vx {

PPUMemoryWord::PPUMemoryWord(PPUMemoryWord::Value const value) : m_value(value) {}

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

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, PPUMemoryWord const& pmw)
{
	os << "PPUMemoryWord(";
	using namespace hate::math;
	auto const w = static_cast<PPUMemoryWord::raw_type>(pmw.get_value());
	std::stringstream out;
	out << "0x" << std::setfill('0') << std::hex
	    << std::setw(round_up_integer_division(num_bits(PPUMemoryWord::Value::max), 4)) << w;
	os << out.str() << ")";
	return os;
}
#endif

template <typename AddressT>
std::array<AddressT, PPUMemoryWord::config_size_in_words> PPUMemoryWord::addresses(
    coordinate_type const& coord)
{
	PPUMemoryWord::raw_type tmp = coord.toPPUMemoryWordOnPPU().value();
	if (coord.toPPUOnDLS() == halco::hicann_dls::vx::PPUOnDLS::top) {
		tmp += top_ppu_base_address;
	} else {
		tmp += bottom_ppu_base_address;
	}
	return {AddressT(tmp)};
}

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, PPUMemoryWord::config_size_in_words>
    PPUMemoryWord::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
        coordinate_type const& coord);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, PPUMemoryWord::config_size_in_words>
    PPUMemoryWord::addresses<halco::hicann_dls::vx::OmnibusAddress>(coordinate_type const& coord);

template <typename WordT>
std::array<WordT, PPUMemoryWord::config_size_in_words> PPUMemoryWord::encode() const
{
	return {{static_cast<WordT>(get_value())}};
}

template SYMBOL_VISIBLE std::
    array<fisch::vx::word_access_type::OmnibusChipOverJTAG, PPUMemoryWord::config_size_in_words>
    PPUMemoryWord::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, PPUMemoryWord::config_size_in_words>
    PPUMemoryWord::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void PPUMemoryWord::decode(std::array<WordT, PPUMemoryWord::config_size_in_words> const& data)
{
	set_value(Value(data[0]));
}

template SYMBOL_VISIBLE void
PPUMemoryWord::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        PPUMemoryWord::config_size_in_words> const& data);

template SYMBOL_VISIBLE void PPUMemoryWord::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<fisch::vx::word_access_type::Omnibus, PPUMemoryWord::config_size_in_words> const&
        data);

PPUMemoryBlock::PPUMemoryBlock(size_type const size) : m_words(size.value()) {}

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
#ifndef __ppu__
		throw std::range_error(ss.str());
#else
		exit(1);
#endif
	}
	m_words = words;
}

PPUMemoryWord& PPUMemoryBlock::at(size_t index)
{
	if (index >= size()) {
		std::stringstream ss;
		ss << "given index(" << index << ") not in range(0 -> " << size() - 1 << ")";
#ifndef __ppu__
		throw std::out_of_range(ss.str());
#else
		exit(1);
#endif
	}
	return m_words.at(index);
}

PPUMemoryWord const& PPUMemoryBlock::at(size_t index) const
{
	if (index >= size()) {
		std::stringstream ss;
		ss << "given index(" << index << ") not in range(0 -> " << size() - 1 << ")";
#ifndef __ppu__
		throw std::out_of_range(ss.str());
#else
		exit(1);
#endif
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
#ifndef __ppu__
		throw std::out_of_range(ss.str());
#else
		exit(1);
#endif
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
#ifndef __ppu__
		throw std::out_of_range(ss.str());
#else
		exit(1);
#endif
	}
	for (size_t i = 0; i < subblock.size(); ++i) {
		m_words.at(i + begin) = subblock.m_words.at(i);
	}
}

halco::hicann_dls::vx::PPUMemoryBlockSize PPUMemoryBlock::size() const
{
	return halco::hicann_dls::vx::PPUMemoryBlockSize(m_words.size());
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
		auto const w = ntohl(static_cast<PPUMemoryWord::raw_type>((x.get_value())));
		char const* c = reinterpret_cast<char const*>(&w);
		for (size_t i = 0; i < sizeof(PPUMemoryWord::raw_type); ++i) {
			// Return if null byte is found
			if (c[i] == 0) {
				return ss.str();
			}

			// discard non-printable characters
			if (isprint(c[i]) or isspace(c[i])) {
				ss << c[i];
			}
		}
	}
	return ss.str();
}

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, PPUMemoryBlock const& pmb)
{
	hate::IndentingOstream ios(os);
	ios << "PPUMemoryBlock(\n" << hate::Indentation("\t");
	int const words_per_line = 4;
	auto const words = pmb.get_words();
	for (unsigned int i = 0; i < words.size(); i += words_per_line) {
		// print halfwords in hex
		std::stringstream halfwords;
		halfwords << std::hex << std::internal << std::setfill('0');
		for (unsigned int j = 0; ((j < words_per_line) && (i + j < words.size())); j++) {
			auto const word = static_cast<PPUMemoryWord::raw_type>(words[i + j].get_value());
			halfwords << std::setw(4) << (word >> 16) << " " << std::setw(4) << (word & 0xffff)
			          << " ";
		}
		// print as ascii
		std::stringstream ascii;
		for (unsigned int j = 0; ((j < words_per_line) && (i + j < words.size())); j++) {
			auto const word = ntohl(static_cast<PPUMemoryWord::raw_type>(words[i + j].get_value()));
			char const* chars = reinterpret_cast<char const*>(&word);
			for (int k = 0; k < 4; k++) {
				ascii << (isprint(chars[k]) ? chars[k] : '.');
			}
		}
		ios << halfwords.str() << ascii.str();
		if (i != words.size() - words_per_line) {
			ios << "\n";
		}
	}
	ios << hate::Indentation() << ")";
	return os;
}
#endif


PPUMemory::PPUMemory(words_type const& words) : m_words(words) {}


auto PPUMemory::get_words() const -> words_type
{
	return m_words;
}

void PPUMemory::set_words(words_type const& words)
{
	m_words = words;
}

PPUMemoryWord::Value PPUMemory::get_word(halco::hicann_dls::vx::PPUMemoryWordOnPPU const& pos) const
{
	return m_words.at(pos).get_value();
}

void PPUMemory::set_word(
    halco::hicann_dls::vx::PPUMemoryWordOnPPU const& pos, PPUMemoryWord::Value const& value)
{
	m_words.at(pos) = PPUMemoryWord(value);
}

PPUMemoryBlock PPUMemory::get_block(
    halco::hicann_dls::vx::PPUMemoryBlockOnPPU const& block_coord) const
{
	auto size = block_coord.toPPUMemoryBlockSize();
	PPUMemoryBlock block(size);
	auto const start = block_coord.toMin().value();
	auto const end = block_coord.toMax().value();
	typename PPUMemoryBlock::words_type const words(
	    std::next(std::begin(m_words), start), std::next(std::begin(m_words), end + 1));
	block.set_words(words);
	return block;
}

void PPUMemory::set_block(
    halco::hicann_dls::vx::PPUMemoryBlockOnPPU const& block_coord, PPUMemoryBlock const& block)
{
	auto size = block_coord.toPPUMemoryBlockSize();
	auto const start = block_coord.toMin().value();
	auto const end = block_coord.toMax().value();
	if (not(block.size() == size)) {
#ifndef __ppu__
		throw std::out_of_range("Mismatch of coordinate size and block size.");
#else
		exit(1);
#endif
	}
	auto start_it = std::next(std::begin(m_words), start);
	auto end_it = std::next(std::begin(m_words), end);
	for (auto it = start_it; it <= end_it; it++) {
		*it = block.at(std::distance(start_it, it));
	}
}

#ifndef __ppu__
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
	while ((program_bytes.size() % sizeof(PPUMemoryWord::raw_type)) != 0) {
		program_bytes.push_back(0);
	}

	if (program_bytes.size() > m_words.size() * sizeof(PPUMemoryWord::raw_type)) {
		throw std::runtime_error("PPU program to be loaded too large for memory bounds.");
	}

	// convert to words
	PPUMemoryWord::raw_type* iter =
	    reinterpret_cast<PPUMemoryWord::raw_type*>(program_bytes.data());
	std::vector<PPUMemoryWord::raw_type> words(
	    iter, iter + program_bytes.size() / sizeof(PPUMemoryWord::raw_type));

	// "correct" endianness (we perform a htonl afterwards for PPUMemoryWord)
	std::transform(words.begin(), words.end(), words.begin(), ntohl);

	// convert to PPUMemoryWords
	std::vector<PPUMemoryWord> ppu_memory_words(words.size());
	std::transform(
	    words.begin(), words.end(), ppu_memory_words.begin(),
	    [](PPUMemoryWord::raw_type const x) { return PPUMemoryWord(PPUMemoryWord::Value(x)); });

	std::copy(ppu_memory_words.cbegin(), ppu_memory_words.cend(), m_words.begin());
}
#endif

bool PPUMemory::operator==(PPUMemory const& other) const
{
	return (m_words == other.get_words());
}

bool PPUMemory::operator!=(PPUMemory const& other) const
{
	return !(*this == other);
}

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, PPUMemory const& pm)
{
	hate::IndentingOstream ios(os);
	ios << "PPUMemory(\n" << hate::Indentation("\t");
	int const words_per_line = 4;
	auto const words = pm.get_words();
	for (unsigned int i = 0; i < words.size(); i += words_per_line) {
		// break line and print address
		std::stringstream addr;
		addr << std::setfill('0') << std::hex << std::setw(6);
		addr << (i * 4) << ": ";
		// print halfwords in hex
		std::stringstream halfwords;
		halfwords << std::hex << std::internal << std::setfill('0');
		for (unsigned int j = 0; ((j < words_per_line) && (i + j < words.size())); j++) {
			auto const word = static_cast<PPUMemoryWord::raw_type>(
			    words[halco::hicann_dls::vx::PPUMemoryWordOnPPU(i + j)].get_value());
			halfwords << std::setw(4) << (word >> 16) << " " << std::setw(4) << (word & 0xffff)
			          << " ";
		}
		// print as ascii
		std::stringstream ascii;
		for (unsigned int j = 0; ((j < words_per_line) && (i + j < words.size())); j++) {
			auto const word = ntohl(static_cast<PPUMemoryWord::raw_type>(
			    words[halco::hicann_dls::vx::PPUMemoryWordOnPPU(i + j)].get_value()));
			char const* chars = reinterpret_cast<char const*>(&word);
			for (int k = 0; k < 4; k++) {
				ascii << (isprint(chars[k]) ? chars[k] : '.');
			}
		}
		ios << addr.str() << halfwords.str() << ascii.str();
		if (i != words.size() - words_per_line) {
			ios << "\n";
		}
	}
	ios << hate::Indentation() << ")";
	return os;
}
#endif


PPUControlRegister::PPUControlRegister() :
    m_cache_controller_enable(false),
    m_inhibit_reset(false),
    m_force_clock_on(false),
    m_force_clock_off(false)
{}

bool PPUControlRegister::get_cache_controller_enable() const
{
	return m_cache_controller_enable;
}

void PPUControlRegister::set_cache_controller_enable(bool const value)
{
	m_cache_controller_enable = value;
}

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

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, PPUControlRegister const& config)
{
	std::stringstream ss;
	ss << "PPUControlRegister(cache_controller_enable: " << std::boolalpha
	   << config.m_cache_controller_enable << ", inhibit_reset: " << config.m_inhibit_reset
	   << ", force_clock_on: " << config.m_force_clock_on
	   << ", force_clock_off: " << config.m_force_clock_off << ")";
	return (os << ss.str());
}
#endif

bool PPUControlRegister::operator==(PPUControlRegister const& other) const
{
	return m_cache_controller_enable == other.get_cache_controller_enable() &&
	       m_inhibit_reset == other.get_inhibit_reset() &&
	       m_force_clock_on == other.get_force_clock_on() &&
	       m_force_clock_off == other.get_force_clock_off();
}

bool PPUControlRegister::operator!=(PPUControlRegister const& other) const
{
	return !(*this == other);
}

namespace {

struct PPUControlRegisterBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
#define WORD \
			(uint32_t inhibit_reset           :  1; /* 0    */) \
			(uint32_t force_clock_on          :  1; /* 1    */) \
			(uint32_t force_clock_off         :  1; /* 2    */) \
			(uint32_t cache_controller_enable :  1; /* 3    */) \
			(uint32_t                         : 28; /* 4-32 */)
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	PPUControlRegisterBitfield() { u.raw = 0u; }

	PPUControlRegisterBitfield(uint32_t data) { u.raw = data & 0b1111ul; }
};

} // namespace

template <typename AddressT>
std::array<AddressT, PPUControlRegister::config_size_in_words> PPUControlRegister::addresses(
    coordinate_type const& coord)
{
	uint32_t tmp;
	if (coord.toPPUOnDLS().value() == 0) {
		tmp = top_ppu_base_address;
	} else {
		tmp = bottom_ppu_base_address;
	}
	tmp |= ppu_control_register_address_mask;
	return {AddressT{tmp}};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    PPUControlRegister::config_size_in_words>
PPUControlRegister::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, PPUControlRegister::config_size_in_words>
    PPUControlRegister::addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& coord);

template <typename WordT>
std::array<WordT, PPUControlRegister::config_size_in_words> PPUControlRegister::encode() const
{
	PPUControlRegisterBitfield bitfield;
	bitfield.u.m.cache_controller_enable = m_cache_controller_enable;
	bitfield.u.m.inhibit_reset = m_inhibit_reset;
	bitfield.u.m.force_clock_on = m_force_clock_on;
	bitfield.u.m.force_clock_off = m_force_clock_off;
	return {WordT{bitfield.u.raw}};
}

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    PPUControlRegister::config_size_in_words>
PPUControlRegister::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, PPUControlRegister::config_size_in_words>
    PPUControlRegister::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void PPUControlRegister::decode(
    std::array<WordT, PPUControlRegister::config_size_in_words> const& data)
{
	PPUControlRegisterBitfield bitfield(static_cast<uint32_t>(data[0]));
	m_cache_controller_enable = bitfield.u.m.cache_controller_enable;
	m_inhibit_reset = bitfield.u.m.inhibit_reset;
	m_force_clock_on = bitfield.u.m.force_clock_on;
	m_force_clock_off = bitfield.u.m.force_clock_off;
}

template SYMBOL_VISIBLE void
PPUControlRegister::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        PPUControlRegister::config_size_in_words> const& data);

template SYMBOL_VISIBLE void PPUControlRegister::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        PPUControlRegister::config_size_in_words> const& data);


PPUStatusRegister::PPUStatusRegister() : m_sleep(false) {}

bool PPUStatusRegister::get_sleep() const
{
	return m_sleep;
}

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, PPUStatusRegister const& config)
{
	std::stringstream ss;
	ss << "PPUStatusRegister(sleep: " << std::boolalpha << config.m_sleep << ")";
	return (os << ss.str());
}
#endif

bool PPUStatusRegister::operator==(PPUStatusRegister const& other) const
{
	return m_sleep == other.m_sleep;
}

bool PPUStatusRegister::operator!=(PPUStatusRegister const& other) const
{
	return !(*this == other);
}

template <typename AddressT>
std::array<AddressT, PPUStatusRegister::read_config_size_in_words>
PPUStatusRegister::read_addresses(coordinate_type const& coord)
{
	uint32_t tmp;
	if (coord.toPPUOnDLS().value() == 0) {
		tmp = top_ppu_base_address;
	} else {
		tmp = bottom_ppu_base_address;
	}
	tmp |= ppu_status_register_address_mask;
	return {AddressT{tmp}};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    PPUStatusRegister::read_config_size_in_words>
PPUStatusRegister::read_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, PPUStatusRegister::read_config_size_in_words>
    PPUStatusRegister::read_addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& coord);

template <typename AddressT>
std::array<AddressT, PPUStatusRegister::write_config_size_in_words>
PPUStatusRegister::write_addresses(coordinate_type const& /* coord */)
{
	return {};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    PPUStatusRegister::write_config_size_in_words>
PPUStatusRegister::write_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, PPUStatusRegister::write_config_size_in_words>
    PPUStatusRegister::write_addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& coord);

template <typename WordT>
std::array<WordT, PPUStatusRegister::write_config_size_in_words> PPUStatusRegister::encode() const
{
	return {};
}

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    PPUStatusRegister::write_config_size_in_words>
PPUStatusRegister::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, PPUStatusRegister::write_config_size_in_words>
    PPUStatusRegister::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void PPUStatusRegister::decode(
    std::array<WordT, PPUStatusRegister::read_config_size_in_words> const& data)
{
	m_sleep = static_cast<bool>(data[0]);
}

template SYMBOL_VISIBLE void
PPUStatusRegister::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        PPUStatusRegister::read_config_size_in_words> const& data);

template SYMBOL_VISIBLE void PPUStatusRegister::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        PPUStatusRegister::read_config_size_in_words> const& data);

} // namespace vx
} // namespace haldls

#ifndef __ppu__
namespace {

static std::unique_ptr<haldls::vx::Container> construct_container_PPUMemoryBlock(
    haldls::vx::Container::Coordinate const& coord)
{
	return std::make_unique<haldls::vx::PPUMemoryBlock>(
	    dynamic_cast<haldls::vx::PPUMemoryBlock::coordinate_type const&>(coord)
	        .toPPUMemoryBlockSize());
}

} // namespace
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE_CUSTOM_CONSTRUCTION(
    haldls::vx::PPUMemoryBlock, construct_container_PPUMemoryBlock)
#else
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::PPUMemoryBlock)
#endif

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::PPUMemoryWord)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::PPUMemory)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::PPUControlRegister)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::PPUStatusRegister)
