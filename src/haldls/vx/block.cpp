#include "haldls/vx/block.h"

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/block_until.tcc"
#include "haldls/vx/container.tcc"
#include "haldls/vx/omnibus_constants.h"

namespace haldls::vx {

PollingOmnibusBlockConfig::PollingOmnibusBlockConfig() : m_address(), m_mask(), m_target() {}

void PollingOmnibusBlockConfig::set_address(Address const value)
{
	m_address = value;
}

PollingOmnibusBlockConfig::Address PollingOmnibusBlockConfig::get_address() const
{
	return m_address;
}

void PollingOmnibusBlockConfig::set_mask(Value const value)
{
	m_mask = value;
}

PollingOmnibusBlockConfig::Value PollingOmnibusBlockConfig::get_mask() const
{
	return m_mask;
}

void PollingOmnibusBlockConfig::set_target(Value const value)
{
	m_target = value;
}

PollingOmnibusBlockConfig::Value PollingOmnibusBlockConfig::get_target() const
{
	return m_target;
}

bool PollingOmnibusBlockConfig::operator==(PollingOmnibusBlockConfig const& other) const
{
	return (m_address == other.m_address) && (m_mask == other.m_mask) &&
	       (m_target == other.m_target);
}

bool PollingOmnibusBlockConfig::operator!=(PollingOmnibusBlockConfig const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, PollingOmnibusBlockConfig const& config)
{
	os << "PollingOmnibusBlockConfig(address: " << config.m_address << ", mask: " << config.m_mask
	   << ", target: " << config.m_target << ")";
	return os;
}

std::array<halco::hicann_dls::vx::OmnibusAddress, PollingOmnibusBlockConfig::config_size_in_words>
PollingOmnibusBlockConfig::addresses(coordinate_type const& /*coord*/)
{
	return {
	    halco::hicann_dls::vx::OmnibusAddress(executor_omnibus_mask + 6),
	    halco::hicann_dls::vx::OmnibusAddress(executor_omnibus_mask + 7),
	    halco::hicann_dls::vx::OmnibusAddress(executor_omnibus_mask + 8)};
}

std::array<fisch::vx::word_access_type::Omnibus, PollingOmnibusBlockConfig::config_size_in_words>
PollingOmnibusBlockConfig::encode() const
{
	return {
	    fisch::vx::word_access_type::Omnibus(fisch::vx::word_access_type::Omnibus(m_address)),
	    fisch::vx::word_access_type::Omnibus(fisch::vx::word_access_type::Omnibus(m_target)),
	    fisch::vx::word_access_type::Omnibus(fisch::vx::word_access_type::Omnibus(m_mask))};
}

void PollingOmnibusBlockConfig::decode(std::array<
                                       fisch::vx::word_access_type::Omnibus,
                                       PollingOmnibusBlockConfig::config_size_in_words> const& data)
{
	m_address = Address(data[0]);
	m_target = Value(data[1]);
	m_mask = Value(data[2]);
}

PollingOmnibusBlock::PollingOmnibusBlock(bool const enable_expects_equality) :
    m_enable_expects_equality(enable_expects_equality)
{}

void PollingOmnibusBlock::set_enable_expects_equality(bool const value)
{
	m_enable_expects_equality = value;
}

bool PollingOmnibusBlock::get_enable_expects_equality() const
{
	return m_enable_expects_equality;
}

bool PollingOmnibusBlock::operator==(PollingOmnibusBlock const& other) const
{
	return m_enable_expects_equality == other.m_enable_expects_equality;
}

bool PollingOmnibusBlock::operator!=(PollingOmnibusBlock const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, PollingOmnibusBlock const& config)
{
	return (os << config.encode().at(0));
}

std::array<
    halco::hicann_dls::vx::PollingOmnibusBlockOnFPGA,
    PollingOmnibusBlock::write_config_size_in_words>
PollingOmnibusBlock::write_addresses(coordinate_type const& coord)
{
	return {coord};
}

std::array<
    fisch::vx::word_access_type::PollingOmnibusBlock,
    PollingOmnibusBlock::write_config_size_in_words>
PollingOmnibusBlock::encode() const
{
	return {fisch::vx::word_access_type::PollingOmnibusBlock(m_enable_expects_equality)};
}

} // namespace haldls::vx

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::PollingOmnibusBlockConfig)
EXPLICIT_INSTANTIATE_HALDLS_BLOCK_UNTIL_BASE(haldls::vx::PollingOmnibusBlock)
