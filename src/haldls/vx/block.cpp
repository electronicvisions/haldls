#include "haldls/vx/block.h"

#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/cerealization.tcc"
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

std::array<fisch::vx::Omnibus, PollingOmnibusBlockConfig::config_size_in_words>
PollingOmnibusBlockConfig::encode() const
{
	return {fisch::vx::Omnibus(fisch::vx::Omnibus::Value(m_address)),
	        fisch::vx::Omnibus(fisch::vx::Omnibus::Value(m_target)),
	        fisch::vx::Omnibus(fisch::vx::Omnibus::Value(m_mask))};
}

void PollingOmnibusBlockConfig::decode(
    std::array<fisch::vx::Omnibus, PollingOmnibusBlockConfig::config_size_in_words> const& data)
{
	m_address = Address(data[0].get());
	m_target = Value(data[1].get());
	m_mask = Value(data[2].get());
}

template <class Archive>
void PollingOmnibusBlockConfig::serialize(Archive& ar, std::uint32_t)
{
	ar(CEREAL_NVP(m_address));
	ar(CEREAL_NVP(m_mask));
	ar(CEREAL_NVP(m_target));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(PollingOmnibusBlockConfig)

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
	return (os << config.encode());
}

fisch::vx::PollingOmnibusBlock PollingOmnibusBlock::encode() const
{
	return fisch::vx::PollingOmnibusBlock(
	    fisch::vx::PollingOmnibusBlock::Value(m_enable_expects_equality));
}

template <class Archive>
void PollingOmnibusBlock::serialize(Archive& ar, std::uint32_t)
{
	ar(CEREAL_NVP(m_enable_expects_equality));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(PollingOmnibusBlock)

} // namespace haldls::vx

CEREAL_CLASS_VERSION(haldls::vx::PollingOmnibusBlockConfig, 0)
