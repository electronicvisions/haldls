#pragma once
#include <array>
#include <iosfwd>

#include "halco/common/geometry.h"
#include "halco/common/iter_all.h"
#include "halco/common/typed_heap_array.h"
#include "halco/hicann-dls/vx/ppu.h"
#include "haldls/vx/common.h"
#include "haldls/vx/container.h"
#include "haldls/vx/coordinate_to_container.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/empty.h"
#include "hate/visibility.h"
#include <cereal/macros.hpp>

namespace haldls::vx {

struct PPUMemoryWord;
struct PPUMemoryBlock;
struct PPUMemory;
struct PPUControlRegister;
struct PPUStatusRegister;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PPUMemoryWord& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PPUMemoryBlock& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PPUMemory& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PPUControlRegister& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PPUStatusRegister& value, std::uint32_t const version);

} // namespace cereal

namespace fisch::vx {
class OmnibusChipOverJTAG;
class Omnibus;
} // namespace fisch::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) PPUMemoryWord
    : public ContainerBase<PPUMemoryWord>
{
public:
	typedef halco::hicann_dls::vx::PPUMemoryWordOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) Value
	    : public halco::common::detail::RantWrapper<Value, uint_fast32_t, 0xffffffff, 0>
	{
		constexpr explicit Value(uintmax_t const val = 0) : rant_t(val) {}
	};

	typedef uint32_t raw_type;
	// verify that the underlying word size matches the value type size
	static_assert(
	    static_cast<raw_type>(-1) == Value::max, "ray_type size does not match Value type.");

	explicit PPUMemoryWord(Value value = Value()) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(value))
	Value get_value() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(value))
	void set_value(Value const& value) SYMBOL_VISIBLE;

	bool operator==(PPUMemoryWord const& other) const SYMBOL_VISIBLE;
	bool operator!=(PPUMemoryWord const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PPUMemoryWord const& pmw) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& word)
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	template <class Archive>
	friend void ::cereal::serialize(Archive& ar, PPUMemoryWord& value, std::uint32_t const version);

	Value m_value;
};

class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) PPUMemoryBlock
    : public ContainerBase<PPUMemoryBlock>
{
public:
	typedef halco::hicann_dls::vx::PPUMemoryBlockOnDLS coordinate_type;
	typedef std::false_type has_local_data;

	typedef std::vector<PPUMemoryWord> words_type;

	typedef halco::hicann_dls::vx::PPUMemoryBlockSize size_type;

	explicit PPUMemoryBlock(
	    size_type size = size_type(halco::hicann_dls::vx::PPUMemoryWordOnPPU::size)) SYMBOL_VISIBLE;

	PPUMemoryWord& at(size_t index) SYMBOL_VISIBLE;
	PPUMemoryWord const& at(size_t index) const SYMBOL_VISIBLE;
	PPUMemoryWord& operator[](size_t index) SYMBOL_VISIBLE;
	PPUMemoryWord const& operator[](size_t index) const SYMBOL_VISIBLE;

	PPUMemoryBlock get_subblock(size_t begin, size_type length) const SYMBOL_VISIBLE;
	void set_subblock(size_t begin, PPUMemoryBlock const& subblock) SYMBOL_VISIBLE;

	size_type size() const SYMBOL_VISIBLE;

	GENPYBIND(getter_for(words))
	words_type const& get_words() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(words))
	void set_words(words_type const& words) SYMBOL_VISIBLE;

	bool operator==(PPUMemoryBlock const& other) const SYMBOL_VISIBLE;
	bool operator!=(PPUMemoryBlock const& other) const SYMBOL_VISIBLE;
	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PPUMemoryBlock const& pmb) SYMBOL_VISIBLE;
	/**
	 * Print words as string discarding non-printable characters.
	 * @return Printable characters as string in order according to PPU endianess
	 */
	std::string to_string() const SYMBOL_VISIBLE;

	friend detail::VisitPreorderImpl<PPUMemoryBlock>;

private:
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar, PPUMemoryBlock& value, std::uint32_t const version);

	words_type m_words;
};

