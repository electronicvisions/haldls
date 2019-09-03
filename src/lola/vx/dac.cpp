#include "lola/vx/dac.h"

#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/cerealization.h"
#include "hate/join.h"

namespace lola::vx {

DACChannelBlock::DACChannelBlock() : m_value() {}

DACChannelBlock::_value_type const& DACChannelBlock::get_value() const
{
	return m_value;
}

void DACChannelBlock::set_value(_value_type const& value)
{
	m_value = value;
}

bool DACChannelBlock::operator==(DACChannelBlock const& other) const
{
	return m_value == other.m_value;
}

bool DACChannelBlock::operator!=(DACChannelBlock const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, DACChannelBlock const& block)
{
	return (os << "[" << hate::join_string(block.m_value, ", ") << "]");
}

template <typename Archive>
void DACChannelBlock::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(DACChannelBlock)

DACChannelBlock const DACChannelBlock::default_ldo_1 = []() -> DACChannelBlock {
	using namespace halco::hicann_dls::vx;
	DACChannelBlock block;
	auto value = block.get_value();

	// Values taken from the xBoard wiki
	// https://brainscales-r.kip.uni-heidelberg.de/projects/symap2ic/wiki/xboard
	DACChannelBlock::Value const vdd25ad_value(4095 - 3820);
	DACChannelBlock::Value const vdd12ad_value(4095 - 3780);
	DACChannelBlock::Value const vdd12mp_value(4095 - 3655);

	value[DACChannelOnBoard::vdd25_digital] = vdd25ad_value;
	value[DACChannelOnBoard::vdd25_analog] = vdd25ad_value;
	value[DACChannelOnBoard::vdd12_digital] = vdd12ad_value;
	value[DACChannelOnBoard::vdd12_analog] = vdd12ad_value;
	value[DACChannelOnBoard::vdd12_madc] = vdd12mp_value;
	value[DACChannelOnBoard::vdd12_pll] = vdd12mp_value;

	block.set_value(value);
	return block;
}();

DACChannelBlock const DACChannelBlock::default_ldo_2 = []() -> DACChannelBlock {
	using namespace halco::hicann_dls::vx;
	DACChannelBlock block;
	auto value = block.get_value();

	DACChannelBlock::Value const vdd25ad_value(4095 - 3487);
	DACChannelBlock::Value const vdd12ad_value(4095 - 2465);
	DACChannelBlock::Value const vdd12mp_value(4095 - 3655);

	value[DACChannelOnBoard::vdd25_digital] = vdd25ad_value;
	value[DACChannelOnBoard::vdd25_analog] = vdd25ad_value;
	value[DACChannelOnBoard::vdd12_digital] = vdd12ad_value;
	value[DACChannelOnBoard::vdd12_analog] = vdd12ad_value;
	value[DACChannelOnBoard::vdd12_madc] = vdd12mp_value;
	value[DACChannelOnBoard::vdd12_pll] = vdd12mp_value;

	block.set_value(value);
	return block;
}();


DACControlBlock::DACControlBlock() : m_enable()
{
	m_enable.fill(false);
	for (auto coord : halco::common::iter_all<halco::hicann_dls::vx::VDDOnBoard>()) {
		m_enable[coord.toDACChannelOnBoard()] = true;
	}
}

DACControlBlock::_enable_type const& DACControlBlock::get_enable() const
{
	return m_enable;
}

void DACControlBlock::set_enable(_enable_type const& value)
{
	m_enable = value;
}

bool DACControlBlock::operator==(DACControlBlock const& other) const
{
	return m_enable == other.m_enable;
}

bool DACControlBlock::operator!=(DACControlBlock const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, DACControlBlock const& block)
{
	return (os << "[" << hate::join_string(block.m_enable, ", ") << "]");
}

template <typename Archive>
void DACControlBlock::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_enable));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(DACControlBlock)

} // namespace lola::vx
