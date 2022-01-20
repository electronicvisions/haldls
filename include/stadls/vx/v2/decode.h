#pragma once
#include "stadls/vx/decode.h"

#include "haldls/vx/v2/capmem.h"

namespace stadls::vx {

// specializations

template <>
inline void decode_random(std::mt19937& gen, haldls::vx::v2::CapMemBlockConfig& config)
{
	typedef haldls::vx::detail::BackendContainerTrait<
	    haldls::vx::v2::CapMemBlockConfig>::default_container word_type;
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
	    0, halco::hicann_dls::vx::v2::CapMemRowOnCapMemBlock::max);
	words.at(0) = word_type(1ul << hotbit_capmem_row(gen));

	// coordinate size is not power-of-two aligned
	std::uniform_int_distribution<uintmax_t> capmem_column(
	    0, halco::hicann_dls::vx::v2::CapMemColumnOnCapMemBlock::max);
	words.at(1) = word_type(capmem_column(gen));

	// select enums are not power-of-two aligned
	std::uniform_int_distribution<uintmax_t> select(0, 2);
	words.at(2) = word_type(select(gen));
	words.at(5) = word_type(select(gen));

	std::uniform_int_distribution<uintmax_t> prescale_ramp(
	    0, haldls::vx::v2::CapMemBlockConfig::PrescaleRamp::max);
	std::uniform_int_distribution<uintmax_t> prescale_pause(
	    0, haldls::vx::v2::CapMemBlockConfig::PrescalePause::max);
	std::uniform_int_distribution<uintmax_t> msbs(0, 0xf);
	std::uniform_int_distribution<uintmax_t> lsbs(0, 0xfffff);
	words.at(6) = word_type(
	    msbs(gen) << 28 | prescale_ramp(gen) << 24 | prescale_pause(gen) << 20 | lsbs(gen));

	// decode words into container
	haldls::vx::visit_preorder(config, coord, stadls::DecodeVisitor<words_type>{std::move(words)});
}

template <>
inline void decode_ones(haldls::vx::v2::CapMemBlockConfig& /* config */)
{
	throw std::runtime_error("CapMemBlockConfig can't be initialized with all ones");
}

} // namespace stadls::vx
