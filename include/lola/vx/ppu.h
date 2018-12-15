#pragma once
#include <boost/hana/adapt_struct.hpp>
#include "halco/hicann-dls/vx/coordinates.h"
#include "hate/visibility.h"
#include "lola/vx/genpybind.h"

#include "haldls/vx/ppu.h"

namespace cereal {
class access;
} // namespace cereal

class Elf;


namespace lola {
namespace vx GENPYBIND_TAG_LOLA_VX {

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
		Symbol(Type const& type, halco::hicann_dls::vx::PPUMemoryBlockOnPPU const& coord)
		    SYMBOL_VISIBLE;

		/** Type of symbol. */
		Type type;

		/** Location of memory block corresponding to symbol. */
		halco::hicann_dls::vx::PPUMemoryBlockOnPPU coordinate;

		bool operator==(Symbol const& other) const SYMBOL_VISIBLE;

		bool operator!=(Symbol const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, Symbol const& symbol) SYMBOL_VISIBLE;
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
	haldls::vx::PPUMemoryBlock read_program() SYMBOL_VISIBLE;

	/**
	 * Close file.
	 */
	~PPUElfFile() SYMBOL_VISIBLE;

private:
	int m_fd;
	Elf* m_elf_ptr;
};

} // namespace vx
} // namespace lola

BOOST_HANA_ADAPT_STRUCT(lola::vx::PPUProgram::Symbol, type, coordinate);
