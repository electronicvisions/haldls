#pragma once
#ifndef __ppu__
#error "stadls write only available on PPU"
#endif

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/coordinate_to_container.h"
#include "libnux/system.h"
#include "libnux/vx/omnibus.h"
#include "stadls/vx/addresses.h"
#include "stadls/vx/encode.h"
#include <algorithm>
#include <array>
#include <vector>


namespace stadls::vx::ppu {

template <typename ContainerT, template <typename> typename CoordinateToContainer>
void write(typename ContainerT::coordinate_type const& coord, ContainerT const& config)
{
	typedef fisch::vx::word_access_type::Omnibus backend_container_type;

	auto const addresses = get_write_addresses<
	    halco::hicann_dls::vx::OmnibusAddress, typename ContainerT::coordinate_type,
	    CoordinateToContainer>(coord);

	auto const words =
	    get_encode<backend_container_type, ContainerT, typename ContainerT::coordinate_type>(
	        config, coord);
	for (size_t i = 0; i < words.size(); ++i) {
		// only full words are supported
		if (!std::all_of(
		        words[i].byte_enables.begin(), words[i].byte_enables.end(),
		        [](auto const& value) { return value; })) {
			exit(1);
		}
		libnux::vx::omnibus_write(addresses[i].value(), words[i].word.value());
	}
}

} // namespace stadls::vx::ppu
