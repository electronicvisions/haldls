#include "haldls/vx/routing_crossbar.h"

#include "fisch/vx/word_access/type/jtag.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/common/iter_all.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/expand_word.h"
#include "haldls/vx/container.tcc"
#include "haldls/vx/omnibus_constants.h"
#include "hate/bitset.h"
#include "hate/join.h"

namespace haldls {
namespace vx {

CrossbarOutputConfig::CrossbarOutputConfig() : m_enable_event_counter(), m_enable_slow() {}

CrossbarOutputConfig::enable_event_counter_type const&
CrossbarOutputConfig::get_enable_event_counter() const
{
	return m_enable_event_counter;
}

void CrossbarOutputConfig::set_enable_event_counter(enable_event_counter_type const& value)
{
	m_enable_event_counter = value;
}

CrossbarOutputConfig::enable_slow_type const& CrossbarOutputConfig::get_enable_slow() const
{
	return m_enable_slow;
}

void CrossbarOutputConfig::set_enable_slow(enable_slow_type const& value)
{
	m_enable_slow = value;
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

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, CrossbarOutputConfig const& config)
{
	std::stringstream ss;
	ss << "CrossbarOutputConfig(enable_event_counter: [";
	ss << std::boolalpha;
	ss << hate::join(config.m_enable_event_counter, ", ");
	ss << "], enable_slow: [";
	ss << hate::join(config.m_enable_slow, ", ");
	ss << "])";
	return (os << ss.str());
}
#endif

template <typename AddressT>
std::array<AddressT, CrossbarOutputConfig::config_size_in_words> CrossbarOutputConfig::addresses(
    coordinate_type const& /* coord */)
{
	return {AddressT(crossbar_out_mux_base_address)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CrossbarOutputConfig::config_size_in_words>
CrossbarOutputConfig::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, CrossbarOutputConfig::config_size_in_words>
    CrossbarOutputConfig::addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& coord);

namespace {

struct CrossbarOutputConfigBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
#define WORD \
			(uint32_t enable_slow          :  4;) \
			(uint32_t enable_event_counter : 12;) \
			(uint32_t /* unused */         : 16;)
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	CrossbarOutputConfigBitfield() { u.raw = 0u; }

	CrossbarOutputConfigBitfield(uint32_t data) { u.raw = data; }
};

} // namespace

template <typename WordT>
std::array<WordT, CrossbarOutputConfig::config_size_in_words> CrossbarOutputConfig::encode() const
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

	return {WordT(bitfield.u.raw)};
}

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    CrossbarOutputConfig::config_size_in_words>
CrossbarOutputConfig::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, CrossbarOutputConfig::config_size_in_words>
    CrossbarOutputConfig::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void CrossbarOutputConfig::decode(
    std::array<WordT, CrossbarOutputConfig::config_size_in_words> const& data)
{
	CrossbarOutputConfigBitfield bitfield(data.at(0));

	hate::bitset<halco::hicann_dls::vx::CrossbarL2OutputOnDLS::size> enable_slow(
	    bitfield.u.m.enable_slow);
	for (auto l2out : halco::common::iter_all<halco::hicann_dls::vx::CrossbarL2OutputOnDLS>()) {
		m_enable_slow[l2out] = enable_slow.test(l2out.toEnum());
	}

	hate::bitset<halco::hicann_dls::vx::CrossbarOutputOnDLS::size> enable_event_counter(
	    bitfield.u.m.enable_event_counter);
	for (auto out : halco::common::iter_all<halco::hicann_dls::vx::CrossbarOutputOnDLS>()) {
		m_enable_event_counter[out] = enable_event_counter.test(out.toEnum());
	}
}

template SYMBOL_VISIBLE void
CrossbarOutputConfig::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        CrossbarOutputConfig::config_size_in_words> const& data);

template SYMBOL_VISIBLE void CrossbarOutputConfig::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        CrossbarOutputConfig::config_size_in_words> const& data);


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

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, CrossbarInputDropCounter const& config)
{
	return (os << "CrossbarInputDropCounter(" << config.m_value << ")");
}
#endif

