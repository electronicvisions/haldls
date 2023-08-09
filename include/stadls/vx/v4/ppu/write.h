#pragma once

#define CHIP_REVISION 4
#include "stadls/vx/ppu/write.h"
#include "stadls/vx/v4/coordinate_to_container.h"

namespace stadls::vx::v4::ppu {

template <typename ContainerT>
void write(typename ContainerT::coordinate_type const& coord, ContainerT const& config)
{
	return stadls::vx::ppu::write<ContainerT, stadls::vx::v4::CoordinateToContainer>(coord, config);
}

} // namespace stadls::vx::v4::ppu
