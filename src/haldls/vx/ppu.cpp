#include <cctype>
#include <iomanip>
#include <utility>

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"

#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/ppu.h"
#include "haldls/vx/print.h"

namespace haldls {
namespace vx {

PPUMemoryWord::PPUMemoryWord() : PPUMemoryWord(PPUMemoryWord::Value(0)) {}

PPUMemoryWord::PPUMemoryWord(PPUMemoryWord::Value const& value) : m_value(value) {}

PPUMemoryWord::Value PPUMemoryWord::get_value() const
{
	return m_value;
}

void PPUMemoryWord::set_value(PPUMemoryWord::Value const& value)
{
	m_value = value;
}

bool PPUMemoryWord::operator==(PPUMemoryWord const& other) const
{
	return m_value == other.get_value();
}

bool PPUMemoryWord::operator!=(PPUMemoryWord const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, PPUMemoryWord const& pmw)
{
	using namespace hate::math;
	uint32_t w = static_cast<uint32_t>(pmw.get_value());
	std::stringstream out;
	out << std::showbase << std::internal << std::setfill('0') << std::hex
	    << std::setw(round_up_integer_division(num_bits(PPUMemoryWord::Value::max), 4)) << w;
	os << out.str() << std::endl;
	return print_words_for_each_backend(os, pmw);
}

template <typename AddressT>
std::array<AddressT, PPUMemoryWord::config_size_in_words> PPUMemoryWord::addresses(
    coordinate_type const& coord) const
{
	uint32_t tmp = coord.toPPUMemoryWordOnPPU().value();
	if (coord.toPPUOnDLS().value() == 0) {
		tmp += top_ppu_base_address;
	} else {
		tmp += bottom_ppu_base_address;
	}
	return {AddressT(tmp)};
}

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, PPUMemoryWord::config_size_in_words>
    PPUMemoryWord::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
        coordinate_type const& coord) const;

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipAddress, PPUMemoryWord::config_size_in_words>
    PPUMemoryWord::addresses<halco::hicann_dls::vx::OmnibusChipAddress>(
        coordinate_type const& coord) const;

template <typename WordT>
std::array<WordT, PPUMemoryWord::config_size_in_words> PPUMemoryWord::encode() const
{
	return {{static_cast<WordT>(typename WordT::value_type(get_value()))}};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, PPUMemoryWord::config_size_in_words>
    PPUMemoryWord::encode<fisch::vx::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE std::array<fisch::vx::OmnibusChip, PPUMemoryWord::config_size_in_words>
PPUMemoryWord::encode<fisch::vx::OmnibusChip>() const;

template <typename WordT>
void PPUMemoryWord::decode(std::array<WordT, PPUMemoryWord::config_size_in_words> const& data)
{
	set_value(Value(data[0].get()));
}

template SYMBOL_VISIBLE void PPUMemoryWord::decode<fisch::vx::OmnibusChipOverJTAG>(
    std::array<fisch::vx::OmnibusChipOverJTAG, PPUMemoryWord::config_size_in_words> const& data);

template SYMBOL_VISIBLE void PPUMemoryWord::decode<fisch::vx::OmnibusChip>(
    std::array<fisch::vx::OmnibusChip, PPUMemoryWord::config_size_in_words> const& data);

template <class Archive>
void PPUMemoryWord::cerealize(Archive& ar)
{
	ar(CEREAL_NVP(m_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(PPUMemoryWord)

PPUControlRegister::PPUControlRegister() :
    m_cache_controller_enable(false),
    m_inhibit_reset(false),
    m_force_clock_on(false),
    m_force_clock_off(false)
{}

bool PPUControlRegister::get_cache_controller_enable() const
{
	return m_cache_controller_enable;
}

void PPUControlRegister::set_cache_controller_enable(bool const value)
{
	m_cache_controller_enable = value;
}

bool PPUControlRegister::get_inhibit_reset() const
{
	return m_inhibit_reset;
}

void PPUControlRegister::set_inhibit_reset(bool const value)
{
	m_inhibit_reset = value;
}

bool PPUControlRegister::get_force_clock_on() const
{
	return m_force_clock_on;
}

void PPUControlRegister::set_force_clock_on(bool const value)
{
	m_force_clock_on = value;
}

bool PPUControlRegister::get_force_clock_off() const
{
	return m_force_clock_off;
}

void PPUControlRegister::set_force_clock_off(bool const value)
{
	m_force_clock_off = value;
}

bool PPUControlRegister::operator==(PPUControlRegister const& other) const
{
	return m_cache_controller_enable == other.get_cache_controller_enable() &&
	       m_inhibit_reset == other.get_inhibit_reset() &&
	       m_force_clock_on == other.get_force_clock_on() &&
	       m_force_clock_off == other.get_force_clock_off();
}

bool PPUControlRegister::operator!=(PPUControlRegister const& other) const
{
	return !(*this == other);
}

namespace {

struct PPUControlRegisterBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t inhibit_reset           :  1; // 0
			uint32_t force_clock_on          :  1; // 1
			uint32_t force_clock_off         :  1; // 2
			uint32_t cache_controller_enable :  1; // 3
			uint32_t                         : 28; // 4-32
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	PPUControlRegisterBitfield() { u.raw = 0u; }

	PPUControlRegisterBitfield(uint32_t data) { u.raw = data & 0b1111ul; }
};

} // namespace

template <typename AddressT>
std::array<AddressT, PPUControlRegister::config_size_in_words> PPUControlRegister::addresses(
    coordinate_type const& coord) const
{
	uint32_t tmp;
	if (coord.toPPUOnDLS().value() == 0) {
		tmp = top_ppu_base_address;
	} else {
		tmp = bottom_ppu_base_address;
	}
	tmp |= ppu_control_register_address_mask;
	return {AddressT{tmp}};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    PPUControlRegister::config_size_in_words>
PPUControlRegister::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord) const;

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipAddress, PPUControlRegister::config_size_in_words>
    PPUControlRegister::addresses<halco::hicann_dls::vx::OmnibusChipAddress>(
        coordinate_type const& coord) const;

template <typename WordT>
std::array<WordT, PPUControlRegister::config_size_in_words> PPUControlRegister::encode() const
{
	PPUControlRegisterBitfield bitfield;
	bitfield.u.m.cache_controller_enable = m_cache_controller_enable;
	bitfield.u.m.inhibit_reset = m_inhibit_reset;
	bitfield.u.m.force_clock_on = m_force_clock_on;
	bitfield.u.m.force_clock_off = m_force_clock_off;
	return {WordT{fisch::vx::OmnibusData(bitfield.u.raw)}};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, PPUControlRegister::config_size_in_words>
    PPUControlRegister::encode<fisch::vx::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE std::array<fisch::vx::OmnibusChip, PPUControlRegister::config_size_in_words>
PPUControlRegister::encode<fisch::vx::OmnibusChip>() const;

template <typename WordT>
void PPUControlRegister::decode(
    std::array<WordT, PPUControlRegister::config_size_in_words> const& data)
{
	PPUControlRegisterBitfield bitfield(static_cast<uint32_t>(data[0].get()));
	m_cache_controller_enable = bitfield.u.m.cache_controller_enable;
	m_inhibit_reset = bitfield.u.m.inhibit_reset;
	m_force_clock_on = bitfield.u.m.force_clock_on;
	m_force_clock_off = bitfield.u.m.force_clock_off;
}

template SYMBOL_VISIBLE void PPUControlRegister::decode<fisch::vx::OmnibusChipOverJTAG>(
    std::array<fisch::vx::OmnibusChipOverJTAG, PPUControlRegister::config_size_in_words> const&
        data);

template SYMBOL_VISIBLE void PPUControlRegister::decode<fisch::vx::OmnibusChip>(
    std::array<fisch::vx::OmnibusChip, PPUControlRegister::config_size_in_words> const& data);

template <class Archive>
void PPUControlRegister::cerealize(Archive& ar)
{
	ar(CEREAL_NVP(m_cache_controller_enable));
	ar(CEREAL_NVP(m_inhibit_reset));
	ar(CEREAL_NVP(m_force_clock_on));
	ar(CEREAL_NVP(m_force_clock_off));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(PPUControlRegister)

PPUStatusRegister::PPUStatusRegister() : m_sleep(false) {}

bool PPUStatusRegister::get_sleep() const
{
	return m_sleep;
}

bool PPUStatusRegister::operator==(PPUStatusRegister const& other) const
{
	return m_sleep == other.m_sleep;
}

bool PPUStatusRegister::operator!=(PPUStatusRegister const& other) const
{
	return !(*this == other);
}

template <typename AddressT>
std::array<AddressT, PPUStatusRegister::config_size_in_words> PPUStatusRegister::addresses(
    coordinate_type const& coord) const
{
	uint32_t tmp;
	if (coord.toPPUOnDLS().value() == 0) {
		tmp = top_ppu_base_address;
	} else {
		tmp = bottom_ppu_base_address;
	}
	tmp |= ppu_status_register_address_mask;
	return {AddressT{tmp}};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    PPUStatusRegister::config_size_in_words>
PPUStatusRegister::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord) const;

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipAddress, PPUStatusRegister::config_size_in_words>
    PPUStatusRegister::addresses<halco::hicann_dls::vx::OmnibusChipAddress>(
        coordinate_type const& coord) const;

template <typename WordT>
std::array<WordT, PPUStatusRegister::config_size_in_words> PPUStatusRegister::encode() const
{
	return {{static_cast<WordT>(typename WordT::value_type(m_sleep))}};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, PPUStatusRegister::config_size_in_words>
    PPUStatusRegister::encode<fisch::vx::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE std::array<fisch::vx::OmnibusChip, PPUStatusRegister::config_size_in_words>
PPUStatusRegister::encode<fisch::vx::OmnibusChip>() const;

template <typename WordT>
void PPUStatusRegister::decode(
    std::array<WordT, PPUStatusRegister::config_size_in_words> const& data)
{
	m_sleep = static_cast<bool>(data[0].get());
}

template SYMBOL_VISIBLE void PPUStatusRegister::decode<fisch::vx::OmnibusChipOverJTAG>(
    std::array<fisch::vx::OmnibusChipOverJTAG, PPUStatusRegister::config_size_in_words> const&
        data);

template SYMBOL_VISIBLE void PPUStatusRegister::decode<fisch::vx::OmnibusChip>(
    std::array<fisch::vx::OmnibusChip, PPUStatusRegister::config_size_in_words> const& data);

template <class Archive>
void PPUStatusRegister::cerealize(Archive& ar)
{
	ar(CEREAL_NVP(m_sleep));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(PPUStatusRegister)

} // namespace vx
} // namespace haldls
