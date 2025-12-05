#include "lola/vx/fpga.h"

#include "haldls/vx/container.tcc"
#include "lola/vx/hana.h"

namespace lola {
namespace vx {

MultichipJboaLeafFPGA::MultichipJboaLeafFPGA() {}

bool MultichipJboaLeafFPGA::operator==(MultichipJboaLeafFPGA const& other) const
{
	return equal(*this, other);
}

bool MultichipJboaLeafFPGA::operator!=(MultichipJboaLeafFPGA const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, MultichipJboaLeafFPGA const& config)
{
	return print(os, config);
}

SinglechipFPGA::SinglechipFPGA() {}

bool SinglechipFPGA::operator==(SinglechipFPGA const& other) const
{
	return equal(*this, other);
}

bool SinglechipFPGA::operator!=(SinglechipFPGA const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, SinglechipFPGA const& config)
{
	return print(os, config);
}

} // namespace vx
} // namespace lola

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::MultichipJboaLeafFPGA)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::SinglechipFPGA)