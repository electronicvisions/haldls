#include "haldls/vx/block_until.h"

#ifndef __ppu__
#include <ostream>

namespace haldls::vx {

BlockUntil::~BlockUntil() {}

std::ostream& operator<<(std::ostream& os, BlockUntil const& block_until)
{
	return block_until.print(os);
}

} // namespace haldls::vx
#endif
