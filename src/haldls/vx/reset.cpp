#include "haldls/vx/reset.h"
#include "haldls/cerealization.h"

namespace haldls::vx {

ResetChip::ResetChip() : m_value() {}

ResetChip::ResetChip(bool const value) : m_value()
{
	m_value.set(value);
}

void ResetChip::set(bool const value)
{
	m_value.set(value);
}

bool ResetChip::operator==(ResetChip const& other) const
{
	return m_value == other.m_value;
}

bool ResetChip::operator!=(ResetChip const& other) const
{
	return !(*this == other);
}

std::array<halco::hicann_dls::vx::ResetChipOnDLS, ResetChip::config_size_in_words>
ResetChip::addresses(coordinate_type const& coord) const
{
	return {coord};
}

std::array<fisch::vx::ResetChip, ResetChip::config_size_in_words> ResetChip::encode() const
{
	return {m_value};
}

void ResetChip::decode(
    std::array<fisch::vx::ResetChip, ResetChip::config_size_in_words> const& /*data*/)
{}

template <class Archive>
void ResetChip::cerealize(Archive& ar)
{
	ar(CEREAL_NVP(m_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ResetChip)

} // namespace haldls::vx
