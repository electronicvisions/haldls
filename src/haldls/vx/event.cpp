#include "haldls/vx/event.h"

#include <cereal/types/array.hpp>

#include "halco/common/cerealization_geometry.h"
#include "haldls/cerealization.tcc"
#include "hate/join.h"
#include "hate/math.h"

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

halco::hicann_dls::vx::NeuronEventOutputOnDLS SpikeLabel::get_neuron_event_output() const
{
	return halco::hicann_dls::vx::NeuronEventOutputOnDLS(
	    halco::common::Enum((value() >> hate::math::num_bits(NeuronBackendAddressOut::max)) & 0x7));
}

void SpikeLabel::set_neuron_event_output(halco::hicann_dls::vx::NeuronEventOutputOnDLS const value)
{
	operator=(SpikeLabel(
	    (static_cast<uint16_t>(value.toEnum())
	     << hate::math::num_bits(NeuronBackendAddressOut::max)) |
	    (this->value() & (~0x700))));
}

NeuronBackendAddressOut SpikeLabel::get_neuron_backend_address_out() const
{
	return NeuronBackendAddressOut(value() & 0xff);
}

void SpikeLabel::set_neuron_backend_address_out(NeuronBackendAddressOut const value)
{
	operator=(SpikeLabel((static_cast<uint16_t>(value) & 0xff) | (this->value() & 0xff00)));
}

PADIEvent::RowSelectAddress SpikeLabel::get_row_select_address() const
{
	return PADIEvent::RowSelectAddress((value() & (static_cast<uint16_t>(0b11111) << 6)) >> 6);
}

void SpikeLabel::set_row_select_address(PADIEvent::RowSelectAddress const value)
{
	operator=(SpikeLabel(
	    (static_cast<uint16_t>(value) << 6) |
	    (this->value() & ~(static_cast<uint16_t>(0b11111) << 6))));
}

SynapseLabelValue SpikeLabel::get_synapse_label() const
{
	return SynapseLabelValue(value() & 0x3f);
}

void SpikeLabel::set_synapse_label(SynapseLabelValue const value)
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
	void SpikePack##Num##ToChip::set_labels(labels_type const& value)                              \
	{                                                                                              \
		m_impl = value;                                                                            \
	}                                                                                              \
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
	    SpikePack##Num##ToChip::write_config_size_in_words>                                        \
	    SpikePack##Num##ToChip::write_addresses(coordinate_type const& coord)                      \
	{                                                                                              \
		return {coord};                                                                            \
	}                                                                                              \
                                                                                                   \
	std::array<                                                                                    \
	    halco::hicann_dls::vx::SpikePack##Num##ToChipOnDLS,                                        \
	    SpikePack##Num##ToChip::read_config_size_in_words>                                         \
	    SpikePack##Num##ToChip::read_addresses(coordinate_type const& /*coord*/)                   \
	{                                                                                              \
		return {};                                                                                 \
	}                                                                                              \
                                                                                                   \
	std::array<                                                                                    \
	    fisch::vx::word_access_type::SpikePack##Num##ToChip,                                       \
	    SpikePack##Num##ToChip::write_config_size_in_words>                                        \
	    SpikePack##Num##ToChip::encode() const                                                     \
	{                                                                                              \
		fisch::vx::word_access_type::SpikePack##Num##ToChip ret;                                   \
		std::transform(                                                                            \
		    std::begin(m_impl), std::end(m_impl), std::begin(ret),                                 \
		    [](SpikeLabel const& sl) { return fisch::vx::word_access_type::SpikeLabel{sl}; });     \
		return {ret};                                                                              \
	}                                                                                              \
                                                                                                   \
	void SpikePack##Num##ToChip::decode(std::array<                                                \
	                                    fisch::vx::word_access_type::SpikePack##Num##ToChip,       \
	                                    read_config_size_in_words> const& /* data */)              \
	{}                                                                                             \
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

SpikeFromChip::SpikeFromChip(fisch::vx::SpikeFromChip const& data) :
    m_label(data.label), m_fpga_time(data.fpga_time), m_chip_time(data.chip_time)
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


MADCSampleFromChip::MADCSampleFromChip() : m_value(), m_channel(), m_fpga_time(), m_chip_time() {}

MADCSampleFromChip::MADCSampleFromChip(
    Value const& value,
    Channel const& channel,
    FPGATime const& fpga_time,
    ChipTime const& chip_time) :
    m_value(value), m_channel(channel), m_fpga_time(fpga_time), m_chip_time(chip_time)
{}

MADCSampleFromChip::MADCSampleFromChip(fisch::vx::MADCSampleFromChip const& data) :
    m_value(data.value),
    m_channel(data.channel),
    m_fpga_time(data.fpga_time),
    m_chip_time(data.chip_time)
{}

MADCSampleFromChip::Value MADCSampleFromChip::get_value() const
{
	return m_value;
}

void MADCSampleFromChip::set_value(MADCSampleFromChip::Value const value)
{
	m_value = value;
}

MADCSampleFromChip::Channel MADCSampleFromChip::get_channel() const
{
	return m_channel;
}

void MADCSampleFromChip::set_channel(MADCSampleFromChip::Channel const channel)
{
	m_channel = channel;
}

FPGATime MADCSampleFromChip::get_fpga_time() const
{
	return m_fpga_time;
}

