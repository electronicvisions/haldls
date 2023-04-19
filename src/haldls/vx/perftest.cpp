#include "haldls/vx/perftest.h"

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/container.tcc"
#include "haldls/vx/omnibus_constants.h"

namespace haldls::vx {

PerfTest::PerfTest(bool const value) : m_enable(value) {}

void PerfTest::set_enable(bool const value)
{
	m_enable = value;
}

bool PerfTest::get_enable() const
{
	return m_enable;
}

bool PerfTest::operator==(PerfTest const& other) const
{
	return m_enable == other.m_enable;
}

bool PerfTest::operator!=(PerfTest const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, PerfTest const& config)
{
	std::stringstream ss;
	ss << "PerfTest(enable: " << std::boolalpha << config.m_enable << ")";
	return (os << ss.str());
}

std::array<halco::hicann_dls::vx::OmnibusAddress, PerfTest::config_size_in_words>
PerfTest::addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::OmnibusAddress(perftest_omnibus_mask)};
}

std::array<fisch::vx::word_access_type::Omnibus, PerfTest::config_size_in_words> PerfTest::encode()
    const
{
	return {fisch::vx::word_access_type::Omnibus(m_enable)};
}

void PerfTest::decode(
    std::array<fisch::vx::word_access_type::Omnibus, PerfTest::config_size_in_words> const& data)
{
	m_enable = (data[0] & 0x1);
}


PerfTestStatus::PerfTestStatus() : m_sent(), m_received(), m_in_order(), m_error_word() {}

void PerfTestStatus::set_sent(Sent const value)
{
	m_sent = value;
}

PerfTestStatus::Sent PerfTestStatus::get_sent() const
{
	return m_sent;
}

void PerfTestStatus::set_received(Received const value)
{
	m_received = value;
}

PerfTestStatus::Received PerfTestStatus::get_received() const
{
	return m_received;
}

void PerfTestStatus::set_in_order(InOrder const value)
{
	m_in_order = value;
}

PerfTestStatus::InOrder PerfTestStatus::get_in_order() const
{
	return m_in_order;
}

void PerfTestStatus::set_error_word(ErrorWord const value)
{
	m_error_word = value;
}

PerfTestStatus::ErrorWord PerfTestStatus::get_error_word() const
{
	return m_error_word;
}

bool PerfTestStatus::operator==(PerfTestStatus const& other) const
{
	return (m_sent == other.m_sent)
	    && (m_received == other.m_received)
	    && (m_in_order == other.m_in_order)
	    && (m_error_word == other.m_error_word);
}

bool PerfTestStatus::operator!=(PerfTestStatus const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, PerfTestStatus const& config)
{
	os << "PerfTestStatus(" << config.m_sent << ", " << config.m_received << ", "
	   << config.m_in_order << ", " << config.m_error_word << ")";
	return os;
}

std::array<halco::hicann_dls::vx::OmnibusAddress, PerfTestStatus::read_config_size_in_words>
PerfTestStatus::read_addresses(coordinate_type const& /*coord*/)
{
	auto base = perftest_omnibus_mask;
	return {halco::hicann_dls::vx::OmnibusAddress(base + 1),
	        halco::hicann_dls::vx::OmnibusAddress(base + 2),
	        halco::hicann_dls::vx::OmnibusAddress(base + 3),
	        halco::hicann_dls::vx::OmnibusAddress(base + 4)};
}

std::array<halco::hicann_dls::vx::OmnibusAddress, PerfTestStatus::write_config_size_in_words>
PerfTestStatus::write_addresses(coordinate_type const& /*coord*/)
{
	return {};
}

std::array<fisch::vx::word_access_type::Omnibus, PerfTestStatus::write_config_size_in_words>
PerfTestStatus::encode() const
{
	return {};
}

void PerfTestStatus::decode(std::array<
                            fisch::vx::word_access_type::Omnibus,
                            PerfTestStatus::read_config_size_in_words> const& data)
{
	m_sent = Sent(data[0]);
	m_received = Received(data[1]);
	m_in_order = InOrder(data[2]);
	m_error_word = ErrorWord(data[3]);
}

} // namespace haldls::vx

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::PerfTest)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::PerfTestStatus)
