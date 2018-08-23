#include "haldls/v2/synapsedriver.h"

#include "halco/common/iter_all.h"

namespace haldls {
namespace v2 {

SynapseDriverBlock::SynapseDriverBlock() : m_pulse_length(0), m_modes() {}

auto SynapseDriverBlock::get_modes() const -> modes_type
{
	return m_modes;
}

void SynapseDriverBlock::set_modes(modes_type const& values)
{
	m_modes = values;
}

SynapseDriverBlock::Mode SynapseDriverBlock::get_mode(
	halco::hicann_dls::v2::SynapseDriverOnDLS const& syndriver) const
{
	return m_modes.at(syndriver.value());
}

void SynapseDriverBlock::set_mode(
	halco::hicann_dls::v2::SynapseDriverOnDLS const& syndriver, SynapseDriverBlock::Mode value)
{
	m_modes.at(syndriver.value()) = value;
}

SynapseDriverBlock::PulseLength SynapseDriverBlock::get_pulse_length() const
{
	return m_pulse_length;
}

void SynapseDriverBlock::set_pulse_length(SynapseDriverBlock::PulseLength const& value)
{
	m_pulse_length = value;
}

bool SynapseDriverBlock::operator==(SynapseDriverBlock const& other) const
{
	return m_pulse_length == other.get_pulse_length() && m_modes == other.get_modes();
}

bool SynapseDriverBlock::operator!=(SynapseDriverBlock const& other) const
{
	return !(*this == other);
}

std::array<hardware_address_type, SynapseDriverBlock::config_size_in_words>
SynapseDriverBlock::addresses(coordinate_type const& /*unique*/) const
{
	hardware_address_type const base_addr = 0x1c000000;
	hardware_address_type const excitator_addr = base_addr + 0;
	hardware_address_type const inhibitory_addr = base_addr + 1;
	hardware_address_type const pulse_length_addr = base_addr + 2;
	return {{excitator_addr, inhibitory_addr, pulse_length_addr}};
}

std::array<hardware_word_type, SynapseDriverBlock::config_size_in_words>
SynapseDriverBlock::encode() const
{
	using namespace halco::common;
	using namespace halco::hicann_dls::v2;
	std::bitset<SynapseDriverOnDLS::size> inhibitory;
	std::bitset<SynapseDriverOnDLS::size> excitatory;
	for (auto const drv : iter_all<SynapseDriverOnDLS>()) {
		size_t const value = drv.value();
		size_t const complement = SynapseDriverOnDLS::max - value;
		if (m_modes.at(value) == Mode::inhibitory)
			inhibitory[complement] = 1;
		if (m_modes.at(value) == Mode::excitatory)
			excitatory[complement] = 1;
	}
	return {{static_cast<hardware_word_type>(excitatory.to_ulong()),
	         static_cast<hardware_word_type>(inhibitory.to_ulong()),
	         static_cast<hardware_word_type>(m_pulse_length)}};
}

void SynapseDriverBlock::decode(
	std::array<hardware_word_type, SynapseDriverBlock::config_size_in_words> const& data)
{
	using namespace halco::common;
	using namespace halco::hicann_dls::v2;
	std::bitset<SynapseDriverOnDLS::size> excitatory(data.at(0));
	std::bitset<SynapseDriverOnDLS::size> inhibitory(data.at(1));
	for (auto const drv : iter_all<SynapseDriverOnDLS>()) {
		size_t const value = drv.value();
		size_t const complement = SynapseDriverOnDLS::max - value;
		if (excitatory[complement])
			m_modes.at(value) = Mode::excitatory;
		else if (inhibitory[complement])
			m_modes.at(value) = Mode::inhibitory;
		else
			m_modes.at(value) = Mode::disabled;
	}
	m_pulse_length = PulseLength(data.at(2));
}


} // namespace v2
} // namespace haldls
