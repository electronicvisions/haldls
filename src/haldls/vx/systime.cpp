#include "haldls/vx/systime.h"

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"
#include "fisch/vx/systime.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/cerealization.tcc"
#include "haldls/vx/omnibus_constants.h"

namespace haldls {
namespace vx {

SystimeSyncBase::SystimeSyncBase(Value value) : m_value(value) {}

SystimeSyncBase::Value SystimeSyncBase::get_value() const
{
	return m_value;
}

void SystimeSyncBase::set_value(SystimeSyncBase::Value const& value)
{
	m_value = value;
}

bool SystimeSyncBase::operator==(SystimeSyncBase const& other) const
{
	return m_value == other.m_value;
}

bool SystimeSyncBase::operator!=(SystimeSyncBase const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, SystimeSyncBase const& config)
{
	os << "SystimeSyncBase(" << config.m_value << ")";
	return os;
}

template <typename AddressT>
std::array<AddressT, SystimeSyncBase::config_size_in_words> SystimeSyncBase::addresses(
    coordinate_type const& /*coord*/)
{
	return {AddressT(systime_base_address), AddressT(systime_base_address + 1)};
}

namespace {

struct SystimeSyncBaseBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct {
			uint64_t low : 32;
			uint64_t high : 11;
			uint64_t /* unused */ : 21;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	SystimeSyncBaseBitfield() { u.raw = 0ul; }

	SystimeSyncBaseBitfield(uint64_t data) { u.raw = data; }
};

} // namespace


template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, SystimeSyncBase::config_size_in_words>
    SystimeSyncBase::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
        coordinate_type const& coord);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, SystimeSyncBase::config_size_in_words>
    SystimeSyncBase::addresses<halco::hicann_dls::vx::OmnibusAddress>(coordinate_type const& coord);

template <typename WordT>
std::array<WordT, SystimeSyncBase::config_size_in_words> SystimeSyncBase::encode() const
{
	SystimeSyncBaseBitfield bitfield(m_value);

	return {WordT(fisch::vx::OmnibusData(bitfield.u.m.low)),
	        WordT(fisch::vx::OmnibusData(bitfield.u.m.high))};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, SystimeSyncBase::config_size_in_words>
    SystimeSyncBase::encode<fisch::vx::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE std::array<fisch::vx::Omnibus, SystimeSyncBase::config_size_in_words>
SystimeSyncBase::encode<fisch::vx::Omnibus>() const;

template <typename WordT>
void SystimeSyncBase::decode(std::array<WordT, SystimeSyncBase::config_size_in_words> const& data)
{
	SystimeSyncBaseBitfield bitfield;
	bitfield.u.m.low = data[0].get();
	bitfield.u.m.high = data[1].get();

	m_value = Value(bitfield.u.raw);
}

template SYMBOL_VISIBLE void SystimeSyncBase::decode<fisch::vx::OmnibusChipOverJTAG>(
    std::array<fisch::vx::OmnibusChipOverJTAG, SystimeSyncBase::config_size_in_words> const& data);

template SYMBOL_VISIBLE void SystimeSyncBase::decode<fisch::vx::Omnibus>(
    std::array<fisch::vx::Omnibus, SystimeSyncBase::config_size_in_words> const& data);

template <class Archive>
void SystimeSyncBase::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(SystimeSyncBase)


SystimeSync::SystimeSync(bool const value) : m_do_sync(value) {}

bool SystimeSync::get_do_sync() const
{
	return m_do_sync;
}

void SystimeSync::set_do_sync(bool const value)
{
	m_do_sync = value;
}

bool SystimeSync::operator==(SystimeSync const& other) const
{
	return m_do_sync == other.m_do_sync;
}

bool SystimeSync::operator!=(SystimeSync const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, SystimeSync const& config)
{
	std::stringstream ss;
	ss << "SystimeSync(do_sync: " << std::boolalpha << config.m_do_sync << ")";
	return (os << ss.str());
}

std::array<SystimeSync::coordinate_type, SystimeSync::read_config_size_in_words>
SystimeSync::read_addresses(coordinate_type const& /*coord*/)
{
	return {};
}

std::array<SystimeSync::coordinate_type, SystimeSync::write_config_size_in_words>
SystimeSync::write_addresses(coordinate_type const& coord)
{
	return {coord};
}

std::array<fisch::vx::SystimeSync, SystimeSync::write_config_size_in_words> SystimeSync::encode()
    const
{
	return {fisch::vx::SystimeSync(m_do_sync)};
}

void SystimeSync::decode(
    std::array<fisch::vx::SystimeSync, SystimeSync::read_config_size_in_words> const& /*data*/)
{}

template <class Archive>
void SystimeSync::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_do_sync));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(SystimeSync)

} // namespace vx
} // namespace haldls

CEREAL_CLASS_VERSION(haldls::vx::SystimeSyncBase, 0)
CEREAL_CLASS_VERSION(haldls::vx::SystimeSync, 0)
