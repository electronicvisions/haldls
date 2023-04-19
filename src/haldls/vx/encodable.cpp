#include "haldls/vx/encodable.h"

#ifndef __ppu__
#include <ostream>

namespace haldls::vx {

Encodable::~Encodable() {}

std::ostream& operator<<(std::ostream& os, Encodable const& encodable)
{
	return encodable.print(os);
}

} // namespace haldls::vx
#endif
