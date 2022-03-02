#pragma once
#define CHIP_REVISION 1
#include "stadls/vx/decode.h"
#undef CHIP_REVISION

#include "lola/vx/v1/chip.h"
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

template <>
inline void decode_random(std::mt19937& gen, lola::vx::v1::NeuronBlock& config)
{
	typedef haldls::vx::detail::BackendContainerTrait<lola::vx::v1::NeuronBlock>::default_container
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

	// overwrite AtomicNeuron words
	for (size_t i = 0; auto& an : config.atomic_neurons) {
		decode_random(gen, an);
		words_type an_words;
		haldls::vx::visit_preorder(
		    an, halco::hicann_dls::vx::v1::AtomicNeuronOnDLS(halco::common::Enum(i)),
		    stadls::EncodeVisitor<words_type>{an_words});
		for (size_t j = 0; j < an_words.size(); ++j) {
			words.at(i * an_words.size() + j) = an_words.at(j);
		}
		i++;
	}

	// decode words into container
	haldls::vx::visit_preorder(config, coord, stadls::DecodeVisitor<words_type>{std::move(words)});
}

template <>
inline void decode_random(std::mt19937& gen, lola::vx::v1::Chip& config)
{
	using namespace halco::hicann_dls::vx::v1;
	using namespace halco::common;

	decode_random(gen, config.memory_timing);
	decode_random(gen, config.crossbar);

	for (auto const c : iter_all<BackgroundSpikeSourceOnDLS>()) {
		decode_random(gen, config.background_spike_sources[c]);
	}

	decode_random(gen, config.capmem);

	for (auto const c : iter_all<SynapseDriverBlockOnDLS>()) {
		decode_random(gen, config.synapse_driver_blocks[c]);
	}

	for (auto const c : iter_all<SynapseBlockOnDLS>()) {
		decode_random(gen, config.synapse_blocks[c]);
	}

	decode_random(gen, config.neuron_block);
	for (auto const c : iter_all<CADCOnDLS>()) {
		decode_random(gen, config.cadc_readout_chains[c]);
	}

	decode_random(gen, config.readout_chain);

	for (auto const c : iter_all<PPUMemoryOnDLS>()) {
		decode_random(gen, config.ppu_memory[c]);
	}

	decode_random(gen, config.external_ppu_memory);
}

} // namespace stadls::vx
