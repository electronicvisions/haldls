#include "lola/vx/system.h"

#include "haldls/vx/container.tcc"
#include "lola/vx/hana.h"

namespace lola {
namespace vx {

ChipAndMultichipJboaLeafFPGA::ChipAndMultichipJboaLeafFPGA() {}

bool ChipAndMultichipJboaLeafFPGA::operator==(ChipAndMultichipJboaLeafFPGA const& other) const
{
	return equal(*this, other);
}

bool ChipAndMultichipJboaLeafFPGA::operator!=(ChipAndMultichipJboaLeafFPGA const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ChipAndMultichipJboaLeafFPGA const& config)
{
	return print(os, config);
}

ChipAndSinglechipFPGA::ChipAndSinglechipFPGA() {}

bool ChipAndSinglechipFPGA::operator==(ChipAndSinglechipFPGA const& other) const
{
	return equal(*this, other);
}

bool ChipAndSinglechipFPGA::operator!=(ChipAndSinglechipFPGA const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ChipAndSinglechipFPGA const& config)
{
	return print(os, config);
}

} // namespace vx
} // namespace lola

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::ChipAndMultichipJboaLeafFPGA)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::ChipAndSinglechipFPGA)