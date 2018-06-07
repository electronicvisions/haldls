#pragma once
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>

#include <fstream>
#include <map>
#include <vector>

#include <gelf.h>
#include <libelf.h>

#include "halco/hicann-dls/v2/coordinates.h"
#include "haldls/v2/ppu.h"

namespace ppu_program_symbols {

using namespace halco::hicann_dls::v2;
using namespace haldls::v2;

std::map<std::string, PPUMemoryBlock::coordinate_type> read_symbols(char const* filename)
{
	int fd = open(filename, O_RDONLY, 0);
	if (fd < 0) {
		throw std::runtime_error("Error opening elf file.");
	}

	if (elf_version(EV_CURRENT) == EV_NONE) {
		throw std::runtime_error("Libelf initialization failed.");
	}

	Elf* elf_ptr;
	if ((elf_ptr = elf_begin(fd, ELF_C_READ, nullptr)) == nullptr) {
		throw std::runtime_error("elf_begin failed.");
	}

	Elf_Kind ek;
	if ((ek = elf_kind(elf_ptr)) != ELF_K_ELF) {
		throw std::runtime_error("Not an ELF file");
	}

	size_t shstrndx;
	if (elf_getshdrstrndx(elf_ptr, &shstrndx) != 0) {
		throw std::runtime_error("shstrndx not found.");
	}

	std::vector<Elf32_Sym> symtab;
	char* strtab = nullptr;

	// find strtab and symtabs
	Elf_Scn* scn = nullptr;
	while ((scn = elf_nextscn(elf_ptr, scn)) != nullptr) {
		Elf32_Shdr* shdr;
		if ((shdr = elf32_getshdr(scn)) == nullptr) {
			break;
		}
		if (shdr->sh_type == SHT_STRTAB) {
			Elf_Data* data = nullptr;
			data = elf_getdata(scn, data);
			strtab = reinterpret_cast<char*>(data->d_buf);
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

	// iterate symtabs and find global data objects
	std::map<std::string, PPUMemoryBlock::coordinate_type> symbols;
	if (strtab != nullptr) {
		for (auto const x : symtab) {
			// we only want object objects
			if (ELF32_ST_TYPE(x.st_info) == STT_OBJECT) {
				auto const name = std::string(strtab + x.st_name);
				PPUMemoryBlock::coordinate_type block_coord(
				    PPUMemoryWordOnDLS((x.st_value / sizeof(uint32_t))),
				    PPUMemoryWordOnDLS((x.st_value + x.st_size - 1) / sizeof(uint32_t)));
				symbols.insert(std::pair<std::string, PPUMemoryBlock::coordinate_type>(name, block_coord));
			}
		}
	} else {
		throw std::runtime_error("No .strtab in elf file.");
	}
	elf_end(elf_ptr);
	close(fd);
	return symbols;
}

std::string print_symbols_info(std::map<std::string, PPUMemoryBlock::coordinate_type> symbols)
{
	std::stringstream ss;
	for (auto const x : symbols) {
		ss << "symbols " << x.first << "; address: " << x.second.first
		   << "; size:" << x.second.second - x.second.first + 1 << std::endl;
	}
	return ss.str();
}
}
