#pragma once
#include "halco/hicann-dls/v2/coordinates.h"
#include "hate/visibility.h"
#include "lola/v2/genpybind.h"

#include "haldls/v2/ppu.h"

// This file is a copy of include/lola/vx/ppu.h.
// Also modify it if changes here are necessary.


namespace cereal {
class access;
} // namespace cereal

class Elf;


namespace lola {
namespace v2 GENPYBIND_TAG_LOLA_V2 {

class GENPYBIND(visible) PPUProgram
{
public:
	/**
	 * Symbol of PPU program describing the location and type of a continuous block of memory.
	 * Symbols can either describe an object, i.e. global data or executable code of a function.
	 */
	class GENPYBIND(visible) Symbol
	{
	public:
		/**
		 * Symbol type.
		 */
		enum class Type
		{
			object,
			function,
			other
		};

		/**
		 * Default construct a symbol of type 'other' and default coordinate.
		 */
		Symbol() SYMBOL_VISIBLE;

		/**
		 * Construct a symbol from a type and a coordinate.
		 */
		Symbol(Type const& type, halco::hicann_dls::v2::PPUMemoryBlockOnDLS const& coord)
		    SYMBOL_VISIBLE;

		/** Type of symbol. */
		Type type;

		/** Location of memory block corresponding to symbol. */
		halco::hicann_dls::v2::PPUMemoryBlockOnDLS coordinate;

		bool operator==(Symbol const& other) const SYMBOL_VISIBLE;

		bool operator!=(Symbol const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, Symbol const& symbol) SYMBOL_VISIBLE;

	private:
		friend class cereal::access;
		template <typename Archive>
		void serialize(Archive& ar) SYMBOL_VISIBLE;
	};
};


/**
 * Read-access to memory and symbol data of PPU program file in the ELF file format.
 */
class GENPYBIND(visible) PPUElfFile
{
public:
	/**
	 * Map of named symbols.
	 */
	typedef std::map<std::string, PPUProgram::Symbol> symbols_type;

	/**
	 * Open file.
	 * @param filename File to open
	 */
	PPUElfFile(std::string const& filename) SYMBOL_VISIBLE;

	/**
	 * Read symbol data.
	 * @return Map of named symbols.
	 */
	symbols_type read_symbols() SYMBOL_VISIBLE;

	/**
	 * Read program memory data, i.e. the data associated to all sections necessary for execution
	 * placed at their specified location in memory.
	 * @return Continuous block of PPU memory data.
	 */
	haldls::v2::PPUMemoryBlock read_program() SYMBOL_VISIBLE;

	/**
	 * Close file.
	 */
	~PPUElfFile() SYMBOL_VISIBLE;

private:
	int m_fd;
	Elf* m_elf_ptr;
};

} // namespace v2
} // namespace lola