class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) PPUMemory
    : public ContainerBase<PPUMemory>
{
public:
	typedef halco::hicann_dls::vx::PPUMemoryOnDLS coordinate_type;
	typedef std::false_type has_local_data;

	typedef halco::common::
	    typed_heap_array<PPUMemoryWord, halco::hicann_dls::vx::PPUMemoryWordOnPPU>
	        words_type GENPYBIND(opaque);

	explicit PPUMemory(words_type const& words = words_type()) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(words))
	words_type get_words() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(words))
	void set_words(words_type const& words) SYMBOL_VISIBLE;

	PPUMemoryWord::Value get_word(halco::hicann_dls::vx::PPUMemoryWordOnPPU const& pos) const
	    SYMBOL_VISIBLE;
	void set_word(
	    halco::hicann_dls::vx::PPUMemoryWordOnPPU const& pos,
	    PPUMemoryWord::Value const& word) SYMBOL_VISIBLE;

	PPUMemoryBlock get_block(halco::hicann_dls::vx::PPUMemoryBlockOnPPU const& block_coord) const
	    SYMBOL_VISIBLE;
	void set_block(
	    halco::hicann_dls::vx::PPUMemoryBlockOnPPU const& block_coord,
	    PPUMemoryBlock const& block) SYMBOL_VISIBLE;

	/**
	 * Load a (stripped) PPU program from a file.
	 * The program is located at the beginning of the memory with words above the program's size
	 * set to zero.
	 * @param filename Name of file to load
	 */
	void load_from_file(std::string const& filename) SYMBOL_VISIBLE;

	bool operator==(PPUMemory const& other) const SYMBOL_VISIBLE;
	bool operator!=(PPUMemory const& other) const SYMBOL_VISIBLE;
	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PPUMemory const& pm) SYMBOL_VISIBLE;

	friend detail::VisitPreorderImpl<PPUMemory>;

private:
	template <class Archive>
	friend void ::cereal::serialize(Archive& ar, PPUMemory& value, std::uint32_t const version);

	words_type m_words;
};

class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) PPUControlRegister
    : public DifferentialWriteTrait
    , public ContainerBase<PPUControlRegister>
{
public:
	typedef halco::hicann_dls::vx::PPUControlRegisterOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	PPUControlRegister() SYMBOL_VISIBLE;

	GENPYBIND(getter_for(cache_controller_enable))
	bool get_cache_controller_enable() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(cache_controller_enable))
	void set_cache_controller_enable(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(inhibit_reset))
	bool get_inhibit_reset() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(inhibit_reset))
	void set_inhibit_reset(bool const value) SYMBOL_VISIBLE;

	/* - force_clock_on: Forces the clock to be on, even if the ppu is sleeping
	 * - force_clock_off: Forces the clock to be off, which is weaker than
	 *   forcing the clock on
	 * None of these is needed in normal operation. Setting the inhibit_reset
	 * bit starts the execution. If the program finishes, the clock is stopped again. */
	GENPYBIND(getter_for(force_clock_on))
	bool get_force_clock_on() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(force_clock_on))
	void set_force_clock_on(bool const value) SYMBOL_VISIBLE;
	GENPYBIND(getter_for(force_clock_off))
	bool get_force_clock_off() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(force_clock_off))
	void set_force_clock_off(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream&, PPUControlRegister const&) SYMBOL_VISIBLE;

	bool operator==(PPUControlRegister const& other) const SYMBOL_VISIBLE;
	bool operator!=(PPUControlRegister const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& coord)
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar, PPUControlRegister& value, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_cache_controller_enable;
	bool m_inhibit_reset;
	bool m_force_clock_on;
	bool m_force_clock_off;
};


class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) PPUStatusRegister
    : public ContainerBase<PPUStatusRegister>
{
public:
	typedef halco::hicann_dls::vx::PPUStatusRegisterOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	PPUStatusRegister() SYMBOL_VISIBLE;

	// Read only property
	GENPYBIND(getter_for(sleep))
	bool get_sleep() const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream&, PPUStatusRegister const&) SYMBOL_VISIBLE;

	bool operator==(PPUStatusRegister const& other) const SYMBOL_VISIBLE;
	bool operator!=(PPUStatusRegister const& other) const SYMBOL_VISIBLE;

	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;
	template <typename AddressT>
	static std::array<AddressT, read_config_size_in_words> read_addresses(
	    coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename AddressT>
	static std::array<AddressT, write_config_size_in_words> write_addresses(
	    coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, write_config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, read_config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar, PPUStatusRegister& value, std::uint32_t const version) SYMBOL_VISIBLE;


	bool m_sleep;
};


