#pragma once

#define CHIP_REVISION 2
#include "stadls/vx/ppu/write.h"
#include "stadls/vx/v2/coordinate_to_container.h"

namespace stadls::vx::v2::ppu {

template <typename ContainerT>
void write(typename ContainerT::coordinate_type const& coord, ContainerT const& config)
{
	return stadls::vx::ppu::write<ContainerT, stadls::vx::v2::CoordinateToContainer>(coord, config);
}

} // namespace stadls::vx::v2::ppu
