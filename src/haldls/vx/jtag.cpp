#include "haldls/vx/jtag.h"

#include "fisch/vx/jtag.h"
#include "halco/common/cerealization_geometry.h"
#include "haldls/cerealization.tcc"

namespace haldls::vx {

JTAGClockScaler::JTAGClockScaler(Value const value) : m_value(value) {}

void JTAGClockScaler::set(Value const value)
{
	m_value = value;
}

bool JTAGClockScaler::operator==(JTAGClockScaler const& other) const
{
	return m_value == other.m_value;
}

bool JTAGClockScaler::operator!=(JTAGClockScaler const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, JTAGClockScaler const& config)
{
	os << "JTAGClockScaler(" << config.m_value << ")";
	return os;
}

std::array<halco::hicann_dls::vx::JTAGClockScalerOnDLS, JTAGClockScaler::config_size_in_words>
JTAGClockScaler::addresses(coordinate_type const& coord)
{
	return {coord};
}

std::array<fisch::vx::JTAGClockScaler, JTAGClockScaler::config_size_in_words>
JTAGClockScaler::encode() const
{
	return {fisch::vx::JTAGClockScaler(fisch::vx::JTAGClockScaler::Value(m_value))};
}

void JTAGClockScaler::decode(
    std::array<fisch::vx::JTAGClockScaler, JTAGClockScaler::config_size_in_words> const& /*data*/)
{}

template <class Archive>
void JTAGClockScaler::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(JTAGClockScaler)


ResetJTAGTap::ResetJTAGTap() {}

bool ResetJTAGTap::operator==(ResetJTAGTap const&) const
{
	return true;
}

bool ResetJTAGTap::operator!=(ResetJTAGTap const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ResetJTAGTap const&)
{
	os << "ResetJTAGTap()";
	return os;
}

std::array<halco::hicann_dls::vx::ResetJTAGTapOnDLS, ResetJTAGTap::config_size_in_words>
ResetJTAGTap::addresses(coordinate_type const& coord)
{
	return {coord};
}

std::array<fisch::vx::ResetJTAGTap, ResetJTAGTap::config_size_in_words> ResetJTAGTap::encode() const
{
	return {fisch::vx::ResetJTAGTap()};
}

void ResetJTAGTap::decode(
    std::array<fisch::vx::ResetJTAGTap, ResetJTAGTap::config_size_in_words> const& /*data*/)
{}

template <class Archive>
void ResetJTAGTap::serialize(Archive&, std::uint32_t const)
{}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ResetJTAGTap)


JTAGIdCode::JTAGIdCode() : m_version(), m_part_number(), m_manufacturer_id() {}

JTAGIdCode::Version JTAGIdCode::get_version() const
{
	return m_version;
}

JTAGIdCode::PartNumber JTAGIdCode::get_part_number() const
{
	return m_part_number;
}

JTAGIdCode::ManufacturerId JTAGIdCode::get_manufacturer_id() const
{
	return m_manufacturer_id;
}

void JTAGIdCode::set_version(JTAGIdCode::Version const value)
{
	m_version = value;
}

void JTAGIdCode::set_part_number(JTAGIdCode::PartNumber const value)
{
	m_part_number = value;
}

void JTAGIdCode::set_manufacturer_id(JTAGIdCode::ManufacturerId const value)
{
	m_manufacturer_id = value;
}

bool JTAGIdCode::operator==(JTAGIdCode const& other) const
{
	return (
	    m_version == other.m_version && m_part_number == other.m_part_number &&
	    m_manufacturer_id == other.m_manufacturer_id);
}

bool JTAGIdCode::operator!=(JTAGIdCode const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, JTAGIdCode const& config)
{
	os << "JTAGIdCode(" << config.m_version << ", " << config.m_part_number << ", "
	   << config.m_manufacturer_id << ")";
	return os;
}

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

namespace {

struct JTAGIdCodeBitfield
{
	union
	{
		fisch::vx::JTAGIdCode::Value::value_type word;
		struct __attribute__((packed))
		{
			fisch::vx::JTAGIdCode::Value::value_type marker : 1;
			fisch::vx::JTAGIdCode::Value::value_type manufacturer_id : 11;
			fisch::vx::JTAGIdCode::Value::value_type part_number : 16;
			fisch::vx::JTAGIdCode::Value::value_type version : 4;
		} m;
		static_assert(sizeof(word) == sizeof(m), "Sizes of union types should match.");
	} u;

	JTAGIdCodeBitfield()
	{
		u.word = fisch::vx::JTAGIdCode::Value(0);
	};

	explicit JTAGIdCodeBitfield(fisch::vx::JTAGIdCode::Value data)
	{
		u.word = data;
	}
};

} // anonymous namespace

void JTAGIdCode::decode(
    std::array<fisch::vx::JTAGIdCode, JTAGIdCode::read_config_size_in_words> const& data)
{
	JTAGIdCodeBitfield bitfield(data[0].get());

	if (!bitfield.u.m.marker) {
		throw std::runtime_error("JTAG ID marker has been decoded as 0b0. This is an invalid "
		                         "response according to the JTAG specification.");
	}

	m_version = Version(bitfield.u.m.version);
	m_part_number = PartNumber(bitfield.u.m.part_number);
	m_manufacturer_id = ManufacturerId(bitfield.u.m.manufacturer_id);
}

template <class Archive>
void JTAGIdCode::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_version));
	ar(CEREAL_NVP(m_part_number));
	ar(CEREAL_NVP(m_manufacturer_id));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(JTAGIdCode)

} // namespace haldls::vx

CEREAL_CLASS_VERSION(haldls::vx::JTAGClockScaler, 0)
CEREAL_CLASS_VERSION(haldls::vx::ResetJTAGTap, 0)
CEREAL_CLASS_VERSION(haldls::vx::JTAGIdCode, 1)
