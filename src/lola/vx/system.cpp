#include "lola/vx/system.h"

#include "haldls/vx/container.tcc"
#include "lola/vx/hana.h"

namespace lola {
namespace vx {

ChipAndMultichipJboaLeafFPGA::ChipAndMultichipJboaLeafFPGA() {}
ChipAndMultichipJboaLeafFPGA::ChipAndMultichipJboaLeafFPGA(Chip chip) : chip(std::move(chip)) {}
ChipAndMultichipJboaLeafFPGA::ChipAndMultichipJboaLeafFPGA(Chip chip, FPGA fpga) :
    chip(std::move(chip)), fpga(std::move(fpga))
{
}

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
ChipAndSinglechipFPGA::ChipAndSinglechipFPGA(Chip chip) : chip(std::move(chip)) {}
ChipAndSinglechipFPGA::ChipAndSinglechipFPGA(Chip chip, FPGA fpga) :
    chip(std::move(chip)), fpga(std::move(fpga))
{
}

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