template <typename AddressT>
std::array<AddressT, CrossbarInputDropCounter::read_config_size_in_words>
CrossbarInputDropCounter::read_addresses(coordinate_type const& coord)
{
	return {AddressT(crossbar_input_drop_counter_base_address + coord.toEnum())};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CrossbarInputDropCounter::read_config_size_in_words>
CrossbarInputDropCounter::read_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    CrossbarInputDropCounter::read_config_size_in_words>
CrossbarInputDropCounter::read_addresses<halco::hicann_dls::vx::OmnibusAddress>(
    coordinate_type const& coord);

template <typename AddressT>
std::array<AddressT, CrossbarInputDropCounter::write_config_size_in_words>
CrossbarInputDropCounter::write_addresses(coordinate_type const& /* coord */)
{
	return {};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CrossbarInputDropCounter::write_config_size_in_words>
CrossbarInputDropCounter::write_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    CrossbarInputDropCounter::write_config_size_in_words>
CrossbarInputDropCounter::write_addresses<halco::hicann_dls::vx::OmnibusAddress>(
    coordinate_type const& coord);

template <typename WordT>
std::array<WordT, CrossbarInputDropCounter::write_config_size_in_words>
CrossbarInputDropCounter::encode() const
{
	return {};
}

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    CrossbarInputDropCounter::write_config_size_in_words>
CrossbarInputDropCounter::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::Omnibus,
    CrossbarInputDropCounter::write_config_size_in_words>
CrossbarInputDropCounter::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void CrossbarInputDropCounter::decode(
    std::array<WordT, CrossbarInputDropCounter::read_config_size_in_words> const& data)
{
	m_value = Value(data.at(0) & Value::max);
}

template SYMBOL_VISIBLE void
CrossbarInputDropCounter::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<fisch::vx::word_access_type::OmnibusChipOverJTAG, read_config_size_in_words> const&
        data);

template SYMBOL_VISIBLE void CrossbarInputDropCounter::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<fisch::vx::word_access_type::Omnibus, read_config_size_in_words> const& data);


CrossbarOutputEventCounter::CrossbarOutputEventCounter() : m_value() {}

CrossbarOutputEventCounter::CrossbarOutputEventCounter(Value const value) : m_value(value) {}

CrossbarOutputEventCounter::Value CrossbarOutputEventCounter::get_value() const
{
	return m_value;
}

void CrossbarOutputEventCounter::set_value(Value const value)
{
	m_value = value;
}

bool CrossbarOutputEventCounter::operator==(CrossbarOutputEventCounter const& other) const
{
	return (m_value == other.m_value);
}

bool CrossbarOutputEventCounter::operator!=(CrossbarOutputEventCounter const& other) const
{
	return !(*this == other);
}

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, CrossbarOutputEventCounter const& config)
{
	return (os << "CrossbarOutputEventCounter(" << config.m_value << ")");
}
#endif

