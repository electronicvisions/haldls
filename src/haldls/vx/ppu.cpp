#include <cctype>
#include <iomanip>
#include <utility>

#include "fisch/vx/jtag.h"

#include "haldls/vx/omnibus_addresses.h"
#include "haldls/vx/ppu.h"

namespace haldls {
namespace vx {

PPUMemoryWord::PPUMemoryWord() : PPUMemoryWord(PPUMemoryWord::Value(0)) {}

PPUMemoryWord::PPUMemoryWord(PPUMemoryWord::Value const& value) : m_value(value) {}

PPUMemoryWord::Value PPUMemoryWord::get() const
{
	return m_value;
}

void PPUMemoryWord::set(PPUMemoryWord::Value const& value)
{
	m_value = value;
}

bool PPUMemoryWord::operator==(PPUMemoryWord const& other) const
{
	return m_value == other.get();
}

bool PPUMemoryWord::operator!=(PPUMemoryWord const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, PPUMemoryWord const& pmw)
{
	uint32_t w = static_cast<uint32_t>(pmw.get());
	std::stringstream out;
	out << std::showbase << std::internal << std::setfill('0') << std::hex << std::setw(8) << w;
	os << out.str();
	return os;
}

template <>
std::array<omnibus_address_type, PPUMemoryWord::config_size_in_words>
PPUMemoryWord::addresses<omnibus_address_type>(coordinate_type const& coord) const
{
	uint32_t tmp = coord.toPPUMemoryWordOnPPU().value();
	if (coord.toPPUOnDLS().value() == 0) {
		tmp += top_ppu_base_address;
	} else {
		tmp += bottom_ppu_base_address;
	}
	return {{static_cast<omnibus_address_type>(tmp)}};
}

template SYMBOL_VISIBLE std::array<omnibus_address_type, PPUMemoryWord::config_size_in_words>
PPUMemoryWord::addresses<omnibus_address_type>(coordinate_type const& coord) const;

template <typename WordT>
std::array<WordT, PPUMemoryWord::config_size_in_words> PPUMemoryWord::encode() const
{
	return {{static_cast<WordT>(omnibus_word_type(get()))}};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusOnChipOverJTAG, PPUMemoryWord::config_size_in_words>
    PPUMemoryWord::encode<fisch::vx::OmnibusOnChipOverJTAG>() const;

template <typename WordT>
void PPUMemoryWord::decode(std::array<WordT, PPUMemoryWord::config_size_in_words> const& data)
{
	set(Value(data[0].get()));
}

template SYMBOL_VISIBLE void PPUMemoryWord::decode<fisch::vx::OmnibusOnChipOverJTAG>(
    std::array<fisch::vx::OmnibusOnChipOverJTAG, PPUMemoryWord::config_size_in_words> const& data);

template <class Archive>
void PPUMemoryWord::cerealize(Archive& ar)
{
	ar(CEREAL_NVP(m_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(PPUMemoryWord)
} // namespace vx
} // namespace haldls
