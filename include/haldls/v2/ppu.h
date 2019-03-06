#pragma once

#include <array>
#include <ostream>

#include "halco/hicann-dls/v2/coordinates.h"

#include "hate/visibility.h"
#include "haldls/v2/common.h"
#include "haldls/v2/genpybind.h"

#include "haldls/cerealization.h"

namespace haldls {
namespace v2 GENPYBIND_TAG_HALDLS_V2 {

class GENPYBIND(visible) PPUMemoryWord
{
public:
	typedef halco::hicann_dls::v2::PPUMemoryWordOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) Value
		: public halco::common::detail::RantWrapper<Value, uint_fast32_t, 0xffffffff, 0>
	{
		constexpr explicit Value(uintmax_t const val = 0) GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val) {}
	};

	PPUMemoryWord() SYMBOL_VISIBLE;
	explicit PPUMemoryWord(Value const& value) SYMBOL_VISIBLE;

	Value get() const SYMBOL_VISIBLE;
	void set(Value const& value) SYMBOL_VISIBLE;

	bool operator==(PPUMemoryWord const& other) const SYMBOL_VISIBLE;
	bool operator!=(PPUMemoryWord const& other) const SYMBOL_VISIBLE;
	friend std::ostream& operator<<(std::ostream& os, PPUMemoryWord const& pmw) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	std::array<hardware_address_type, config_size_in_words> addresses(coordinate_type const& word) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<hardware_word_type, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<hardware_word_type, config_size_in_words> const& data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	Value m_value;
};

class GENPYBIND(visible) PPUMemoryBlock
{
public:
	typedef halco::hicann_dls::v2::PPUMemoryBlockOnDLS coordinate_type;
	typedef std::false_type has_local_data;

	typedef std::vector<PPUMemoryWord> words_type;

	typedef halco::hicann_dls::v2::PPUMemoryBlockSize size_type;

	PPUMemoryBlock() SYMBOL_VISIBLE;
	explicit PPUMemoryBlock(size_type const& size) SYMBOL_VISIBLE;

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
	friend std::ostream& operator<<(std::ostream& os, PPUMemoryBlock const& pmb) SYMBOL_VISIBLE;

	friend detail::VisitPreorderImpl<PPUMemoryBlock>;

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	words_type m_words;
};

class GENPYBIND(visible) PPUMemory
{
public:
	typedef halco::hicann_dls::v2::PPUMemoryOnDLS coordinate_type;
	typedef std::false_type has_local_data;

	typedef std::array<PPUMemoryWord, halco::hicann_dls::v2::PPUMemoryWordOnDLS::size>
		words_type;

	PPUMemory() SYMBOL_VISIBLE;
	explicit PPUMemory(words_type const& words) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(words))
	words_type get_words() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(words))
	void set_words(words_type const& words) SYMBOL_VISIBLE;

	PPUMemoryWord::Value get_word(halco::hicann_dls::v2::PPUMemoryWordOnDLS const& pos) const SYMBOL_VISIBLE;
	void set_word(
		halco::hicann_dls::v2::PPUMemoryWordOnDLS const& pos, PPUMemoryWord::Value const& word) SYMBOL_VISIBLE;

	bool operator==(PPUMemory const& other) const SYMBOL_VISIBLE;
	bool operator!=(PPUMemory const& other) const SYMBOL_VISIBLE;
	friend std::ostream& operator<<(std::ostream& os, PPUMemory const& pm) SYMBOL_VISIBLE;

	friend detail::VisitPreorderImpl<PPUMemory>;

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	words_type m_words;
};

class GENPYBIND(visible) PPUControlRegister
{
public:
	typedef halco::hicann_dls::v2::PPUControlRegisterOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	PPUControlRegister() SYMBOL_VISIBLE;

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

	bool operator==(PPUControlRegister const& other) const SYMBOL_VISIBLE;
	bool operator!=(PPUControlRegister const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	std::array<hardware_address_type, config_size_in_words> addresses(coordinate_type const& unique) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<hardware_word_type, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<hardware_word_type, config_size_in_words> const& data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	bool m_inhibit_reset;
	bool m_force_clock_on;
	bool m_force_clock_off;
};

class GENPYBIND(visible) PPUStatusRegister
{
public:
	typedef halco::hicann_dls::v2::PPUStatusRegisterOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	PPUStatusRegister() SYMBOL_VISIBLE;

	// Read only property
	GENPYBIND(getter_for(sleep))
	bool get_sleep() const SYMBOL_VISIBLE;

	bool operator==(PPUStatusRegister const& other) const SYMBOL_VISIBLE;
	bool operator!=(PPUStatusRegister const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	std::array<hardware_address_type, config_size_in_words> addresses(coordinate_type const& unique) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<hardware_word_type, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<hardware_word_type, config_size_in_words> const& data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	bool m_sleep;
};

namespace detail {

template <>
struct VisitPreorderImpl<PPUMemory> {
	template <typename ContainerT, typename VisitorT>
	static void call(ContainerT& config, halco::hicann_dls::v2::PPUMemoryOnDLS const& coord, VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::v2;

		visitor(coord, config);

		for (auto const word : iter_all<PPUMemoryWordOnDLS>()) {
			// No std::forward for visitor argument, as we want to pass a reference to the
			// nested visitor in any case, even if it was passed as an rvalue to this function.
			visit_preorder(config.m_words[word], word, visitor);
		}
	}
};

template <>
struct VisitPreorderImpl<PPUMemoryBlock>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config, PPUMemoryBlock::coordinate_type const& coord, VisitorT&& visitor)
	{
		using namespace halco::hicann_dls::v2;

		if (coord.toPPUMemoryBlockSize() != config.size()) {
			std::stringstream ss;
			ss << "container size(" << config.size() << ") and coord(" << coord.min() << ", "
			   << coord.max() << " do not match.";
			throw std::runtime_error(ss.str());
		}

		visitor(coord, config);

		for (size_t counter = 0; counter < config.size(); counter++) {
			auto word_coord = PPUMemoryWordOnDLS(coord.min() + counter);
			visit_preorder(config.m_words.at(counter), word_coord, visitor);
		}
	}
};

} // namespace detail

} // namespace v2
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::v2::PPUMemoryWord::Value)

} // namespace std
