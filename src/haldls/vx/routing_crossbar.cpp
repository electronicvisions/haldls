#include "haldls/vx/routing_crossbar.h"

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"

#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/cerealization.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/print.h"

#include "hate/bitset.h"

namespace haldls {
namespace vx {

CrossbarOutputConfig::CrossbarOutputConfig() : m_enable_event_counter(), m_enable_slow() {}

bool CrossbarOutputConfig::get_enable_event_counter(
    halco::hicann_dls::vx::CrossbarOutputOnDLS const& coord) const
{
	return m_enable_event_counter[coord];
}

void CrossbarOutputConfig::set_enable_event_counter(
    halco::hicann_dls::vx::CrossbarOutputOnDLS const& coord, bool const value)
{
	m_enable_event_counter[coord] = value;
}

bool CrossbarOutputConfig::get_enable_slow(
    halco::hicann_dls::vx::CrossbarL2OutputOnDLS const& coord) const
{
	return m_enable_slow[coord];
}

void CrossbarOutputConfig::set_enable_slow(
    halco::hicann_dls::vx::CrossbarL2OutputOnDLS const& coord, bool const value)
{
	m_enable_slow[coord] = value;
}

bool CrossbarOutputConfig::operator==(CrossbarOutputConfig const& other) const
{
	return (m_enable_event_counter == other.m_enable_event_counter) &&
	       (m_enable_slow == other.m_enable_slow);
}

bool CrossbarOutputConfig::operator!=(CrossbarOutputConfig const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, CrossbarOutputConfig const& config)
{
	return print_words_for_each_backend(os, config);
}

template <typename AddressT>
std::array<AddressT, CrossbarOutputConfig::read_config_size_in_words>
CrossbarOutputConfig::read_addresses(coordinate_type const& /* coord */) const
{
	return {};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CrossbarOutputConfig::read_config_size_in_words>
CrossbarOutputConfig::read_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord) const;

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipAddress,
    CrossbarOutputConfig::read_config_size_in_words>
CrossbarOutputConfig::read_addresses<halco::hicann_dls::vx::OmnibusChipAddress>(
    coordinate_type const& coord) const;

template <typename AddressT>
std::array<AddressT, CrossbarOutputConfig::write_config_size_in_words>
CrossbarOutputConfig::write_addresses(coordinate_type const& /* coord */) const
{
	return {AddressT(crossbar_out_mux_base_address)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CrossbarOutputConfig::write_config_size_in_words>
CrossbarOutputConfig::write_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord) const;

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipAddress,
    CrossbarOutputConfig::write_config_size_in_words>
CrossbarOutputConfig::write_addresses<halco::hicann_dls::vx::OmnibusChipAddress>(
    coordinate_type const& coord) const;

namespace {

struct CrossbarOutputConfigBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t enable_slow          :  4;
			uint32_t enable_event_counter : 12;
			uint32_t /* unused */         : 16;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	CrossbarOutputConfigBitfield() { u.raw = 0u; }

	CrossbarOutputConfigBitfield(uint32_t data) { u.raw = data; }
};

} // namespace

template <typename WordT>
std::array<WordT, CrossbarOutputConfig::write_config_size_in_words> CrossbarOutputConfig::encode()
    const
{
	CrossbarOutputConfigBitfield bitfield;

	hate::bitset<halco::hicann_dls::vx::CrossbarL2OutputOnDLS::size> enable_slow;
	for (auto l2out : halco::common::iter_all<halco::hicann_dls::vx::CrossbarL2OutputOnDLS>()) {
		enable_slow.set(l2out.toEnum(), m_enable_slow[l2out]);
	}
	bitfield.u.m.enable_slow = static_cast<uint32_t>(enable_slow);

	hate::bitset<halco::hicann_dls::vx::CrossbarOutputOnDLS::size> enable_event_counter;
	for (auto out : halco::common::iter_all<halco::hicann_dls::vx::CrossbarOutputOnDLS>()) {
		enable_event_counter.set(out.toEnum(), m_enable_event_counter[out]);
	}
	bitfield.u.m.enable_event_counter = static_cast<uint32_t>(enable_event_counter);

	return {WordT(fisch::vx::OmnibusData(bitfield.u.raw))};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, CrossbarOutputConfig::write_config_size_in_words>
    CrossbarOutputConfig::encode<fisch::vx::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChip, CrossbarOutputConfig::write_config_size_in_words>
    CrossbarOutputConfig::encode<fisch::vx::OmnibusChip>() const;

template <typename WordT>
void CrossbarOutputConfig::decode(
    std::array<WordT, CrossbarOutputConfig::read_config_size_in_words> const& /* data */)
{}

template SYMBOL_VISIBLE void CrossbarOutputConfig::decode<fisch::vx::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::OmnibusChipOverJTAG,
        CrossbarOutputConfig::read_config_size_in_words> const& data);

template SYMBOL_VISIBLE void CrossbarOutputConfig::decode<fisch::vx::OmnibusChip>(
    std::array<fisch::vx::OmnibusChip, CrossbarOutputConfig::read_config_size_in_words> const&
        data);

template <class Archive>
void CrossbarOutputConfig::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_enable_slow));
	ar(CEREAL_NVP(m_enable_event_counter));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CrossbarOutputConfig)

} // namespace vx
} // namespace haldls
