#include "haldls/vx/capmem.h"

#include "fisch/vx/word_access/type/jtag.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/expand_word.h"
#include "haldls/vx/container.tcc"
#include "haldls/vx/omnibus_constants.h"

namespace haldls {
namespace vx {

// Override provide working defaults for Issue 4047
CapMemSRAMTimingConfig::CapMemSRAMTimingConfig()
{
	this->set_address_setup_time(CapMemSRAMTimingConfig::AddressSetupTime(1));
	this->set_write_width(CapMemSRAMTimingConfig::WriteWidth(1));
	this->set_read_delay(CapMemSRAMTimingConfig::ReadDelay(8));
}

bool CapMemSRAMTimingConfig::operator==(CapMemSRAMTimingConfig const& other) const
{
	return static_cast<SRAMTimingConfig>(*this) == static_cast<SRAMTimingConfig>(other);
}

bool CapMemSRAMTimingConfig::operator!=(CapMemSRAMTimingConfig const& other) const
{
	return !(*this == other);
}

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, CapMemSRAMTimingConfig const& config)
{
	os << "CapMemSRAMTimingConfig(" << config.get_read_delay() << ", "
	   << config.get_address_setup_time() << ", " << config.get_write_width() << ")";
	return os;
}
#endif

template <typename AddressT>
std::array<AddressT, CapMemSRAMTimingConfig::config_size_in_words>
CapMemSRAMTimingConfig::addresses(coordinate_type const& coord)
{
	return {
	    AddressT(capmem_sram_timing_base_addresses.at(coord.toEnum())),
	    AddressT(capmem_sram_timing_base_addresses.at(coord.toEnum()) + 1)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CapMemSRAMTimingConfig::config_size_in_words>
CapMemSRAMTimingConfig::addresses(coordinate_type const& coord);
template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, CapMemSRAMTimingConfig::config_size_in_words>
    CapMemSRAMTimingConfig::addresses(coordinate_type const& coord);

constexpr uint32_t gray2bin(uint32_t const& value)
{
	uint32_t num = value;
	uint32_t mask = value;

	while (mask) {
		mask >>= 1;
		num ^= mask;
	}

	return num;
}

constexpr uint32_t bin2gray(uint32_t const& value)
{
	return value ^ (value >> 1);
}

// The values written to the config are gray coded by the SRAM controller. See Issue 4047.
// To ensure the correct values are written and read to the config the words must be (inverse) gray
// coded from (to) the omnibus
template <typename WordT>
std::array<WordT, CapMemSRAMTimingConfig::config_size_in_words> CapMemSRAMTimingConfig::encode()
    const
{
	std::array<WordT, CapMemSRAMTimingConfig::config_size_in_words> data =
	    SRAMTimingConfig::encode<WordT>();
	std::array<WordT, CapMemSRAMTimingConfig::config_size_in_words> gc_data;
	std::transform(data.begin(), data.end(), gc_data.begin(), [](WordT const& w) {
		return static_cast<WordT>(gray2bin(w));
	});

	return gc_data;
}

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    CapMemSRAMTimingConfig::config_size_in_words>
CapMemSRAMTimingConfig::encode() const;
template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, CapMemSRAMTimingConfig::config_size_in_words>
    CapMemSRAMTimingConfig::encode() const;

template <typename WordT>
void CapMemSRAMTimingConfig::decode(
    std::array<WordT, CapMemSRAMTimingConfig::config_size_in_words> const& data)
{
	std::array<WordT, CapMemSRAMTimingConfig::config_size_in_words> bin_data;
	std::transform(data.begin(), data.end(), bin_data.begin(), [](WordT const& w) {
		return static_cast<WordT>(bin2gray(w));
	});
	SRAMTimingConfig::decode<WordT>(bin_data);
}

template SYMBOL_VISIBLE void CapMemSRAMTimingConfig::decode(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        CapMemSRAMTimingConfig::config_size_in_words> const& data);
template SYMBOL_VISIBLE void CapMemSRAMTimingConfig::decode(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        CapMemSRAMTimingConfig::config_size_in_words> const& data);
} // namespace vx
} // namespace haldls

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::CapMemSRAMTimingConfig)