#include "haldls/vx/jtag.h"
#include "haldls/cerealization.h"
#include "haldls/vx/print.h"

namespace haldls::vx {

JTAGClockScaler::JTAGClockScaler(Value const value) : m_value(value) {}

void JTAGClockScaler::set(Value const value)
{
	m_value.set(value);
}

bool JTAGClockScaler::operator==(JTAGClockScaler const& other) const
{
	return m_value == other.m_value;
}

bool JTAGClockScaler::operator!=(JTAGClockScaler const& other) const
{
	return !(*this == other);
}

HALDLS_VX_DEFAULT_OSTREAM_OP(JTAGClockScaler)

std::array<halco::hicann_dls::vx::JTAGClockScalerOnDLS, JTAGClockScaler::config_size_in_words>
JTAGClockScaler::addresses(coordinate_type const& coord)
{
	return {coord};
}

std::array<fisch::vx::JTAGClockScaler, JTAGClockScaler::config_size_in_words>
JTAGClockScaler::encode() const
{
	return {m_value};
}

void JTAGClockScaler::decode(
    std::array<fisch::vx::JTAGClockScaler, JTAGClockScaler::config_size_in_words> const& /*data*/)
{}

template <class Archive>
void JTAGClockScaler::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(JTAGClockScaler)


ResetJTAGTap::ResetJTAGTap() : m_value() {}

bool ResetJTAGTap::operator==(ResetJTAGTap const& other) const
{
	return m_value == other.m_value;
}

bool ResetJTAGTap::operator!=(ResetJTAGTap const& other) const
{
	return !(*this == other);
}

HALDLS_VX_DEFAULT_OSTREAM_OP(ResetJTAGTap)

std::array<halco::hicann_dls::vx::ResetJTAGTapOnDLS, ResetJTAGTap::config_size_in_words>
ResetJTAGTap::addresses(coordinate_type const& coord)
{
	return {coord};
}

std::array<fisch::vx::ResetJTAGTap, ResetJTAGTap::config_size_in_words> ResetJTAGTap::encode() const
{
	return {m_value};
}

void ResetJTAGTap::decode(
    std::array<fisch::vx::ResetJTAGTap, ResetJTAGTap::config_size_in_words> const& /*data*/)
{}

template <class Archive>
void ResetJTAGTap::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ResetJTAGTap)


JTAGIdCode::JTAGIdCode() : m_value() {}

JTAGIdCode::Value JTAGIdCode::get() const
{
	return Value(m_value.get());
}

bool JTAGIdCode::operator==(JTAGIdCode const& other) const
{
	return m_value == other.m_value;
}

bool JTAGIdCode::operator!=(JTAGIdCode const& other) const
{
	return !(*this == other);
}

HALDLS_VX_DEFAULT_OSTREAM_OP(JTAGIdCode)

std::array<halco::hicann_dls::vx::JTAGIdCodeOnDLS, JTAGIdCode::read_config_size_in_words>
JTAGIdCode::read_addresses(coordinate_type const& coord)
{
	return {coord};
}

std::array<halco::hicann_dls::vx::JTAGIdCodeOnDLS, JTAGIdCode::write_config_size_in_words>
JTAGIdCode::write_addresses(coordinate_type const& /* coord */)
{
	return {};
}

std::array<fisch::vx::JTAGIdCode, JTAGIdCode::write_config_size_in_words> JTAGIdCode::encode() const
{
	return {};
}

void JTAGIdCode::decode(
    std::array<fisch::vx::JTAGIdCode, JTAGIdCode::read_config_size_in_words> const& data)
{
	m_value = data[0];
}

template <class Archive>
void JTAGIdCode::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(JTAGIdCode)

} // namespace haldls::vx
