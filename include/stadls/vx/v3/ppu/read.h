#pragma once

#define CHIP_REVISION 3
#include "stadls/vx/ppu/read.h"
#include "stadls/vx/v3/coordinate_to_container.h"

namespace stadls::vx::v3::ppu {

template <typename ContainerT>
ContainerT read(typename ContainerT::coordinate_type const& coord)
{
	return stadls::vx::ppu::read<ContainerT, stadls::vx::v3::CoordinateToContainer>(coord);
}


} // namespace stadls::vx::v3::ppu
