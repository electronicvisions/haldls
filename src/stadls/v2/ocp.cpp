#include "stadls/v2/ocp.h"

#include "flyspi-rw_api/flyspi_com.h"

#include "stadls/visitors.h"

namespace stadls {
namespace v2 {

std::vector<haldls::v2::ocp_word_type> ocp_read(
	rw_api::FlyspiCom& com, std::vector<haldls::v2::ocp_address_type> const& addresses)
{
	auto const loc = com.locate().chip(0);

	std::vector<haldls::v2::ocp_word_type> words;
	for (auto const& address : addresses) {
		haldls::v2::ocp_word_type data{rw_api::flyspi::ocpRead(com, loc, address.value)};
		words.push_back(data);
	}

	if (words.size() != addresses.size())
		throw std::logic_error("number of OCP addresses and words do not match");

	return words;
}

void ocp_write(
	rw_api::FlyspiCom& com,
	std::vector<haldls::v2::ocp_word_type> const& words,
	std::vector<haldls::v2::ocp_address_type> const& addresses)
{
	if (words.size() != addresses.size())
		throw std::logic_error("number of OCP addresses and words do not match");

	auto const loc = com.locate().chip(0);
	auto addr_it = addresses.cbegin();
	for (auto const& word : words) {
		rw_api::flyspi::ocpWrite(com, loc, addr_it->value, word.value);
		++addr_it;
	}
}

template <class T>
T ocp_read_container(rw_api::FlyspiCom& com, typename T::coordinate_type const& coord)
{
	typedef std::vector<haldls::v2::ocp_address_type> ocp_addresses_type;
	typedef std::vector<haldls::v2::ocp_word_type> ocp_words_type;

	T container;
	ocp_addresses_type addresses;
	visit_preorder(container, coord, ReadAddressVisitor<ocp_addresses_type>{addresses});

	ocp_words_type words = ocp_read(com, addresses);
	visit_preorder(container, coord, DecodeVisitor<ocp_words_type>{words});

	return container;
}

template <class T>
void ocp_write_container(
	rw_api::FlyspiCom& com, typename T::coordinate_type const& coord, T const& container)
{
	typedef std::vector<haldls::v2::ocp_address_type> ocp_addresses_type;
	typedef std::vector<haldls::v2::ocp_word_type> ocp_words_type;

	ocp_addresses_type addresses;
	visit_preorder(container, coord, WriteAddressVisitor<ocp_addresses_type>{addresses});
	ocp_words_type words;
	visit_preorder(container, coord, EncodeVisitor<ocp_words_type>{words});

	ocp_write(com, words, addresses);
}

// Explicit instantiation of template functions for all valid ocp container types.
#define OCP_CONTAINER(Type)                                                                        \
	template SYMBOL_VISIBLE void ocp_write_container<Type>(                                        \
		rw_api::FlyspiCom&, Type::coordinate_type const&, Type const&);                            \
	template SYMBOL_VISIBLE Type ocp_read_container<Type>(                                         \
		rw_api::FlyspiCom&, Type::coordinate_type const&);

OCP_CONTAINER(haldls::v2::Board)
OCP_CONTAINER(haldls::v2::FlyspiProgramAddress)
OCP_CONTAINER(haldls::v2::FlyspiProgramSize)
OCP_CONTAINER(haldls::v2::FlyspiResultAddress)
OCP_CONTAINER(haldls::v2::FlyspiResultSize)
OCP_CONTAINER(haldls::v2::FlyspiState)
OCP_CONTAINER(haldls::v2::FlyspiControl)
OCP_CONTAINER(haldls::v2::FlyspiConfig)
OCP_CONTAINER(haldls::v2::FlyspiException)

#undef OCP_CONTAINER

} // namespace v2
} // namespace stadls
