#include "haldls/vx/reset.h"


#include "fisch/vx/reset.h"
#include "haldls/cerealization.tcc"

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

std::array<halco::hicann_dls::vx::ResetChipOnDLS, ResetChip::config_size_in_words>
ResetChip::addresses(coordinate_type const& coord)
{
	return {coord};
}

std::array<fisch::vx::ResetChip, ResetChip::config_size_in_words> ResetChip::encode() const
{
	return {fisch::vx::ResetChip(m_value)};
}

void ResetChip::decode(
    std::array<fisch::vx::ResetChip, ResetChip::config_size_in_words> const& /*data*/)
{}

template <class Archive>
void ResetChip::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ResetChip)

} // namespace haldls::vx

CEREAL_CLASS_VERSION(haldls::vx::ResetChip, 0)