void MADCSampleFromChip::set_fpga_time(FPGATime const value)
{
	m_fpga_time = value;
}

ChipTime MADCSampleFromChip::get_chip_time() const
{
	return m_chip_time;
}

void MADCSampleFromChip::set_chip_time(ChipTime const value)
{
	m_chip_time = value;
}

bool MADCSampleFromChip::operator==(MADCSampleFromChip const& other) const
{
	return m_value == other.m_value && m_channel == other.m_channel &&
	       m_fpga_time == other.m_fpga_time && m_chip_time == other.m_chip_time;
}

bool MADCSampleFromChip::operator!=(MADCSampleFromChip const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, MADCSampleFromChip const& sample)
{
	return (
	    os << "MADCSampleFromChip(" << sample.m_value << ", " << sample.m_channel << ", "
	       << sample.m_fpga_time << ", " << sample.m_chip_time << ")");
}

template <typename Archive>
void MADCSampleFromChip::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_value));
	ar(CEREAL_NVP(m_channel));
	ar(CEREAL_NVP(m_fpga_time));
	ar(CEREAL_NVP(m_chip_time));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(MADCSampleFromChip)


HighspeedLinkNotification::HighspeedLinkNotification(
    fisch::vx::HighspeedLinkNotification const& data)
{
	m_fpga_time = data.get_fpga_time();
	auto const value = data.get_value().value();
	m_phy = halco::hicann_dls::vx::PhyStatusOnFPGA((value & 0b11100000) >> 5);
	m_link_up = value & 0b00010000;
	m_decode_error = value & 0b000001000;
	m_crc_error = value & 0b00000100;
	m_crc_recover = value & 0b00000010;
	m_check_error = value & 0b00000001;
}

halco::hicann_dls::vx::PhyStatusOnFPGA HighspeedLinkNotification::get_phy() const
{
	return m_phy;
}

void HighspeedLinkNotification::set_phy(halco::hicann_dls::vx::PhyStatusOnFPGA const value)
{
	m_phy = value;
}

bool HighspeedLinkNotification::get_link_up() const
{
	return m_link_up;
}

void HighspeedLinkNotification::set_link_up(bool const value)
{
	m_link_up = value;
}

bool HighspeedLinkNotification::get_decode_error() const
{
	return m_decode_error;
}

void HighspeedLinkNotification::set_decode_error(bool const value)
{
	m_decode_error = value;
}

bool HighspeedLinkNotification::get_crc_error() const
{
	return m_crc_error;
}

void HighspeedLinkNotification::set_crc_error(bool const value)
{
	m_crc_error = value;
}

bool HighspeedLinkNotification::get_crc_recover() const
{
	return m_crc_recover;
}

void HighspeedLinkNotification::set_crc_recover(bool const value)
{
	m_crc_recover = value;
}

void HighspeedLinkNotification::set_check_error(bool const value)
{
	m_check_error = value;
}

bool HighspeedLinkNotification::get_check_error() const
{
	return m_check_error;
}

FPGATime HighspeedLinkNotification::get_fpga_time() const
{
	return m_fpga_time;
}

void HighspeedLinkNotification::set_fpga_time(FPGATime const value)
{
	m_fpga_time = value;
}

bool HighspeedLinkNotification::operator==(HighspeedLinkNotification const& other) const
{
	return m_phy == other.m_phy && m_fpga_time == other.m_fpga_time &&
	       m_link_up == other.m_link_up && m_decode_error == other.m_decode_error &&
	       m_crc_error == other.m_crc_error && m_crc_recover == other.m_crc_recover &&
	       m_check_error == other.m_check_error;
}

bool HighspeedLinkNotification::operator!=(HighspeedLinkNotification const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, HighspeedLinkNotification const& sample)
{
	std::vector<std::string> active_notifications;
	if (sample.m_link_up) {
		active_notifications.push_back("link up");
	}
	if (sample.m_decode_error) {
		active_notifications.push_back("decode error");
	}
	if (sample.m_crc_error) {
		active_notifications.push_back("crc error");
	}
	if (sample.m_crc_recover) {
		active_notifications.push_back("crc recover");
	}
	if (sample.m_check_error) {
		active_notifications.push_back("check error");
	}

	std::stringstream ss;
	ss << "HighspeedLinkNotification(";
	ss << sample.m_phy << ", " << sample.m_fpga_time << ": ";
	hate::join(ss, active_notifications, ", ");
	ss << ")";
	return (os << ss.str());
}

template <typename Archive>
void HighspeedLinkNotification::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_phy));
	ar(CEREAL_NVP(m_link_up));
	ar(CEREAL_NVP(m_decode_error));
	ar(CEREAL_NVP(m_crc_error));
	ar(CEREAL_NVP(m_crc_recover));
	ar(CEREAL_NVP(m_check_error));
	ar(CEREAL_NVP(m_fpga_time));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(HighspeedLinkNotification)

} // namespace haldls::vx

CEREAL_CLASS_VERSION(haldls::vx::SpikePack1ToChip, 0)
CEREAL_CLASS_VERSION(haldls::vx::SpikePack2ToChip, 0)
CEREAL_CLASS_VERSION(haldls::vx::SpikePack3ToChip, 0)
CEREAL_CLASS_VERSION(haldls::vx::SpikeFromChip, 0)
CEREAL_CLASS_VERSION(haldls::vx::HighspeedLinkNotification, 0)
