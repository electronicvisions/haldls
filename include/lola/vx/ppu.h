#pragma once
#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/cerealization.h"
#include "hate/visibility.h"
#include "lola/vx/cerealization.h"
#include "lola/vx/genpybind.h"
#include <boost/hana/adapt_struct.hpp>

#include "haldls/vx/fpga.h"
#include "haldls/vx/ppu.h"

class Elf;

namespace lola {
namespace vx GENPYBIND_TAG_LOLA_VX {

/**
 * Contiguous block of bytes in the external PPU memory.
 */
class GENPYBIND(visible) ExternalPPUMemoryBlock
{
public:
	typedef halco::hicann_dls::vx::ExternalPPUMemoryBlockOnFPGA coordinate_type;
	typedef std::false_type has_local_data;

	typedef std::vector<haldls::vx::ExternalPPUMemoryByte> bytes_type;

	typedef halco::hicann_dls::vx::ExternalPPUMemoryBlockSize size_type;

	explicit ExternalPPUMemoryBlock(
	    size_type size = size_type(halco::hicann_dls::vx::ExternalPPUMemoryByteOnFPGA::size))
	    SYMBOL_VISIBLE;

	haldls::vx::ExternalPPUMemoryByte& at(size_t index) SYMBOL_VISIBLE;
	haldls::vx::ExternalPPUMemoryByte const& at(size_t index) const SYMBOL_VISIBLE;
	haldls::vx::ExternalPPUMemoryByte& operator[](size_t index) SYMBOL_VISIBLE;
	haldls::vx::ExternalPPUMemoryByte const& operator[](size_t index) const SYMBOL_VISIBLE;

	ExternalPPUMemoryBlock get_subblock(size_t begin, size_type length) const SYMBOL_VISIBLE;
	void set_subblock(size_t begin, ExternalPPUMemoryBlock const& subblock) SYMBOL_VISIBLE;

	size_type size() const SYMBOL_VISIBLE;

	GENPYBIND(getter_for(bytes))
	bytes_type const& get_bytes() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(bytes))
	void set_bytes(bytes_type const& bytes) SYMBOL_VISIBLE;

	bool operator==(ExternalPPUMemoryBlock const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExternalPPUMemoryBlock const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ExternalPPUMemoryBlock const& config)
	    SYMBOL_VISIBLE;

	std::string to_string() const SYMBOL_VISIBLE;

	friend haldls::vx::detail::VisitPreorderImpl<ExternalPPUMemoryBlock>;

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version);

	bytes_type m_bytes;
};

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

#include "lola/vx/ppu.tcc"
