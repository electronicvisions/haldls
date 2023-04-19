#include "haldls/vx/reset.h"


#include "fisch/vx/word_access/type/reset.h"
#include "haldls/vx/container.tcc"

namespace haldls::vx {

ResetChip::ResetChip(bool const value) : m_value(value) {}

void ResetChip::set(bool const value)
{
	m_value = value;
}

bool ResetChip::operator==(ResetChip const& other) const
{
	return m_value == other.m_value;
}

bool ResetChip::operator!=(ResetChip const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ResetChip const& config)
{
	std::stringstream ss;
	ss << "ResetChip(" << std::boolalpha << config.m_value << ")";
	os << ss.str();
	return os;
}

std::array<halco::hicann_dls::vx::ResetChipOnDLS, ResetChip::write_config_size_in_words>
ResetChip::write_addresses(coordinate_type const& coord)
{
	return {coord};
}

std::array<halco::hicann_dls::vx::ResetChipOnDLS, ResetChip::read_config_size_in_words>
ResetChip::read_addresses(coordinate_type const& /*coord*/)
{
	return {};
}

std::array<fisch::vx::word_access_type::ResetChip, ResetChip::write_config_size_in_words>
ResetChip::encode() const
{
	return {fisch::vx::word_access_type::ResetChip(m_value)};
}

void ResetChip::decode(std::array<
                       fisch::vx::word_access_type::ResetChip,
                       ResetChip::read_config_size_in_words> const& /*data*/)
{}

} // namespace haldls::vx

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::ResetChip)
