#pragma once
#ifndef __ppu__
#error "stadls read only available on PPU"
#endif

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/coordinate_to_container.h"
#include "libnux/vx/omnibus.h"
#include "stadls/vx/addresses.h"
#include "stadls/vx/set_decode.h"
#include <array>
#include <vector>

namespace stadls::vx::ppu {

template <typename ContainerT, template <typename> typename CoordinateToContainer>
ContainerT read(typename ContainerT::coordinate_type const& coord)
{
	typedef fisch::vx::word_access_type::Omnibus backend_container_type;

	auto const addresses = get_read_addresses<
	    halco::hicann_dls::vx::OmnibusAddress, typename ContainerT::coordinate_type,
	    CoordinateToContainer>(coord);

	auto config = haldls::vx::detail::coordinate_to_container<
	    typename ContainerT::coordinate_type, ContainerT>(coord);
	if constexpr (std::is_same_v<
	                  std::vector<typename std::remove_cv_t<decltype(addresses)>::value_type>,
	                  std::remove_cv_t<decltype(addresses)>>) {
		std::vector<backend_container_type> words;
		for (size_t i = 0; i < addresses.size(); ++i) {
			words.push_back(fisch::vx::word_access_type::Omnibus(
			    libnux::vx::omnibus_read(addresses[i].value())));
		}
		set_decode(config, coord, words);
	} else { // array
		std::array<backend_container_type, std::tuple_size_v<decltype(addresses)>> words;
		for (size_t i = 0; i < addresses.size(); ++i) {
			words[i] = fisch::vx::word_access_type::Omnibus(
			    libnux::vx::omnibus_read(addresses[i].value()));
		}
		set_decode(config, coord, words);
	}
	return config;
}

} // namespace stadls::vx::ppu
