#include "haldls/vx/arq.h"

#include <string>

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/container.tcc"
#include "haldls/vx/omnibus_constants.h"

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

std::array<halco::hicann_dls::vx::OmnibusAddress, HicannARQStatus::read_config_size_in_words>
HicannARQStatus::read_addresses(HicannARQStatus::coordinate_type const& /*coord*/)
{
	std::array<halco::hicann_dls::vx::OmnibusAddress, HicannARQStatus::read_config_size_in_words>
	    data;
	size_t i = 0;
	std::generate(data.begin(), data.end(), [&]() {
		return static_cast<halco::hicann_dls::vx::OmnibusAddress>(
		    hicann_arq_status_base_address + i++);
	});
	return data;
}

std::array<halco::hicann_dls::vx::OmnibusAddress, HicannARQStatus::write_config_size_in_words>
HicannARQStatus::write_addresses(HicannARQStatus::coordinate_type const& /*coord*/)
{
	return {};
}

std::array<fisch::vx::word_access_type::Omnibus, HicannARQStatus::write_config_size_in_words>
HicannARQStatus::encode() const
{
	return {};
}

void HicannARQStatus::decode(std::array<
                             fisch::vx::word_access_type::Omnibus,
                             HicannARQStatus::read_config_size_in_words> const& data)
{
	m_write_count = WriteCount(data.at(0));
	m_tx_count = TxCount(data.at(1));
	m_rx_count = RxCount(data.at(2));
	m_read_count = ReadCount(data.at(3));
}

std::ostream& operator<<(std::ostream& os, HicannARQStatus const& config)
{
	os << "HicannARQStatus(" << config.m_read_count << ", " << config.m_write_count << ", "
	   << config.m_rx_count << ", " << config.m_tx_count << ")";
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

} // namespace vx
} // namespace haldls

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::HicannARQStatus)
