#include <cctype>
#include <iomanip>
#include <utility>

#include "fisch/vx/jtag.h"

#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/print.h"
#include "haldls/vx/systime.h"

#include "halco/common/cerealization_geometry.h"
#include "haldls/cerealization.h"

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
	os << config.m_value << std::endl;
	return print_words_for_each_backend(os, config);
}

template <typename AddressT>
std::array<AddressT, SystimeSyncBase::config_size_in_words> SystimeSyncBase::addresses(
    coordinate_type const& /*coord*/) const
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
        coordinate_type const& coord) const;

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipAddress, SystimeSyncBase::config_size_in_words>
    SystimeSyncBase::addresses<halco::hicann_dls::vx::OmnibusChipAddress>(
        coordinate_type const& coord) const;

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

template SYMBOL_VISIBLE std::array<fisch::vx::OmnibusChip, SystimeSyncBase::config_size_in_words>
SystimeSyncBase::encode<fisch::vx::OmnibusChip>() const;

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

template SYMBOL_VISIBLE void SystimeSyncBase::decode<fisch::vx::OmnibusChip>(
    std::array<fisch::vx::OmnibusChip, SystimeSyncBase::config_size_in_words> const& data);

template <class Archive>
void SystimeSyncBase::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(SystimeSyncBase)

} // namespace vx
} // namespace haldls
