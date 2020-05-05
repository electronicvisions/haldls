#include "haldls/vx/arq.h"

#include <string>

#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/cerealization.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/print.h"

namespace haldls {
namespace vx {

HicannARQStatus::HicannARQStatus() : m_read_count(), m_write_count(), m_rx_count(), m_tx_count() {}

HicannARQStatus::ReadCount HicannARQStatus::get_read_count() const
{
	return m_read_count;
}
void HicannARQStatus::set_read_count(ReadCount const value)
{
	m_read_count = value;
}

HicannARQStatus::WriteCount HicannARQStatus::get_write_count() const
{
	return m_write_count;
}
void HicannARQStatus::set_write_count(WriteCount const value)
{
	m_write_count = value;
}

HicannARQStatus::RxCount HicannARQStatus::get_rx_count() const
{
	return m_rx_count;
}
void HicannARQStatus::set_rx_count(RxCount const value)
{
	m_rx_count = value;
}

HicannARQStatus::TxCount HicannARQStatus::get_tx_count() const
{
	return m_tx_count;
}
void HicannARQStatus::set_tx_count(TxCount const value)
{
	m_tx_count = value;
}

std::array<halco::hicann_dls::vx::OmnibusFPGAAddress, HicannARQStatus::read_config_size_in_words>
HicannARQStatus::read_addresses(HicannARQStatus::coordinate_type const& /*coord*/)
{
	std::array<
	    halco::hicann_dls::vx::OmnibusFPGAAddress, HicannARQStatus::read_config_size_in_words>
	    data;
	size_t i = 0;
	std::generate(data.begin(), data.end(), [&]() {
		return static_cast<halco::hicann_dls::vx::OmnibusFPGAAddress>(
		    hicann_arq_status_base_address + i++);
	});
	return data;
}

std::array<halco::hicann_dls::vx::OmnibusFPGAAddress, HicannARQStatus::write_config_size_in_words>
HicannARQStatus::write_addresses(HicannARQStatus::coordinate_type const& /*coord*/)
{
	return {};
}

std::array<fisch::vx::OmnibusFPGA, HicannARQStatus::write_config_size_in_words>
HicannARQStatus::encode() const
{
	return {};
}

void HicannARQStatus::decode(
    std::array<fisch::vx::OmnibusFPGA, HicannARQStatus::read_config_size_in_words> const& data)
{
	m_write_count = WriteCount(data.at(0).get());
	m_tx_count = TxCount(data.at(1).get());
	m_rx_count = RxCount(data.at(2).get());
	m_read_count = ReadCount(data.at(3).get());
}

std::ostream& operator<<(std::ostream& os, HicannARQStatus const& config)
{
	os << config.m_read_count << std::endl;
	os << config.m_write_count << std::endl;
	os << config.m_rx_count << std::endl;
	os << config.m_tx_count;
	return os;
}

bool HicannARQStatus::operator==(HicannARQStatus const& other) const
{
	return (
	    (m_read_count == other.m_read_count) && (m_write_count == other.m_write_count) &&
	    (m_rx_count == other.m_rx_count) && (m_tx_count == other.m_tx_count));
}

bool HicannARQStatus::operator!=(HicannARQStatus const& other) const
{
	return !(*this == other);
}

template <class Archive>
void HicannARQStatus::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_read_count));
	ar(CEREAL_NVP(m_write_count));
	ar(CEREAL_NVP(m_rx_count));
	ar(CEREAL_NVP(m_tx_count));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(HicannARQStatus)

} // namespace vx
} // namespace haldls

CEREAL_CLASS_VERSION(haldls::vx::HicannARQStatus, 0)
