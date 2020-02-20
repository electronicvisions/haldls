#include "lola/vx/ppu.h"

#include <map>
#include <vector>

#include <errno.h>
#include <fcntl.h>
#include <libelf.h>
#include <netinet/in.h>

#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/vx/ppu.h"

#include "lola/vx/hana.h"

namespace lola {
namespace vx {

PPUProgram::Symbol::Symbol() : type(Type::other), coordinate() {}

PPUProgram::Symbol::Symbol(
    Type const& type, halco::hicann_dls::vx::PPUMemoryBlockOnPPU const& coord) :
    type(type),
    coordinate(coord)
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
	os << ", " << symbol.coordinate << ")";
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

	Elf_Kind ek;
	if ((ek = elf_kind(m_elf_ptr)) != ELF_K_ELF) {
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
				auto min = halco::hicann_dls::vx::PPUMemoryWordOnPPU(x.st_value / sizeof(uint32_t));
				auto max = halco::hicann_dls::vx::PPUMemoryWordOnPPU(
				    min + (x.st_size + sizeof(uint32_t) - 1) / sizeof(uint32_t) - 1);
				symbol.coordinate = halco::hicann_dls::vx::PPUMemoryBlockOnPPU(min, max);
				symbols.insert({name, symbol});
			}
		}
	} else {
		throw std::runtime_error("No .strtab in elf file.");
	}
	return symbols;
}

haldls::vx::PPUMemoryBlock PPUElfFile::read_program()
{
	size_t phdrnum = 0;

	if (elf_getphdrnum(m_elf_ptr, &phdrnum) != 0) {
		throw std::runtime_error("Getting number of program headers failed.");
	}

	Elf32_Phdr* phdr = elf32_getphdr(m_elf_ptr);
	if (phdr == nullptr) {
		throw std::runtime_error("No program header found.");
	}

	std::vector<char> bytes(phdr->p_filesz);
	pread(m_fd, bytes.data(), phdr->p_filesz, phdr->p_offset);

	// convert to words
	uint32_t* iter = reinterpret_cast<uint32_t*>(bytes.data());
	std::vector<uint32_t> words(iter, iter + bytes.size() / sizeof(uint32_t));

	// correct endianness
	std::transform(words.begin(), words.end(), words.begin(), ntohl);

	// convert to PPUMemoryWords
	std::vector<haldls::vx::PPUMemoryWord> ppu_memory_words(words.size());
	std::transform(words.begin(), words.end(), ppu_memory_words.begin(), [](uint32_t const x) {
		return haldls::vx::PPUMemoryWord(haldls::vx::PPUMemoryWord::Value(x));
	});

	haldls::vx::PPUMemoryBlock block(
	    halco::hicann_dls::vx::PPUMemoryBlockSize(ppu_memory_words.size()));
	block.set_words(ppu_memory_words);
	return block;
}

PPUElfFile::~PPUElfFile()
{
	// post-elf_end activation count => we do not care.
	elf_end(m_elf_ptr);
	close(m_fd);
}

} // namespace vx
} // namespace lola
