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
} // namespace vx
} // namespace haldls
