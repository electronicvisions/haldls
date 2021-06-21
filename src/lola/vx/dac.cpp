#include "lola/vx/dac.h"

#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/cerealization.tcc"
#include "hate/join.h"
#include "lola/vx/cerealization.tcc"
#include "lola/vx/hana.h"

namespace lola::vx {

DACChannelBlock::DACChannelBlock() : value()
{}

bool DACChannelBlock::operator==(DACChannelBlock const& other) const
{
	return equal(*this, other);
}

bool DACChannelBlock::operator!=(DACChannelBlock const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, DACChannelBlock const& block)
{
	return (os << "DACChannelBlock([" << hate::join_string(block.value, ", ") << "])");
}

DACChannelBlock const DACChannelBlock::default_ldo_1 = []() -> DACChannelBlock {
	using namespace halco::hicann_dls::vx;

	// Values taken from the xBoard wiki
	// https://brainscales-r.kip.uni-heidelberg.de/projects/symap2ic/wiki/xboard
	DACChannelBlock::Value const vdd25ad_value(4095 - 3820);
	DACChannelBlock::Value const vdd12ad_value(4095 - 3780);
	DACChannelBlock::Value const vdd12mp_value(4095 - 3655);

	DACChannelBlock block;
	block.value[DACChannelOnBoard::vdd25_digital] = vdd25ad_value;
	block.value[DACChannelOnBoard::vdd25_analog] = vdd25ad_value;
	block.value[DACChannelOnBoard::vdd12_digital] = vdd12ad_value;
	block.value[DACChannelOnBoard::vdd12_analog] = vdd12ad_value;
	block.value[DACChannelOnBoard::vdd12_madc] = vdd12mp_value;
	block.value[DACChannelOnBoard::vdd12_pll] = vdd12mp_value;
	return block;
}();

DACChannelBlock const DACChannelBlock::default_ldo_2 = []() -> DACChannelBlock {
	using namespace halco::hicann_dls::vx;

	DACChannelBlock::Value const vdd25ad_value(4095 - 3487);
	DACChannelBlock::Value const vdd12ad_value(4095 - 2465);
	DACChannelBlock::Value const vdd12mp_value(4095 - 3655);

	DACChannelBlock block;
	block.value[DACChannelOnBoard::vdd25_digital] = vdd25ad_value;
	block.value[DACChannelOnBoard::vdd25_analog] = vdd25ad_value;
	block.value[DACChannelOnBoard::vdd12_digital] = vdd12ad_value;
	block.value[DACChannelOnBoard::vdd12_analog] = vdd12ad_value;
	block.value[DACChannelOnBoard::vdd12_madc] = vdd12mp_value;
	block.value[DACChannelOnBoard::vdd12_pll] = vdd12mp_value;
	return block;
}();


DACControlBlock::DACControlBlock() : enable()
{
	enable.fill(true);
}

bool DACControlBlock::operator==(DACControlBlock const& other) const
{
	return equal(*this, other);
}

bool DACControlBlock::operator!=(DACControlBlock const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, DACControlBlock const& block)
{
	return (os << "DACControlBlock([" << hate::join_string(block.enable, ", ") << "])");
}

} // namespace lola::vx

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::DACChannelBlock)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::DACControlBlock)
