#pragma once

#ifndef CHIP_REVISION
#error "Requires CHIP_REVISION"
#endif

#include <limits>
#include <random>

#include "fisch/vx/container.h"
#include "fisch/vx/container_cast.h"
#include "fisch/vx/fill.h"
#include "halco/hicann-dls/vx/capmem.h"
#include "halco/hicann-dls/vx/ppu.h"
#include "haldls/vx/common.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/traits.h"
#include "lola/vx/capmem.h"
#include "stadls/visitors.h"

#define ID(x) x
#define XSTR(x) #x
#define STR(x) XSTR(x)
// clang-format off
#define CONCAT(w, x, y, z) STR(ID(w)ID(/)ID(x)ID(/)ID(v)ID(y)ID(/)ID(z))
#include CONCAT(haldls,vx,CHIP_REVISION,capmem.h)
// clang-format on
#undef ID
#undef XSTR
#undef STR
#undef CONCAT

#if CHIP_REVISION == 3
#define CHIP_REVISION_STR v3
#elif CHIP_REVISION == 4
#define CHIP_REVISION_STR v4
#else
#error "Unknown CHIP_REVISION"
#endif

namespace stadls::vx {

namespace detail {

/**
 * Get coordinate matching container, e.g. with respect to size.
 * @tparam T Container type
 * @param Container
 * @return Coordinate
 */
template <typename T>
typename T::coordinate_type get_coord(T const& config)
{
	using namespace halco::hicann_dls::vx;

	typename T::coordinate_type coord;
	if constexpr (std::is_same<T, haldls::vx::PPUMemoryBlock>::value) {
		coord = PPUMemoryBlockOnDLS(
		    PPUMemoryBlockOnPPU(
		        PPUMemoryWordOnPPU(0), PPUMemoryWordOnPPU(config.get_words().size() - 1)),
		    PPUOnDLS());
	}
	if constexpr (std::is_same<T, lola::vx::ExternalPPUMemoryBlock>::value) {
		coord = ExternalPPUMemoryBlockOnFPGA(
		    ExternalPPUMemoryByteOnFPGA(0),
		    ExternalPPUMemoryByteOnFPGA(config.get_bytes().size() - 1));
	}
	return coord;
}


/**
 * Get number of words expected for decoding.
 * @tparam T Container type
 * @param Container
 * @return Number of words for default backend
 */
template <typename T>
size_t count_decoding_words(T const& config)
{
	using namespace halco::hicann_dls::vx;
	typedef typename haldls::vx::detail::BackendContainerTrait<T>::default_container word_type;
	typedef std::vector<typename decltype(
	    fisch::vx::container_cast(std::declval<word_type>()))::coordinate_type>
	    addresses_type;
	addresses_type addresses;

	auto const coord = get_coord(config);

	// get number of expected words
	T mutable_config = config; // needed because visitor impl. might check on constness
	haldls::vx::visit_preorder(
	    mutable_config, coord, stadls::ReadAddressVisitor<addresses_type>{addresses});
	return addresses.size();
}

} // namespace detail


/**
 * Decode random data into a container.
 * @tparam T Container type
 * @param gen RNG
 * @return randomly filled container
 */
template <class T>
inline void decode_random(std::mt19937& gen, T& config)
{
	typedef typename haldls::vx::detail::BackendContainerTrait<T>::default_container word_type;
	typedef std::vector<word_type> words_type;

	words_type words(detail::count_decoding_words(config));

	auto const coord = detail::get_coord(config);

	// randomize word content
	for (auto& word : words) {
		word = fisch::vx::container_cast(
		    fisch::vx::fill_random<decltype(fisch::vx::container_cast(std::declval<word_type>()))>(
		        gen));
	}
	// decode words into container
	haldls::vx::visit_preorder(config, coord, stadls::DecodeVisitor<words_type>{std::move(words)});
}


/**
 * Decode data with all bits set to one into a container.
 * @tparam T Container type
 * @return Filled container
 */
template <class T>
inline void decode_ones(T& config)
{
	typedef typename haldls::vx::detail::BackendContainerTrait<T>::default_container word_type;
	typedef std::vector<word_type> words_type;

	words_type words(detail::count_decoding_words(config));

	auto const coord = detail::get_coord(config);

	// fill words
	for (auto& word : words) {
		word = fisch::vx::container_cast(
		    fisch::vx::fill_ones<decltype(fisch::vx::container_cast(std::declval<word_type>()))>());
	}
	// decode words into container
	haldls::vx::visit_preorder(config, coord, stadls::DecodeVisitor<words_type>{std::move(words)});
}

// specializations

template <>
inline void decode_random(std::mt19937& gen, haldls::vx::JTAGIdCode& config)
{
	// Draw a random word, to be transformed into a valid JTAG ID
	std::uniform_int_distribution<fisch::vx::JTAGIdCode::Value::value_type> random_word(
	    std::numeric_limits<fisch::vx::JTAGIdCode::Value::value_type>::min(),
	    std::numeric_limits<fisch::vx::JTAGIdCode::Value::value_type>::max());

	// Ensure marker-bit is set to 1, thereby form a valid JTAG ID
	auto random_jtag_id_word = random_word(gen) | 1u;

	std::array<
	    fisch::vx::word_access_type::JTAGIdCode, haldls::vx::JTAGIdCode::read_config_size_in_words>
	    data{fisch::vx::word_access_type::JTAGIdCode(random_jtag_id_word)};

	config.decode(data);
}

template <>
inline void decode_random(
    std::mt19937& gen, haldls::vx::CHIP_REVISION_STR::CapMemBlockConfig& config)
{
	typedef haldls::vx::detail::BackendContainerTrait<
	    haldls::vx::CHIP_REVISION_STR::CapMemBlockConfig>::default_container word_type;
	typedef std::vector<word_type> words_type;

	words_type words(detail::count_decoding_words(config));

	auto const coord = detail::get_coord(config);

	// randomize word content
	for (auto& word : words) {
		word = fisch::vx::container_cast(
		    fisch::vx::fill_random<decltype(fisch::vx::container_cast(std::declval<word_type>()))>(
		        gen));
	}

	std::uniform_int_distribution<uintmax_t> hotbit_capmem_row(
	    0, halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemRowOnCapMemBlock::max);
	words.at(0) = word_type(1ul << hotbit_capmem_row(gen));

	// coordinate size is not power-of-two aligned
	std::uniform_int_distribution<uintmax_t> capmem_column(
	    0, halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemColumnOnCapMemBlock::max);
	words.at(1) = word_type(capmem_column(gen));

	// select enums are not power-of-two aligned
	std::uniform_int_distribution<uintmax_t> select(0, 2);
	words.at(2) = word_type(select(gen));
	words.at(5) = word_type(select(gen));

	std::uniform_int_distribution<uintmax_t> prescale_ramp(
	    0, haldls::vx::CHIP_REVISION_STR::CapMemBlockConfig::PrescaleRamp::max);
	std::uniform_int_distribution<uintmax_t> prescale_pause(
	    0, haldls::vx::CHIP_REVISION_STR::CapMemBlockConfig::PrescalePause::max);
	std::uniform_int_distribution<uintmax_t> msbs(0, 0xf);
	std::uniform_int_distribution<uintmax_t> lsbs(0, 0xfffff);
	words.at(6) = word_type(
	    msbs(gen) << 28 | prescale_ramp(gen) << 24 | prescale_pause(gen) << 20 | lsbs(gen));

	// decode words into container
	haldls::vx::visit_preorder(config, coord, stadls::DecodeVisitor<words_type>{std::move(words)});
}

template <>
inline void decode_ones(haldls::vx::CHIP_REVISION_STR::CapMemBlockConfig& /* config */)
{
	throw std::runtime_error("CapMemBlockConfig can't be initialized with all ones");
}

template <>
inline void decode_random(std::mt19937& gen, lola::vx::CHIP_REVISION_STR::CapMem& config)
{
	typedef haldls::vx::detail::BackendContainerTrait<
	    lola::vx::CHIP_REVISION_STR::CapMem>::default_container word_type;
	typedef std::vector<word_type> words_type;

	words_type words(detail::count_decoding_words(config));

	auto const coord = detail::get_coord(config);

	// randomize word content
	for (auto& word : words) {
		word = fisch::vx::container_cast(
		    fisch::vx::fill_random<decltype(fisch::vx::container_cast(std::declval<word_type>()))>(
		        gen));
	}

	// overwrite CapMemBlockConfig words
	for (size_t i = 0; auto& block : config.blocks) {
		decode_random(gen, block);
		words_type block_words;
		haldls::vx::visit_preorder(
		    block, halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemBlockConfigOnDLS(i),
		    stadls::EncodeVisitor<words_type>{block_words});
		for (size_t j = 0; j < block_words.size(); ++j) {
			words.at(i * block_words.size() + j) = block_words.at(j);
		}
		i++;
	}

	// decode words into container
	haldls::vx::visit_preorder(config, coord, stadls::DecodeVisitor<words_type>{std::move(words)});
}

template <>
inline void decode_ones(lola::vx::CHIP_REVISION_STR::CapMem& /* config */)
{
	throw std::runtime_error("CapMem can't be initialized with all ones");
}

} // namespace stadls::vx
