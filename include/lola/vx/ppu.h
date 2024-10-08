#pragma once
#include "halco/common/typed_heap_array.h"
#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/vx/container.h"
#include "haldls/vx/fpga.h"
#include "haldls/vx/ppu.h"
#include "hate/visibility.h"
#include "lola/vx/genpybind.h"
#include <map>
#include <optional>
#include <variant>
#include <boost/hana/adapt_struct.hpp>
#include <cereal/macros.hpp>

#ifdef __GENPYBIND_GENERATED__
#include <pybind11/pybind11.h>
#endif

class Elf;

namespace lola::vx {

struct ExternalPPUMemoryBlock;
struct ExternalPPUDRAMMemoryBlock;

} // namespace lola::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, lola::vx::ExternalPPUMemoryBlock& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, lola::vx::ExternalPPUDRAMMemoryBlock& value, std::uint32_t const version);

} // namespace cereal

namespace lola {
namespace vx GENPYBIND_TAG_LOLA_VX {

/**
 * Contiguous block of bytes in the external PPU memory.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ExternalPPUMemoryBlock
    : public haldls::vx::ContainerBase<ExternalPPUMemoryBlock>
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
	friend void ::cereal::CEREAL_SERIALIZE_FUNCTION_NAME(
	    Archive& ar, ExternalPPUMemoryBlock& value, std::uint32_t const version);

	bytes_type m_bytes;
};


/**
 * Contiguous block of bytes in the external DRAM PPU memory.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ExternalPPUDRAMMemoryBlock
    : public haldls::vx::ContainerBase<ExternalPPUDRAMMemoryBlock>
{
public:
	typedef halco::hicann_dls::vx::ExternalPPUDRAMMemoryBlockOnFPGA coordinate_type;
	typedef std::false_type has_local_data;

	typedef std::vector<haldls::vx::ExternalPPUDRAMMemoryByte> bytes_type;

	typedef halco::hicann_dls::vx::ExternalPPUDRAMMemoryBlockSize size_type;

	explicit ExternalPPUDRAMMemoryBlock(size_type size = size_type()) SYMBOL_VISIBLE;

	haldls::vx::ExternalPPUDRAMMemoryByte& at(size_t index) SYMBOL_VISIBLE;
	haldls::vx::ExternalPPUDRAMMemoryByte const& at(size_t index) const SYMBOL_VISIBLE;
	haldls::vx::ExternalPPUDRAMMemoryByte& operator[](size_t index) SYMBOL_VISIBLE;
	haldls::vx::ExternalPPUDRAMMemoryByte const& operator[](size_t index) const SYMBOL_VISIBLE;

	ExternalPPUDRAMMemoryBlock get_subblock(size_t begin, size_type length) const SYMBOL_VISIBLE;
	void set_subblock(size_t begin, ExternalPPUDRAMMemoryBlock const& subblock) SYMBOL_VISIBLE;

	size_type size() const SYMBOL_VISIBLE;

	GENPYBIND(getter_for(bytes))
	bytes_type const& get_bytes() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(bytes))
	void set_bytes(bytes_type const& bytes) SYMBOL_VISIBLE;

	bool operator==(ExternalPPUDRAMMemoryBlock const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExternalPPUDRAMMemoryBlock const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ExternalPPUDRAMMemoryBlock const& config)
	    SYMBOL_VISIBLE;

	std::string to_string() const SYMBOL_VISIBLE;

	friend haldls::vx::detail::VisitPreorderImpl<ExternalPPUDRAMMemoryBlock>;

private:
	friend class cereal::access;
	template <typename Archive>
	friend void ::cereal::CEREAL_SERIALIZE_FUNCTION_NAME(
	    Archive& ar, ExternalPPUDRAMMemoryBlock& value, std::uint32_t const version);

	bytes_type m_bytes;
};


/**
 * Complete external PPU memory.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ExternalPPUMemory
    : public haldls::vx::ContainerBase<ExternalPPUMemory>
{
public:
	typedef halco::hicann_dls::vx::ExternalPPUMemoryOnFPGA coordinate_type;
	typedef std::false_type has_local_data;

	typedef halco::common::typed_heap_array<
	    haldls::vx::ExternalPPUMemoryByte,
	    halco::hicann_dls::vx::ExternalPPUMemoryByteOnFPGA>
	    Bytes GENPYBIND(opaque);

	typedef halco::hicann_dls::vx::ExternalPPUMemoryBlockSize size_type;

	ExternalPPUMemory() SYMBOL_VISIBLE;

	Bytes bytes;

	ExternalPPUMemoryBlock get_subblock(size_t begin, size_type length) const SYMBOL_VISIBLE;
	void set_subblock(size_t begin, ExternalPPUMemoryBlock const& subblock) SYMBOL_VISIBLE;

	bool operator==(ExternalPPUMemory const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExternalPPUMemory const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ExternalPPUMemory const& config)
	    SYMBOL_VISIBLE;

	/**
	 * Print words as string discarding non-printable characters.
	 * @return Printable characters as string
	 */
	std::string to_string() const SYMBOL_VISIBLE;

	friend haldls::vx::detail::VisitPreorderImpl<ExternalPPUMemory>;
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
		typedef std::variant<
		    halco::hicann_dls::vx::PPUMemoryBlockOnPPU,
		    halco::hicann_dls::vx::ExternalPPUMemoryBlockOnFPGA,
		    halco::hicann_dls::vx::ExternalPPUDRAMMemoryBlockOnFPGA>
		    Coordinate;

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
		Symbol(Type const& type, Coordinate const& coord) SYMBOL_VISIBLE;

		/** Type of symbol. */
		Type type;

		/** Location of memory block corresponding to symbol. */
		Coordinate coordinate;

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

	struct GENPYBIND(visible) Memory
	{
		haldls::vx::PPUMemoryBlock internal;
		std::optional<lola::vx::ExternalPPUMemoryBlock> external;
		std::optional<lola::vx::ExternalPPUDRAMMemoryBlock> external_dram;
	};

	/**
	 * Read program memory data, i.e. the data associated to all sections necessary for execution
	 * placed at their specified location in memory.
	 * @return Continuous block of PPU memory data for external and internal memory.
	 */
	Memory read_program() SYMBOL_VISIBLE;

	/**
	 * Close file.
	 */
	~PPUElfFile() SYMBOL_VISIBLE;

private:
	int m_fd;
	Elf* m_elf_ptr;

	/**
	 * Base address of external memory for instructions from the view of the PPU.
	 */
	constexpr static uint32_t external_base_address = 0x8000'0000;
	/**
	 * Base address of external memory for data from the view of the PPU.
	 */
	constexpr static uint32_t external_data_base_address = 0x4000'0000;
	/**
	 * Base address of external DRAM memory for instructions from the view of the PPU.
	 */
	constexpr static uint32_t external_dram_base_address = 0x9000'0000;
	/**
	 * Base address of external DRAM memory for data from the view of the PPU.
	 */
	constexpr static uint32_t external_dram_data_base_address = 0x5000'0000;
};

} // namespace vx
} // namespace lola

#include "lola/vx/ppu.tcc"
