#include "haldls/vx/event.h"

#include <cereal/types/array.hpp>

#include "halco/common/cerealization_geometry.h"
#include "hate/join.h"
#include "haldls/cerealization.h"

namespace haldls::vx {

halco::hicann_dls::vx::SPL1Address SpikeLabel::get_spl1_address() const
{
	return halco::hicann_dls::vx::SPL1Address(value() >> 14);
}

void SpikeLabel::set_spl1_address(halco::hicann_dls::vx::SPL1Address const value)
{
	operator=(SpikeLabel(
	    (this->value() & halco::hicann_dls::vx::NeuronLabel::max) |
	    (static_cast<uint16_t>(value) << 14)));
}

halco::hicann_dls::vx::NeuronLabel SpikeLabel::get_neuron_label() const
{
	return halco::hicann_dls::vx::NeuronLabel(value() & halco::hicann_dls::vx::NeuronLabel::max);
}

void SpikeLabel::set_neuron_label(halco::hicann_dls::vx::NeuronLabel const value)
{
	operator=(SpikeLabel(
	    (static_cast<uint16_t>(value) & halco::hicann_dls::vx::NeuronLabel::max) |
	    (this->value() & 0xc000)));
}

SpikeLabel::PADILabel SpikeLabel::get_padi_label() const
{
	return PADILabel(value() & 0x3ff);
}

void SpikeLabel::set_padi_label(PADILabel const value)
{
	operator=(SpikeLabel((static_cast<uint16_t>(value) & 0x3ff) | (this->value() & 0xfc00)));
}

NeuronBackendConfig::AddressOut SpikeLabel::get_neuron_backend_address_out() const
{
	return NeuronBackendConfig::AddressOut(value() & 0xff);
}

void SpikeLabel::set_neuron_backend_address_out(NeuronBackendConfig::AddressOut const value)
{
	operator=(SpikeLabel((static_cast<uint16_t>(value) & 0xff) | (this->value() & 0xff00)));
}

SynapseQuad::Label SpikeLabel::get_synapse_label() const
{
	return SynapseQuad::Label(value() & 0x3f);
}

void SpikeLabel::set_synapse_label(SynapseQuad::Label const value)
{
	operator=(SpikeLabel((static_cast<uint16_t>(value) & 0x3f) | (this->value() & 0xffc0)));
}


#define SpikePackToChip(Num)                                                                       \
	SpikePack##Num##ToChip::SpikePack##Num##ToChip() : m_impl() {}                                 \
                                                                                                   \
	SpikePack##Num##ToChip::SpikePack##Num##ToChip(labels_type const& labels) : m_impl(labels) {}  \
                                                                                                   \
	typename SpikePack##Num##ToChip::labels_type SpikePack##Num##ToChip::get_labels() const        \
	{                                                                                              \
		return m_impl;                                                                             \
	}                                                                                              \
                                                                                                   \
	void SpikePack##Num##ToChip::set_labels(labels_type const& value) { m_impl = value; }          \
                                                                                                   \
	bool SpikePack##Num##ToChip::operator==(SpikePack##Num##ToChip const& other) const             \
	{                                                                                              \
		return (m_impl == other.m_impl);                                                           \
	}                                                                                              \
                                                                                                   \
	bool SpikePack##Num##ToChip::operator!=(SpikePack##Num##ToChip const& other) const             \
	{                                                                                              \
		return !(*this == other);                                                                  \
	}                                                                                              \
                                                                                                   \
	std::ostream& operator<<(std::ostream& os, SpikePack##Num##ToChip const& spike_pack)           \
	{                                                                                              \
		os << "SpikePack" << #Num << "ToChip(" << hate::join_string(spike_pack.m_impl, ", ")       \
		   << ")";                                                                                 \
		return os;                                                                                 \
	}                                                                                              \
                                                                                                   \
	std::array<                                                                                    \
	    halco::hicann_dls::vx::SpikePack##Num##ToChipOnDLS,                                        \
	    SpikePack##Num##ToChip::config_size_in_words>                                              \
	    SpikePack##Num##ToChip::addresses(coordinate_type const& coord)                            \
	{                                                                                              \
		return {coord};                                                                            \
	}                                                                                              \
                                                                                                   \
	std::array<fisch::vx::SpikePack##Num##ToChip, SpikePack##Num##ToChip::config_size_in_words>    \
	    SpikePack##Num##ToChip::encode() const                                                     \
	{                                                                                              \
		fisch::vx::SpikePack##Num##ToChip::labels_type ret;                                        \
		std::transform(                                                                            \
		    std::begin(m_impl), std::end(m_impl), std::begin(ret),                                 \
		    [](SpikeLabel const& sl) { return fisch::vx::SpikeLabel{sl}; });                       \
		return {fisch::vx::SpikePack##Num##ToChip{ret}};                                           \
	}                                                                                              \
                                                                                                   \
	void SpikePack##Num##ToChip::decode(                                                           \
	    std::array<fisch::vx::SpikePack##Num##ToChip, config_size_in_words> const& data)           \
	{                                                                                              \
		fisch::vx::SpikePack##Num##ToChip tmp = data[0];                                           \
		auto const& labels = tmp.get_labels();                                                     \
		std::transform(                                                                            \
		    std::begin(labels), std::end(labels), std::begin(m_impl),                              \
		    [](fisch::vx::SpikeLabel const& sl) { return SpikeLabel{sl}; });                       \
	}                                                                                              \
                                                                                                   \
	template <typename Archive>                                                                    \
	void SpikePack##Num##ToChip::serialize(Archive& ar, std::uint32_t const)                       \
	{                                                                                              \
		ar(CEREAL_NVP(m_impl));                                                                    \
	}                                                                                              \
                                                                                                   \
	EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(SpikePack##Num##ToChip)

// clang-format off
SpikePackToChip(1)
SpikePackToChip(2)
SpikePackToChip(3)
// clang-format on

#undef SpikePackToChip


SpikeFromChip::SpikeFromChip() :
    m_label(),
    m_fpga_time(),
    m_chip_time()
{}

SpikeFromChip::SpikeFromChip(
    SpikeLabel const& label, FPGATime const& fpga_time, ChipTime const& chip_time) :
    m_label(label),
    m_fpga_time(fpga_time),
    m_chip_time(chip_time)
{}

SpikeFromChip::SpikeFromChip(fisch::vx::SpikeFromChipEvent const& data) :
    m_label(data.get_spike().get_label()),
    m_fpga_time(data.get_fpga_time()),
    m_chip_time(data.get_spike().get_chip_time())
{}

SpikeLabel SpikeFromChip::get_label() const
{
	return m_label;
}

void SpikeFromChip::set_label(SpikeLabel const value)
{
	m_label = value;
}

FPGATime SpikeFromChip::get_fpga_time() const
{
	return m_fpga_time;
}

void SpikeFromChip::set_fpga_time(FPGATime const value)
{
	m_fpga_time = value;
}

ChipTime SpikeFromChip::get_chip_time() const
{
	return m_chip_time;
}

void SpikeFromChip::set_chip_time(ChipTime const value)
{
	m_chip_time = value;
}

bool SpikeFromChip::operator==(SpikeFromChip const& other) const
{
	return m_label == other.m_label && m_fpga_time == other.m_fpga_time &&
	       m_chip_time == other.m_chip_time;
}

bool SpikeFromChip::operator!=(SpikeFromChip const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, SpikeFromChip const& spike)
{
	return (
	    os << "SpikeFromChip(" << spike.m_label << ", " << spike.m_fpga_time << ", "
	       << spike.m_chip_time << ")");
}

template <typename Archive>
void SpikeFromChip::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_label));
	ar(CEREAL_NVP(m_fpga_time));
	ar(CEREAL_NVP(m_chip_time));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(SpikeFromChip)

} // namespace haldls::vx

CEREAL_CLASS_VERSION(haldls::vx::SpikePack1ToChip, 0)
CEREAL_CLASS_VERSION(haldls::vx::SpikePack2ToChip, 0)
CEREAL_CLASS_VERSION(haldls::vx::SpikePack3ToChip, 0)
CEREAL_CLASS_VERSION(haldls::vx::SpikeFromChip, 0)
