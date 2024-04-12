#include "lola/vx/ppu.h"

#include <iostream>
#include <map>
#include <vector>

#include <errno.h>
#include <fcntl.h>
#include <libelf.h>
#include <netinet/in.h>

#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/vx/container.tcc"
#include "haldls/vx/ppu.h"

#include "lola/vx/hana.h"

namespace lola {
namespace vx {

ExternalPPUMemoryBlock::ExternalPPUMemoryBlock(size_type const size) : m_bytes(size.value()) {}

ExternalPPUMemoryBlock::bytes_type const& ExternalPPUMemoryBlock::get_bytes() const
{
	return m_bytes;
}

void ExternalPPUMemoryBlock::set_bytes(bytes_type const& bytes)
{
	if (bytes.size() != size()) {
		std::stringstream ss;
		ss << "given vector size(" << bytes.size() << ") does not match container size(" << size()
		   << ")";
		throw std::range_error(ss.str());
	}
	m_bytes = bytes;
}

haldls::vx::ExternalPPUMemoryByte& ExternalPPUMemoryBlock::at(size_t index)
{
	if (index >= size()) {
		std::stringstream ss;
		ss << "given index(" << index << ") not in range(0 -> " << size() - 1 << ")";
		throw std::out_of_range(ss.str());
	}
	return m_bytes.at(index);
}

haldls::vx::ExternalPPUMemoryByte const& ExternalPPUMemoryBlock::at(size_t index) const
{
	if (index >= size()) {
		std::stringstream ss;
		ss << "given index(" << index << ") not in range(0 -> " << size() - 1 << ")";
		throw std::out_of_range(ss.str());
	}
	return m_bytes.at(index);
}

haldls::vx::ExternalPPUMemoryByte& ExternalPPUMemoryBlock::operator[](size_t index)
{
	return at(index);
}

haldls::vx::ExternalPPUMemoryByte const& ExternalPPUMemoryBlock::operator[](size_t index) const
{
	return at(index);
}

ExternalPPUMemoryBlock ExternalPPUMemoryBlock::get_subblock(size_t begin, size_type length) const
{
	if (begin + length > size()) {
		std::stringstream ss;
		ss << "subblock from index " << begin << " of size " << length
		   << " larger than block size of " << size();
		throw std::out_of_range(ss.str());
	}
	ExternalPPUMemoryBlock subblock(length);
	for (size_t i = 0; i < length; ++i) {
		subblock.m_bytes.at(i) = m_bytes.at(i + begin);
	}
	return subblock;
}

void ExternalPPUMemoryBlock::set_subblock(size_t begin, ExternalPPUMemoryBlock const& subblock)
{
	if (begin + subblock.size() > size()) {
		std::stringstream ss;
		ss << "subblock from index " << begin << " of size " << subblock.size()
		   << " larger than block size of " << size();
		throw std::out_of_range(ss.str());
	}
	for (size_t i = 0; i < subblock.size(); ++i) {
		m_bytes.at(i + begin) = subblock.m_bytes.at(i);
	}
}

halco::hicann_dls::vx::ExternalPPUMemoryBlockSize ExternalPPUMemoryBlock::size() const
{
	return halco::hicann_dls::vx::ExternalPPUMemoryBlockSize(m_bytes.size());
}

bool ExternalPPUMemoryBlock::operator==(ExternalPPUMemoryBlock const& other) const
{
	return (m_bytes == other.get_bytes());
}

bool ExternalPPUMemoryBlock::operator!=(ExternalPPUMemoryBlock const& other) const
{
	return !(*this == other);
}

std::string ExternalPPUMemoryBlock::to_string() const
{
	std::stringstream ss;
	for (auto x : m_bytes) {
		auto const c = x.get_value().value();
		// Return if null byte is found
		if (c == 0) {
			return ss.str();
		}

		// discard non-printable characters
		if (isprint(c) or isspace(c)) {
			ss << *reinterpret_cast<char const*>(&c);
		}
	}
	return ss.str();
}

std::ostream& operator<<(std::ostream& os, ExternalPPUMemoryBlock const& config)
{
	os << "ExternalPPUMemoryBlock(" << std::endl;
	for (auto const& byte : config.m_bytes) {
		os << "\t" << byte << std::endl;
	}
	os << ")";
	return os;
}


ExternalPPUMemory::ExternalPPUMemory() : bytes() {}

ExternalPPUMemoryBlock ExternalPPUMemory::get_subblock(size_t begin, size_type length) const
{
	if (begin + length > bytes.size()) {
		std::stringstream ss;
		ss << "subblock from index " << begin << " of size " << length
		   << " larger than block size of " << bytes.size();
		throw std::out_of_range(ss.str());
	}
	ExternalPPUMemoryBlock subblock(length);
	for (size_t i = 0; i < length; ++i) {
		subblock.at(i) = bytes[halco::hicann_dls::vx::ExternalPPUMemoryByteOnFPGA(i + begin)];
	}
	return subblock;
}

void ExternalPPUMemory::set_subblock(size_t begin, ExternalPPUMemoryBlock const& subblock)
{
	if (begin + subblock.size() > bytes.size()) {
		std::stringstream ss;
		ss << "subblock from index " << begin << " of size " << subblock.size()
		   << " larger than block size of " << bytes.size();
		throw std::out_of_range(ss.str());
	}
	for (size_t i = 0; i < subblock.size(); ++i) {
		bytes[halco::hicann_dls::vx::ExternalPPUMemoryByteOnFPGA(i + begin)] = subblock.at(i);
	}
}

bool ExternalPPUMemory::operator==(ExternalPPUMemory const& other) const
{
	return (bytes == other.bytes);
}

bool ExternalPPUMemory::operator!=(ExternalPPUMemory const& other) const
{
	return !(*this == other);
}

std::string ExternalPPUMemory::to_string() const
{
	std::stringstream ss;
	for (auto x : bytes) {
		auto const c = x.get_value().value();
		// Return if null byte is found
		if (c == 0) {
			return ss.str();
		}

		// discard non-printable characters
		if (isprint(c) or isspace(c)) {
			ss << *reinterpret_cast<char const*>(&c);
		}
	}
	return ss.str();
}

std::ostream& operator<<(std::ostream& os, ExternalPPUMemory const& config)
{
	return print(os, config);
}


PPUProgram::Symbol::Symbol() : type(Type::other), coordinate() {}

PPUProgram::Symbol::Symbol(Type const& type, Coordinate const& coord) :
    type(type), coordinate(coord)
{}

bool PPUProgram::Symbol::operator==(Symbol const& other) const
{
	return equal(*this, other);
}

bool PPUProgram::Symbol::operator!=(Symbol const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, PPUProgram::Symbol const& symbol)
{
	os << "Symbol(";
	switch (symbol.type) {
		case PPUProgram::Symbol::Type::object: {
			os << "object";
			break;
		}
		case PPUProgram::Symbol::Type::function: {
			os << "function";
			break;
		}
		default: {
			os << "other";
			break;
		}
	}
	os << ", ";
	std::visit([&](auto const& c) { os << c; }, symbol.coordinate);
	os << ")";
	return os;
}


PPUElfFile::PPUElfFile(std::string const& filename)
{
	if (elf_version(EV_CURRENT) == EV_NONE) {
		throw std::runtime_error("Libelf initialization failed.");
	}

	m_fd = open(filename.c_str(), O_RDONLY, 0);
	if (m_fd < 0) {
		std::stringstream ss;
		ss << "Error opening elf file \"" << filename << "\": " << strerror(errno) << ".";
		throw std::runtime_error(ss.str());
	}

	if ((m_elf_ptr = elf_begin(m_fd, ELF_C_READ, NULL)) == nullptr) {
		throw std::runtime_error("Elf_begin failed.");
	}

	if (elf_kind(m_elf_ptr) != ELF_K_ELF) {
		throw std::runtime_error("Not an ELF file");
	}
}

PPUElfFile::symbols_type PPUElfFile::read_symbols()
{
	size_t shstrndx;
	if (elf_getshdrstrndx(m_elf_ptr, &shstrndx) != 0) {
		throw std::runtime_error("Shstrndx not found.");
	}

	std::vector<Elf32_Sym> symtab;
	char* strtab = nullptr;

	// find strtab and symtabs
	Elf_Scn* scn = nullptr;
	while ((scn = elf_nextscn(m_elf_ptr, scn)) != nullptr) {
		Elf32_Shdr* shdr;
		if ((shdr = elf32_getshdr(scn)) == nullptr) {
			break;
		}
		char* name;
		if ((name = elf_strptr(m_elf_ptr, shstrndx, shdr->sh_name)) == nullptr) {
			throw std::runtime_error("elf_strptr failed.");
		}
		if ((shdr->sh_type == SHT_STRTAB) && (std::string(name) == ".strtab")) {
			Elf_Data* data = nullptr;
			if ((data = elf_getdata(scn, data)) != nullptr) {
				strtab = reinterpret_cast<char*>(data->d_buf);
			}
		}
		if (shdr->sh_type == SHT_SYMTAB) {
			Elf_Data* data = nullptr;
			for (size_t n = 0; n < shdr->sh_size && (data = elf_getdata(scn, data)) != nullptr;
			     ++n) {
				Elf32_Sym* p = reinterpret_cast<Elf32_Sym*>(data->d_buf);
				for (size_t i = 0; i < data->d_size / sizeof(Elf32_Sym); ++i) {
					symtab.push_back(p[i]);
				}
			}
		}
	}

	// iterate symtabs and find global objects
	symbols_type symbols;
	if (strtab != nullptr) {
		for (auto const x : symtab) {
			if (x.st_size != 0) { // ignore symbols of size 0 as they can't be altered
				PPUProgram::Symbol symbol;
				switch (ELF32_ST_TYPE(x.st_info)) {
					case STT_OBJECT: {
						symbol.type = PPUProgram::Symbol::Type::object;
						break;
					}
					case STT_FUNC: {
						symbol.type = PPUProgram::Symbol::Type::function;
						break;
					}
					default: {
						symbol.type = PPUProgram::Symbol::Type::other;
						break;
					}
				}
				auto const name = std::string(strtab + x.st_name);
				if (x.st_value >= external_base_address) { // external memory instructions
					auto min = halco::hicann_dls::vx::ExternalPPUMemoryByteOnFPGA(
					    x.st_value - external_base_address);
					auto max =
					    halco::hicann_dls::vx::ExternalPPUMemoryByteOnFPGA(min + x.st_size - 1);
					symbol.coordinate =
					    halco::hicann_dls::vx::ExternalPPUMemoryBlockOnFPGA(min, max);
				} else if (x.st_value >= external_data_base_address) { // external memory
					                                                   // data
					auto min = halco::hicann_dls::vx::ExternalPPUMemoryByteOnFPGA(
					    x.st_value - external_data_base_address);
					auto max =
					    halco::hicann_dls::vx::ExternalPPUMemoryByteOnFPGA(min + x.st_size - 1);
					symbol.coordinate =
					    halco::hicann_dls::vx::ExternalPPUMemoryBlockOnFPGA(min, max);
				} else { // internal memory
					auto min = halco::hicann_dls::vx::PPUMemoryWordOnPPU(
					    x.st_value / sizeof(haldls::vx::PPUMemoryWord::raw_type));
					auto max = halco::hicann_dls::vx::PPUMemoryWordOnPPU(
					    min +
					    (x.st_size + sizeof(haldls::vx::PPUMemoryWord::raw_type) - 1) /
					        sizeof(haldls::vx::PPUMemoryWord::raw_type) -
					    1);
					symbol.coordinate = halco::hicann_dls::vx::PPUMemoryBlockOnPPU(min, max);
				}
				symbols.insert({name, symbol});
			}
		}
	} else {
		throw std::runtime_error("No .strtab in elf file.");
	}
	return symbols;
}

namespace {

haldls::vx::PPUMemoryBlock to_internal_block(std::vector<char>&& bytes)
{
	// pad to multiple of raw word size
	while ((bytes.size() % sizeof(haldls::vx::PPUMemoryWord::raw_type)) != 0) {
		bytes.push_back(0);
	}

	// convert to words
	haldls::vx::PPUMemoryWord::raw_type* iter =
	    reinterpret_cast<haldls::vx::PPUMemoryWord::raw_type*>(bytes.data());
	std::vector<haldls::vx::PPUMemoryWord::raw_type> words(
	    iter, iter + bytes.size() / sizeof(haldls::vx::PPUMemoryWord::raw_type));

	// "correct" endianness (we perform a htonl afterwards for PPUMemoryWord)
	std::transform(words.begin(), words.end(), words.begin(), ntohl);

	// convert to PPUMemoryWords
	std::vector<haldls::vx::PPUMemoryWord> ppu_memory_words(words.size());
	std::transform(
	    words.begin(), words.end(), ppu_memory_words.begin(),
	    [](haldls::vx::PPUMemoryWord::raw_type const x) {
		    return haldls::vx::PPUMemoryWord(haldls::vx::PPUMemoryWord::Value(x));
	    });

	haldls::vx::PPUMemoryBlock block(
	    halco::hicann_dls::vx::PPUMemoryBlockSize(ppu_memory_words.size()));
	block.set_words(ppu_memory_words);
	return block;
}

std::optional<ExternalPPUMemoryBlock> to_external_block(std::vector<char>&& bytes)
{
	if (bytes.empty()) {
		return {};
	}

	// pad to multiple of raw word size
	while ((bytes.size() % sizeof(haldls::vx::PPUMemoryWord::raw_type)) != 0) {
		bytes.push_back(0);
	}

	ExternalPPUMemoryBlock block(halco::hicann_dls::vx::ExternalPPUMemoryBlockSize(bytes.size()));

	auto ppu_bytes = block.get_bytes();
	std::transform(bytes.begin(), bytes.end(), ppu_bytes.begin(), [](auto const& byte) {
		return haldls::vx::ExternalPPUMemoryByte(haldls::vx::ExternalPPUMemoryByte::Value(
		    *reinterpret_cast<haldls::vx::ExternalPPUMemoryByte::raw_type const*>(&byte)));
	});

	block.set_bytes(ppu_bytes);
	return block;
}

} // namespace

PPUElfFile::Memory PPUElfFile::read_program()
{
	size_t shstrndx;
	if (elf_getshdrstrndx(m_elf_ptr, &shstrndx) != 0) {
		throw std::runtime_error("Shstrndx not found.");
	}

	std::vector<char> internal_bytes;
	std::vector<char> external_bytes;

	auto const copy_scn = [](Elf_Scn* scn, std::vector<char>& bytes, size_t base_address) {
		Elf_Data* data = nullptr;
		if ((data = elf_getdata(scn, data)) != nullptr) {
			// maybe resize such that the new section fits
			if (bytes.size() <= base_address + data->d_size) {
				bytes.resize(base_address + data->d_size);
			}
			auto const byte_ptr = reinterpret_cast<char*>(data->d_buf);
			auto const begin = bytes.begin() + base_address;
			if (byte_ptr == nullptr) {
				std::fill(begin, begin + data->d_size, 0);
			} else {
				std::copy(byte_ptr, byte_ptr + data->d_size, begin);
			}
		} else {
			throw std::runtime_error("Error during elf_getdata.");
		}
	};

	Elf_Scn* scn = nullptr;
	while ((scn = elf_nextscn(m_elf_ptr, scn)) != nullptr) {
		Elf32_Shdr* shdr;
		if ((shdr = elf32_getshdr(scn)) == nullptr) {
			break;
		}
		char* name;
		if ((name = elf_strptr(m_elf_ptr, shstrndx, shdr->sh_name)) == nullptr) {
			throw std::runtime_error("elf_strptr failed.");
		}
		if (std::string(name).rfind("int", 0) == 0) {
			copy_scn(scn, internal_bytes, shdr->sh_addr);
		} else if (std::string(name).rfind("ext", 0) == 0) {
			if (shdr->sh_addr >= external_base_address) { // extmem instructions
				copy_scn(scn, external_bytes, shdr->sh_addr - external_base_address);
			} else if (shdr->sh_addr >= external_data_base_address) { // extmem data
				copy_scn(scn, external_bytes, shdr->sh_addr - external_data_base_address);
			} else {
				throw std::logic_error("External memory section base address not implemented.");
			}
		} else if (
		    (std::string(name).find(".text") != std::string::npos) ||
		    (std::string(name).find(".data") != std::string::npos) ||
		    (std::string(name).find(".rodata") != std::string::npos) ||
		    (std::string(name).find(".bss") != std::string::npos) ||
		    (std::string(name).find(".init") != std::string::npos) ||
		    (std::string(name).find(".fini") != std::string::npos)) {
			// old-skool matches
			copy_scn(scn, internal_bytes, shdr->sh_addr);
		}
		/* else drop silently */
	}

	auto const internal_block = to_internal_block(std::move(internal_bytes));
	auto const external_block = to_external_block(std::move(external_bytes));
	return {internal_block, external_block};
}

PPUElfFile::~PPUElfFile()
{
	// post-elf_end activation count => we do not care.
	elf_end(m_elf_ptr);
	close(m_fd);
}

} // namespace vx
} // namespace lola

#ifndef __ppu__
namespace {

static std::unique_ptr<haldls::vx::Container> construct_container_ExternalPPUMemoryBlock(
    haldls::vx::Container::Coordinate const& coord)
{
	return std::make_unique<lola::vx::ExternalPPUMemoryBlock>(
	    dynamic_cast<lola::vx::ExternalPPUMemoryBlock::coordinate_type const&>(coord)
	        .toExternalPPUMemoryBlockSize());
}

} // namespace

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE_CUSTOM_CONSTRUCTION(
    lola::vx::ExternalPPUMemoryBlock, construct_container_ExternalPPUMemoryBlock)
#else
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::ExternalPPUMemoryBlock)
#endif

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::ExternalPPUMemory)
