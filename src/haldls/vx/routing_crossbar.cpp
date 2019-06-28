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


CrossbarInputDropCounter::CrossbarInputDropCounter() : m_value() {}

CrossbarInputDropCounter::CrossbarInputDropCounter(Value const value) : m_value(value) {}

CrossbarInputDropCounter::Value CrossbarInputDropCounter::get_value() const
{
	return m_value;
}

void CrossbarInputDropCounter::set_value(Value const value)
{
	m_value = value;
}

bool CrossbarInputDropCounter::operator==(CrossbarInputDropCounter const& other) const
{
	return (m_value == other.m_value);
}

bool CrossbarInputDropCounter::operator!=(CrossbarInputDropCounter const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, CrossbarInputDropCounter const& config)
{
	return print_words_for_each_backend(os, config);
}

template <typename AddressT>
std::array<AddressT, CrossbarInputDropCounter::config_size_in_words>
CrossbarInputDropCounter::addresses(coordinate_type const& coord) const
{
	return {AddressT(crossbar_input_drop_counter_base_address + coord.toEnum())};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CrossbarInputDropCounter::config_size_in_words>
CrossbarInputDropCounter::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord) const;

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusChipAddress, CrossbarInputDropCounter::config_size_in_words>
    CrossbarInputDropCounter::addresses<halco::hicann_dls::vx::OmnibusChipAddress>(
        coordinate_type const& coord) const;

template <typename WordT>
std::array<WordT, CrossbarInputDropCounter::config_size_in_words> CrossbarInputDropCounter::encode()
    const
{
	return {WordT(fisch::vx::OmnibusData(m_value))};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, CrossbarInputDropCounter::config_size_in_words>
    CrossbarInputDropCounter::encode<fisch::vx::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChip, CrossbarInputDropCounter::config_size_in_words>
    CrossbarInputDropCounter::encode<fisch::vx::OmnibusChip>() const;

template <typename WordT>
void CrossbarInputDropCounter::decode(
    std::array<WordT, CrossbarInputDropCounter::config_size_in_words> const& data)
{
	m_value = Value(data.at(0).get());
}

template SYMBOL_VISIBLE void CrossbarInputDropCounter::decode<fisch::vx::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::OmnibusChipOverJTAG,
        CrossbarInputDropCounter::config_size_in_words> const& data);

template SYMBOL_VISIBLE void CrossbarInputDropCounter::decode<fisch::vx::OmnibusChip>(
    std::array<fisch::vx::OmnibusChip, CrossbarInputDropCounter::config_size_in_words> const& data);

template <class Archive>
void CrossbarInputDropCounter::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CrossbarInputDropCounter)


CrossbarNode::CrossbarNode() : m_mask(), m_target(), m_enable_drop_counter() {}

typename CrossbarNode::neuron_label_type CrossbarNode::get_mask() const
{
	return m_mask;
}

void CrossbarNode::set_mask(neuron_label_type const value)
{
	m_mask = value;
}

typename CrossbarNode::neuron_label_type CrossbarNode::get_target() const
{
	return m_target;
}

void CrossbarNode::set_target(neuron_label_type const value)
{
	m_target = value;
}

bool CrossbarNode::get_enable_drop_counter() const
{
	return m_enable_drop_counter;
}

void CrossbarNode::set_enable_drop_counter(bool const value)
{
	m_enable_drop_counter = value;
}

bool CrossbarNode::operator==(CrossbarNode const& other) const
{
	return (m_mask == other.m_mask) && (m_target == other.m_target) &&
	       (m_enable_drop_counter == other.m_enable_drop_counter);
}

bool CrossbarNode::operator!=(CrossbarNode const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, CrossbarNode const& config)
{
	return print_words_for_each_backend(os, config);
}

template <typename AddressT>
std::array<AddressT, CrossbarNode::read_config_size_in_words> CrossbarNode::read_addresses(
    coordinate_type const& /* coord */) const
{
	return {};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CrossbarNode::read_config_size_in_words>
CrossbarNode::read_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord) const;

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipAddress, CrossbarNode::read_config_size_in_words>
    CrossbarNode::read_addresses<halco::hicann_dls::vx::OmnibusChipAddress>(
        coordinate_type const& coord) const;

template <typename AddressT>
std::array<AddressT, CrossbarNode::write_config_size_in_words> CrossbarNode::write_addresses(
    coordinate_type const& coord) const
{
	return {AddressT(crossbar_node_base_address + coord.toEnum())};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CrossbarNode::write_config_size_in_words>
CrossbarNode::write_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord) const;

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipAddress, CrossbarNode::write_config_size_in_words>
    CrossbarNode::write_addresses<halco::hicann_dls::vx::OmnibusChipAddress>(
        coordinate_type const& coord) const;

namespace {

struct CrossbarNodeBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t mask                : 14;
			uint32_t /* unused */        :  2;
			uint32_t target              : 14;
			uint32_t /* unused */        :  1;
			uint32_t enable_drop_counter :  1;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	CrossbarNodeBitfield() { u.raw = 0u; }

	CrossbarNodeBitfield(uint32_t data) { u.raw = data; }
};

} // namespace

template <typename WordT>
std::array<WordT, CrossbarNode::write_config_size_in_words> CrossbarNode::encode() const
{
	CrossbarNodeBitfield bitfield;
	bitfield.u.m.mask = m_mask;
	bitfield.u.m.target = m_target;
	bitfield.u.m.enable_drop_counter = m_enable_drop_counter;

	return {WordT(fisch::vx::OmnibusData(bitfield.u.raw))};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, CrossbarNode::write_config_size_in_words>
    CrossbarNode::encode<fisch::vx::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE std::array<fisch::vx::OmnibusChip, CrossbarNode::write_config_size_in_words>
CrossbarNode::encode<fisch::vx::OmnibusChip>() const;

template <typename WordT>
void CrossbarNode::decode(
    std::array<WordT, CrossbarNode::read_config_size_in_words> const& /* data */)
{}

template SYMBOL_VISIBLE void CrossbarNode::decode<fisch::vx::OmnibusChipOverJTAG>(
    std::array<fisch::vx::OmnibusChipOverJTAG, CrossbarNode::read_config_size_in_words> const&
        data);

template SYMBOL_VISIBLE void CrossbarNode::decode<fisch::vx::OmnibusChip>(
    std::array<fisch::vx::OmnibusChip, CrossbarNode::read_config_size_in_words> const& data);

template <class Archive>
void CrossbarNode::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_mask));
	ar(CEREAL_NVP(m_target));
	ar(CEREAL_NVP(m_enable_drop_counter));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CrossbarNode)

} // namespace vx
} // namespace haldls