namespace detail {

template <>
struct BackendContainerTrait<PPUMemoryWord>
    : public BackendContainerBase<
          PPUMemoryWord,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<PPUMemoryBlock>
    : public BackendContainerBase<
          PPUMemoryBlock,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<PPUMemory>
    : public BackendContainerBase<
          PPUMemory,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<PPUMemoryBlock>
{
	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config, PPUMemoryBlock::coordinate_type const& coord, VisitorT&& visitor)
	{
		using namespace halco::hicann_dls::vx;

		if (coord.toPPUMemoryBlockOnPPU().toPPUMemoryBlockSize() != config.size()) {
			std::stringstream ss;
			ss << "container size(" << config.size() << ") and " << coord << " size do not match.";
			throw std::runtime_error(ss.str());
		}

		visitor(coord, config);

		for (size_t counter = 0; counter < config.size(); counter++) {
			auto word_coord = PPUMemoryWordOnDLS(
			    PPUMemoryWordOnPPU(coord.toPPUMemoryBlockOnPPU().toMin() + counter),
			    coord.toPPUOnDLS());
			visit_preorder(config.m_words.at(counter), word_coord, visitor);
		}
	}

	template <typename VisitorT>
	static void call(
	    hate::Empty<PPUMemoryBlock> const& config,
	    PPUMemoryBlock::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using namespace halco::hicann_dls::vx;
		visitor(coord, config);

		for (size_t counter = 0; counter < coord.toPPUMemoryBlockSize(); counter++) {
			auto word_coord = PPUMemoryWordOnDLS(
			    PPUMemoryWordOnPPU(coord.toPPUMemoryBlockOnPPU().toMin() + counter),
			    coord.toPPUOnDLS());
			hate::Empty<PPUMemoryWord> word_config;
			visit_preorder(word_config, word_coord, visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<PPUMemoryBlock::coordinate_type> const& coord,
	    VisitorT&& visitor)
	{
		using namespace halco::hicann_dls::vx;
		visitor(coord, config);

		for (size_t counter = 0; counter < config.size(); counter++) {
			hate::Empty<PPUMemoryWordOnDLS> word_coord;
			visit_preorder(config.m_words.at(counter), word_coord, visitor);
		}
	}
};

template <>
struct VisitPreorderImpl<PPUMemory>
{
	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config, PPUMemory::coordinate_type const& coord, VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const word : iter_all<PPUMemoryWordOnPPU>()) {
			// No std::forward for visitor argument, as we want to pass a reference to the
			// nested visitor in any case, even if it was passed as an rvalue to this function.
			visit_preorder(
			    config.m_words[word], PPUMemoryWordOnDLS(word, coord.toPPUOnDLS()), visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<PPUMemory::coordinate_type> const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const word : iter_all<PPUMemoryWordOnPPU>()) {
			// No std::forward for visitor argument, as we want to pass a reference to the
			// nested visitor in any case, even if it was passed as an rvalue to this function.
			hate::Empty<PPUMemoryWordOnDLS> word_coord;
			visit_preorder(config.m_words[word], word_coord, visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> const& config,
	    PPUMemory::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const word : iter_all<PPUMemoryWordOnPPU>()) {
			// No std::forward for visitor argument, as we want to pass a reference to the
			// nested visitor in any case, even if it was passed as an rvalue to this function.
			hate::Empty<PPUMemoryWord> word_config;
			visit_preorder(word_config, PPUMemoryWordOnDLS(word, coord.toPPUOnDLS()), visitor);
		}
	}
};

template <>
struct BackendContainerTrait<PPUStatusRegister>
    : public BackendContainerBase<
          PPUStatusRegister,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<PPUControlRegister>
    : public BackendContainerBase<
          PPUControlRegister,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
inline PPUMemoryBlock coordinate_to_container(PPUMemoryBlock::coordinate_type const& coord)
{
	return PPUMemoryBlock(coord.toPPUMemoryBlockSize());
}

} // namespace detail

} // namespace vx
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::vx::PPUMemoryWord::Value)

} // namespace std