template <typename AddressT>
std::array<AddressT, CrossbarOutputEventCounter::read_config_size_in_words>
CrossbarOutputEventCounter::read_addresses(coordinate_type const& coord)
{
	return {AddressT(crossbar_output_event_counter_base_address + coord.toEnum())};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CrossbarOutputEventCounter::read_config_size_in_words>
CrossbarOutputEventCounter::read_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    CrossbarOutputEventCounter::read_config_size_in_words>
CrossbarOutputEventCounter::read_addresses<halco::hicann_dls::vx::OmnibusAddress>(
    coordinate_type const& coord);

template <typename AddressT>
std::array<AddressT, CrossbarOutputEventCounter::write_config_size_in_words>
CrossbarOutputEventCounter::write_addresses(coordinate_type const& /* coord */)
{
	return {};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CrossbarOutputEventCounter::write_config_size_in_words>
CrossbarOutputEventCounter::write_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    CrossbarOutputEventCounter::write_config_size_in_words>
CrossbarOutputEventCounter::write_addresses<halco::hicann_dls::vx::OmnibusAddress>(
    coordinate_type const& coord);

template <typename WordT>
std::array<WordT, CrossbarOutputEventCounter::write_config_size_in_words>
CrossbarOutputEventCounter::encode() const
{
	return {};
}

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    CrossbarOutputEventCounter::write_config_size_in_words>
CrossbarOutputEventCounter::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::Omnibus,
    CrossbarOutputEventCounter::write_config_size_in_words>
CrossbarOutputEventCounter::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void CrossbarOutputEventCounter::decode(
    std::array<WordT, CrossbarOutputEventCounter::read_config_size_in_words> const& data)
{
	m_value = Value(data.at(0) & Value::max);
}

template SYMBOL_VISIBLE void
CrossbarOutputEventCounter::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        CrossbarOutputEventCounter::read_config_size_in_words> const& data);

template SYMBOL_VISIBLE void
CrossbarOutputEventCounter::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        CrossbarOutputEventCounter::read_config_size_in_words> const& data);


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

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, CrossbarNode const& config)
{
	std::stringstream ss;
	ss << "CrossbarNode(mask: " << config.m_mask << ", target: " << config.m_target
	   << ", enable_drop_counter: " << std::boolalpha << config.m_enable_drop_counter << ")";
	return (os << ss.str());
}
#endif

CrossbarNode const CrossbarNode::drop_all = []() {
	CrossbarNode ret;
	ret.set_mask(neuron_label_type(0));
	ret.set_target(neuron_label_type(1));
	return ret;
}();

template <typename AddressT>
std::array<AddressT, CrossbarNode::config_size_in_words> CrossbarNode::addresses(
    coordinate_type const& coord)
{
	return {AddressT(crossbar_node_base_address + coord.toEnum())};
}

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, CrossbarNode::config_size_in_words>
    CrossbarNode::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
        coordinate_type const& coord);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, CrossbarNode::config_size_in_words>
    CrossbarNode::addresses<halco::hicann_dls::vx::OmnibusAddress>(coordinate_type const& coord);

namespace {

struct CrossbarNodeBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
#define WORD \
			(uint32_t mask                : 14;) \
			(uint32_t /* unused */        :  2;) \
			(uint32_t target              : 14;) \
			(uint32_t /* unused */        :  1;) \
			(uint32_t enable_drop_counter :  1;)
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	CrossbarNodeBitfield() { u.raw = 0u; }

	CrossbarNodeBitfield(uint32_t data) { u.raw = data; }
};

} // namespace

template <typename WordT>
std::array<WordT, CrossbarNode::config_size_in_words> CrossbarNode::encode() const
{
	CrossbarNodeBitfield bitfield;
	bitfield.u.m.mask = m_mask;
	bitfield.u.m.target = m_target;
	bitfield.u.m.enable_drop_counter = m_enable_drop_counter;

	return {WordT(bitfield.u.raw)};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::OmnibusChipOverJTAG, CrossbarNode::config_size_in_words>
    CrossbarNode::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, CrossbarNode::config_size_in_words>
    CrossbarNode::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void CrossbarNode::decode(std::array<WordT, CrossbarNode::config_size_in_words> const& data)
{
	CrossbarNodeBitfield bitfield(data.at(0));
	m_mask = neuron_label_type(bitfield.u.m.mask);
	m_target = neuron_label_type(bitfield.u.m.target);
	m_enable_drop_counter = bitfield.u.m.enable_drop_counter;
}

template SYMBOL_VISIBLE void CrossbarNode::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        CrossbarNode::config_size_in_words> const& data);

template SYMBOL_VISIBLE void CrossbarNode::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<fisch::vx::word_access_type::Omnibus, CrossbarNode::config_size_in_words> const&
        data);

} // namespace vx
} // namespace haldls

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::CrossbarOutputConfig)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::CrossbarInputDropCounter)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::CrossbarOutputEventCounter)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::CrossbarNode)
