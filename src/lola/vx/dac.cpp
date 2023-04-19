#include "lola/vx/dac.h"

#include "haldls/vx/container.tcc"
#include "hate/join.h"
#include "lola/vx/hana.h"
#include <stdexcept>

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

void DACChannelBlock::set_voltage(halco::hicann_dls::vx::DACChannelOnBoard coord, double voltage)
{
	using namespace halco::hicann_dls::vx;
	double v_reference = 0;

	if (coord == DACChannelOnBoard::i_ref_board ||
	    coord == DACChannelOnBoard::ana_readout_debug_0 ||
	    coord == DACChannelOnBoard::ana_readout_debug_1 || coord == DACChannelOnBoard::mux_dac_25)
		v_reference = 2.5;
	else if (
	    coord == DACChannelOnBoard::v_reset || coord == DACChannelOnBoard::v_res_meas ||
	    coord == DACChannelOnBoard::mux_rfu_0 || coord == DACChannelOnBoard::mux_rfu_1)
		v_reference = 1.2;
	else
		throw std::invalid_argument("set_voltage is only available for DAC channels without LDOs.");

	value[coord] = Value(voltage / v_reference * Value::max);
}

double DACChannelBlock::get_voltage(halco::hicann_dls::vx::DACChannelOnBoard coord) const
{
	using namespace halco::hicann_dls::vx;
	double v_reference = 0;

	if (coord == DACChannelOnBoard::i_ref_board ||
	    coord == DACChannelOnBoard::ana_readout_debug_0 ||
	    coord == DACChannelOnBoard::ana_readout_debug_1 || coord == DACChannelOnBoard::mux_dac_25)
		v_reference = 2.5;
	else if (
	    coord == DACChannelOnBoard::v_reset || coord == DACChannelOnBoard::v_res_meas ||
	    coord == DACChannelOnBoard::mux_rfu_0 || coord == DACChannelOnBoard::mux_rfu_1)
		v_reference = 1.2;
	else
		throw std::invalid_argument("set_voltage is only available for DAC channels without LDOs.");

	return static_cast<double>(value[coord]) / static_cast<double>(Value::max) * v_reference;
}

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

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::DACChannelBlock)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::DACControlBlock)
