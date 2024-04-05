#include "haldls/vx/event.h"

#include "haldls/vx/container.tcc"
#include "hate/join.h"

namespace haldls::vx {

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
		os << "SpikePack" << #Num << "ToChip(" << hate::join(spike_pack.m_impl, ", ") << ")";      \
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
		    [](halco::hicann_dls::vx::SpikeLabel const& sl) {                                      \
			    return halco::hicann_dls::vx::SpikeLabel{sl};                                      \
		    });                                                                                    \
		return {ret};                                                                              \
	}                                                                                              \
                                                                                                   \
	void SpikePack##Num##ToChip::decode(std::array<                                                \
	                                    fisch::vx::word_access_type::SpikePack##Num##ToChip,       \
	                                    read_config_size_in_words> const& /* data */)              \
	{}


// clang-format off
SpikePackToChip(1)
SpikePackToChip(2)
SpikePackToChip(3)
// clang-format on

#undef SpikePackToChip


bool SpikeFromChip::operator==(SpikeFromChip const& other) const
{
	return label == other.label && fpga_time == other.fpga_time && chip_time == other.chip_time;
}

bool SpikeFromChip::operator!=(SpikeFromChip const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, SpikeFromChip const& spike)
{
	return (
	    os << "SpikeFromChip(" << spike.label << ", " << spike.fpga_time << ", " << spike.chip_time
	       << ")");
}


bool MADCSampleFromChip::operator==(MADCSampleFromChip const& other) const
{
	return value == other.value && channel == other.channel && fpga_time == other.fpga_time &&
	       chip_time == other.chip_time;
}

bool MADCSampleFromChip::operator!=(MADCSampleFromChip const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, MADCSampleFromChip const& sample)
{
	return (
	    os << "MADCSampleFromChip(" << sample.value << ", " << sample.channel << ", "
	       << sample.fpga_time << ", " << sample.chip_time << ")");
}


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
	ss << hate::join(active_notifications, ", ");
	ss << ")";
	return (os << ss.str());
}

} // namespace haldls::vx

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::SpikePack1ToChip)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::SpikePack2ToChip)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::SpikePack3ToChip)
