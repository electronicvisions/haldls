#include "stadls/vx/v3/dumperdone.h"

#include "fisch/vx/container_cast.h"
#include "haldls/cerealization.tcc"
#include "stadls/visitors.h"
#include <log4cxx/logger.h>

namespace stadls::vx::v3 {

bool DumperDone::operator==(DumperDone const& other) const
{
	return values == other.values;
}
bool DumperDone::operator!=(DumperDone const& other) const
{
	return !(*this == other);
}


namespace {

lola::vx::v3::Chip convert_to_chip_impl(
    DumperDone const& dumperdone, std::optional<lola::vx::v3::Chip> const& previous)
{
	typedef
	    typename haldls::vx::detail::BackendContainerTrait<lola::vx::v3::Chip>::default_container
	        backend_container_type;
	typedef decltype(
	    fisch::vx::container_cast(std::declval<backend_container_type>())) fisch_container_type;
	typedef typename fisch_container_type::coordinate_type address_type;
	typedef std::vector<address_type> addresses_type;
	typedef std::vector<backend_container_type> words_type;

	std::map<address_type, backend_container_type> memory;

	// apply previous state to memory
	if (previous) {
		words_type words;
		haldls::vx::visit_preorder(
		    *previous, halco::hicann_dls::vx::v3::ChipOnDLS(),
		    stadls::EncodeVisitor<words_type>{words});
		addresses_type addresses;
		haldls::vx::visit_preorder(
		    *previous, halco::hicann_dls::vx::v3::ChipOnDLS(),
		    stadls::WriteAddressVisitor<addresses_type>{addresses});

		for (size_t i = 0; i < addresses.size(); ++i) {
			if (memory.contains(addresses.at(i))) {
				for (size_t b = 0; b < sizeof(uint32_t); ++b) {
					auto& word = memory.at(addresses.at(i));
					if (words.at(i).byte_enables.at(b)) {
						word.word = backend_container_type::Word(
						    (word.word.value() & ~(0xff << (b * CHAR_BIT))) |
						    (words.at(i).word & (0xff << (b * CHAR_BIT))));
						if (word.byte_enables.at(b)) {
							throw std::logic_error(
							    "Trying to set same byte in same word multiple times: address(" +
							    std::to_string(addresses.at(i).value()) + ")");
						}
						word.byte_enables.at(b) = true;
					}
				}
			} else {
				memory[addresses.at(i)] = words.at(i);
			}
		}
	}

	// apply coco list to memory
	for (auto const& coco : dumperdone.values) {
		words_type words;
		addresses_type addresses;
		std::visit(
		    [&words, &addresses](auto const& cc) {
			    auto const& [coord, config] = cc;
			    if constexpr (hate::is_in_type_list<
			                      backend_container_type,
			                      typename haldls::vx::detail::BackendContainerTrait<
			                          std::remove_const_t<decltype(config)>>::container_list>::
			                      value) {
				    haldls::vx::visit_preorder(
				        config, coord, stadls::EncodeVisitor<words_type>{words});
				    haldls::vx::visit_preorder(
				        config, coord, stadls::WriteAddressVisitor<addresses_type>{addresses});
			    }
		    },
		    coco);

		for (size_t i = 0; i < addresses.size(); ++i) {
			if (memory.contains(addresses.at(i))) {
				for (size_t b = 0; b < sizeof(uint32_t); ++b) {
					auto& word = memory.at(addresses.at(i));
					if (words.at(i).byte_enables.at(b)) {
						word.word = backend_container_type::Word(
						    (word.word.value() & ~(0xff << (b * CHAR_BIT))) |
						    (words.at(i).word & (0xff << (b * CHAR_BIT))));
						word.byte_enables.at(b) = true;
					}
				}
			} else {
				memory[addresses.at(i)] = words.at(i);
			}
		}
	}

	// get chip read addresses
	lola::vx::v3::Chip chip;
	words_type words;
	haldls::vx::visit_preorder(
	    chip, halco::hicann_dls::vx::v3::ChipOnDLS(), stadls::EncodeVisitor<words_type>{words});
	addresses_type addresses;
	haldls::vx::visit_preorder(
	    chip, halco::hicann_dls::vx::v3::ChipOnDLS(),
	    stadls::ReadAddressVisitor<addresses_type>{addresses});

	// check that read and write addresses are the same, i.e. chip encoding/decoding is bijective
	static bool first_check = true;
	if (first_check) {
		addresses_type write_addresses;
		haldls::vx::visit_preorder(
		    chip, halco::hicann_dls::vx::v3::ChipOnDLS(),
		    stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		if (write_addresses != addresses) {
			throw std::logic_error("lola::Chip encoding/decoding is not bijective.");
		}
		first_check = false;
	}

	// check that memory doesn't contain configuration not present in chip
	{
		addresses_type addresses_sorted(addresses);
		std::sort(addresses_sorted.begin(), addresses_sorted.end());
		addresses_type memory_addresses_sorted;
		memory_addresses_sorted.reserve(memory.size());
		for (auto const& [address, _] : memory) {
			memory_addresses_sorted.push_back(address);
		}
		if (!std::includes(
		        addresses_sorted.begin(), addresses_sorted.end(), memory_addresses_sorted.begin(),
		        memory_addresses_sorted.end())) {
			auto logger = log4cxx::Logger::getLogger("stadls.convert_to_chip");
			LOG4CXX_WARN(
			    logger,
			    "Coco-list contains containers, which are not present in the lola chip object.");
		}
	}

	// apply memory to chip
	for (size_t i = 0; i < addresses.size(); ++i) {
		auto const& address = addresses.at(i);
		if (memory.contains(address)) {
			words.at(i) = memory.at(address);
		}
	}
	haldls::vx::visit_preorder(
	    chip, halco::hicann_dls::vx::v3::ChipOnDLS(), stadls::DecodeVisitor<words_type>{words});
	return chip;
}

} // namespace

lola::vx::v3::Chip convert_to_chip(DumperDone const& dumperdone)
{
	return convert_to_chip_impl(dumperdone, std::nullopt);
}

lola::vx::v3::Chip convert_to_chip(DumperDone const& dumperdone, lola::vx::v3::Chip const& previous)
{
	return convert_to_chip_impl(dumperdone, previous);
}

} // namespace stadls::vx::v3

template std::string haldls::vx::to_json(stadls::vx::v3::DumperDone const&);
template std::string haldls::vx::to_portablebinary(stadls::vx::v3::DumperDone const&);
template void haldls::vx::from_json(stadls::vx::v3::DumperDone&, std::string const&);
template void haldls::vx::from_portablebinary(stadls::vx::v3::DumperDone&, std::string const&);

CEREAL_CLASS_VERSION(stadls::vx::v3::DumperDone, 4)
