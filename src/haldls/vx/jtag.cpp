#include "haldls/vx/jtag.h"

#include "fisch/vx/word_access/type/jtag.h"
#include "haldls/vx/container.tcc"

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

std::array<halco::hicann_dls::vx::JTAGClockScalerOnDLS, JTAGClockScaler::write_config_size_in_words>
JTAGClockScaler::write_addresses(coordinate_type const& coord)
{
	return {coord};
}

std::array<halco::hicann_dls::vx::JTAGClockScalerOnDLS, JTAGClockScaler::read_config_size_in_words>
JTAGClockScaler::read_addresses(coordinate_type const& /*coord*/)
{
	return {};
}

std::
    array<fisch::vx::word_access_type::JTAGClockScaler, JTAGClockScaler::write_config_size_in_words>
    JTAGClockScaler::encode() const
{
	return {fisch::vx::word_access_type::JTAGClockScaler(m_value)};
}

void JTAGClockScaler::decode(std::array<
                             fisch::vx::word_access_type::JTAGClockScaler,
                             JTAGClockScaler::read_config_size_in_words> const& /*data*/)
{}


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

std::array<halco::hicann_dls::vx::ResetJTAGTapOnDLS, ResetJTAGTap::write_config_size_in_words>
ResetJTAGTap::write_addresses(coordinate_type const& coord)
{
	return {coord};
}

std::array<halco::hicann_dls::vx::ResetJTAGTapOnDLS, ResetJTAGTap::read_config_size_in_words>
ResetJTAGTap::read_addresses(coordinate_type const& /*coord*/)
{
	return {};
}

std::array<fisch::vx::word_access_type::ResetJTAGTap, ResetJTAGTap::write_config_size_in_words>
ResetJTAGTap::encode() const
{
	return {fisch::vx::word_access_type::ResetJTAGTap()};
}

void ResetJTAGTap::decode(std::array<
                          fisch::vx::word_access_type::ResetJTAGTap,
                          ResetJTAGTap::read_config_size_in_words> const& /*data*/)
{}


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

std::array<fisch::vx::word_access_type::JTAGIdCode, JTAGIdCode::write_config_size_in_words>
JTAGIdCode::encode() const
{
	return {};
}

namespace {

struct JTAGIdCodeBitfield
{
	union
	{
		fisch::vx::word_access_type::JTAGIdCode::value_type word;
		struct __attribute__((packed))
		{
			fisch::vx::word_access_type::JTAGIdCode::value_type marker : 1;
			fisch::vx::word_access_type::JTAGIdCode::value_type manufacturer_id : 11;
			fisch::vx::word_access_type::JTAGIdCode::value_type part_number : 16;
			fisch::vx::word_access_type::JTAGIdCode::value_type version : 4;
		} m;
		static_assert(sizeof(word) == sizeof(m), "Sizes of union types should match.");
	} u;

	JTAGIdCodeBitfield()
	{
		u.word = fisch::vx::word_access_type::JTAGIdCode(0);
	};

	explicit JTAGIdCodeBitfield(fisch::vx::word_access_type::JTAGIdCode data)
	{
		u.word = data;
	}
};

} // anonymous namespace

void JTAGIdCode::decode(std::array<
                        fisch::vx::word_access_type::JTAGIdCode,
                        JTAGIdCode::read_config_size_in_words> const& data)
{
	JTAGIdCodeBitfield bitfield(data[0]);

	if (!bitfield.u.m.marker) {
		throw std::runtime_error("JTAG ID marker has been decoded as 0b0. This is an invalid "
		                         "response according to the JTAG specification.");
	}

	m_version = Version(bitfield.u.m.version);
	m_part_number = PartNumber(bitfield.u.m.part_number);
	m_manufacturer_id = ManufacturerId(bitfield.u.m.manufacturer_id);
}

} // namespace haldls::vx

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::JTAGClockScaler)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::ResetJTAGTap)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::JTAGIdCode)
