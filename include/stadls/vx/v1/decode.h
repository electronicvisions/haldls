#pragma once
#define CHIP_REVISION 1
#include "stadls/vx/decode.h"
#undef CHIP_REVISION

#include "lola/vx/v1/neuron.h"

namespace stadls::vx {

// specializations

template <>
inline void decode_random(std::mt19937& gen, lola::vx::v1::AtomicNeuron& config)
{
	typedef haldls::vx::detail::BackendContainerTrait<lola::vx::v1::AtomicNeuron>::default_container
	    word_type;
	typedef std::vector<word_type> words_type;

	words_type words(detail::count_decoding_words(config));

	auto const coord = detail::get_coord(config);

	// randomize word content
	for (auto& word : words) {
		word = fisch::vx::container_cast(
		    fisch::vx::fill_random<decltype(fisch::vx::container_cast(std::declval<word_type>()))>(
		        gen));
	}

	haldls::vx::v1::NeuronConfig neuron_config;
	decode_random(gen, neuron_config);

	neuron_config.set_enable_adaptation_readout(
	    neuron_config.get_readout_source() ==
	    haldls::vx::v1::NeuronConfig::ReadoutSource::adaptation);

	// overwrite NeuronConfig words
	words_type neuron_config_words;
	haldls::vx::visit_preorder(
	    neuron_config, coord.toNeuronConfigOnDLS(),
	    stadls::EncodeVisitor<words_type>{neuron_config_words});
	for (size_t j = 0; j < neuron_config_words.size(); ++j) {
		words.at(j) = neuron_config_words.at(j);
	}

	// decode words into container
	haldls::vx::visit_preorder(config, coord, stadls::DecodeVisitor<words_type>{std::move(words)});
}

} // namespace stadls::vx
