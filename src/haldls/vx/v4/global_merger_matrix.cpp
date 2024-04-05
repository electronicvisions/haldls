#include "haldls/vx/v4/global_merger_matrix.h"

#include "fisch/vx/word_access/type/jtag.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/common/iter_all.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/expand_word.h"
#include "haldls/vx/container.tcc"
#include "haldls/vx/v4/omnibus_constants.h"
#include "hate/bitset.h"
#include "hate/join.h"
#include "hate/math.h"

namespace haldls::vx::v4 {

GlobalMergerMatrixOutputConfig::GlobalMergerMatrixOutputConfig() :
    m_enable_event_counter(), m_enable_slow()
{}

GlobalMergerMatrixOutputConfig::enable_event_counter_type const&
GlobalMergerMatrixOutputConfig::get_enable_event_counter() const
{
	return m_enable_event_counter;
}

void GlobalMergerMatrixOutputConfig::set_enable_event_counter(
    enable_event_counter_type const& value)
{
	m_enable_event_counter = value;
}

GlobalMergerMatrixOutputConfig::enable_slow_type const&
GlobalMergerMatrixOutputConfig::get_enable_slow() const
{
	return m_enable_slow;
}

void GlobalMergerMatrixOutputConfig::set_enable_slow(enable_slow_type const& value)
{
	m_enable_slow = value;
}

bool GlobalMergerMatrixOutputConfig::operator==(GlobalMergerMatrixOutputConfig const& other) const
{
	return (m_enable_event_counter == other.m_enable_event_counter) &&
	       (m_enable_slow == other.m_enable_slow);
}

bool GlobalMergerMatrixOutputConfig::operator!=(GlobalMergerMatrixOutputConfig const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, GlobalMergerMatrixOutputConfig const& config)
{
	std::stringstream ss;
	ss << "GlobalMergerMatrixOutputConfig(enable_event_counter: [";
	ss << std::boolalpha;
	ss << hate::join(config.m_enable_event_counter, ", ");
	ss << "], enable_slow: [";
	ss << hate::join(config.m_enable_slow, ", ");
	ss << "])";
	return (os << ss.str());
}

template <typename AddressT>
std::array<AddressT, GlobalMergerMatrixOutputConfig::config_size_in_words>
GlobalMergerMatrixOutputConfig::addresses(coordinate_type const& /* coord */)
{
	return {AddressT(global_merger_matrix_out_mux_base_address)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    GlobalMergerMatrixOutputConfig::config_size_in_words>
GlobalMergerMatrixOutputConfig::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    GlobalMergerMatrixOutputConfig::config_size_in_words>
GlobalMergerMatrixOutputConfig::addresses<halco::hicann_dls::vx::OmnibusAddress>(
    coordinate_type const& coord);

namespace {

struct GlobalMergerMatrixOutputConfigBitfield
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

	GlobalMergerMatrixOutputConfigBitfield()
	{
		u.raw = 0u;
	}

	GlobalMergerMatrixOutputConfigBitfield(uint32_t data)
	{
		u.raw = data;
	}
};

} // namespace

template <typename WordT>
std::array<WordT, GlobalMergerMatrixOutputConfig::config_size_in_words>
GlobalMergerMatrixOutputConfig::encode() const
{
	GlobalMergerMatrixOutputConfigBitfield bitfield;

	hate::bitset<halco::hicann_dls::vx::GlobalMergerMatrixOutputChannelOnDLS::size> enable_slow;
	for (auto out :
	     halco::common::iter_all<halco::hicann_dls::vx::GlobalMergerMatrixOutputChannelOnDLS>()) {
		enable_slow.set(out.toEnum(), m_enable_slow[out]);
	}
	bitfield.u.m.enable_slow = static_cast<uint32_t>(enable_slow);

	hate::bitset<halco::hicann_dls::vx::GlobalMergerMatrixOutputChannelOnDLS::size>
	    enable_event_counter;
	for (auto out :
	     halco::common::iter_all<halco::hicann_dls::vx::GlobalMergerMatrixOutputChannelOnDLS>()) {
		enable_event_counter.set(out.toEnum(), m_enable_event_counter[out]);
	}
	bitfield.u.m.enable_event_counter = static_cast<uint32_t>(enable_event_counter);

	return {WordT(bitfield.u.raw)};
}

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    GlobalMergerMatrixOutputConfig::config_size_in_words>
GlobalMergerMatrixOutputConfig::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::Omnibus,
    GlobalMergerMatrixOutputConfig::config_size_in_words>
GlobalMergerMatrixOutputConfig::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void GlobalMergerMatrixOutputConfig::decode(
    std::array<WordT, GlobalMergerMatrixOutputConfig::config_size_in_words> const& data)
{
	GlobalMergerMatrixOutputConfigBitfield bitfield(data.at(0));

	hate::bitset<halco::hicann_dls::vx::GlobalMergerMatrixOutputChannelOnDLS::size> enable_slow(
	    bitfield.u.m.enable_slow);
	for (auto out :
	     halco::common::iter_all<halco::hicann_dls::vx::GlobalMergerMatrixOutputChannelOnDLS>()) {
		m_enable_slow[out] = enable_slow.test(out.toEnum());
	}

	hate::bitset<halco::hicann_dls::vx::GlobalMergerMatrixOutputChannelOnDLS::size>
	    enable_event_counter(bitfield.u.m.enable_event_counter);
	for (auto out :
	     halco::common::iter_all<halco::hicann_dls::vx::GlobalMergerMatrixOutputChannelOnDLS>()) {
		m_enable_event_counter[out] = enable_event_counter.test(out.toEnum());
	}
}

template SYMBOL_VISIBLE void
GlobalMergerMatrixOutputConfig::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        GlobalMergerMatrixOutputConfig::config_size_in_words> const& data);

template SYMBOL_VISIBLE void
GlobalMergerMatrixOutputConfig::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        GlobalMergerMatrixOutputConfig::config_size_in_words> const& data);


NeuronLabelToGL1EventLUTEntry::NeuronLabelToGL1EventLUTEntry() :
    m_global_merger_matrix_mutable(), m_gl1_index()
{}

halco::hicann_dls::vx::SpikeLabel::GlobalMergerMatrixMutable
NeuronLabelToGL1EventLUTEntry::get_global_merger_matrix_mutable() const
{
	return m_global_merger_matrix_mutable;
}

void NeuronLabelToGL1EventLUTEntry::set_global_merger_matrix_mutable(
    halco::hicann_dls::vx::SpikeLabel::GlobalMergerMatrixMutable value)
{
	m_global_merger_matrix_mutable = value;
}

GlobalMergerMatrixNode::GL1Index NeuronLabelToGL1EventLUTEntry::get_gl1_index() const
{
	return m_gl1_index;
}

void NeuronLabelToGL1EventLUTEntry::set_gl1_index(GlobalMergerMatrixNode::GL1Index value)
{
	m_gl1_index = value;
}

bool NeuronLabelToGL1EventLUTEntry::operator==(NeuronLabelToGL1EventLUTEntry const& other) const
{
	return (m_global_merger_matrix_mutable == other.m_global_merger_matrix_mutable) &&
	       (m_gl1_index == other.m_gl1_index);
}

bool NeuronLabelToGL1EventLUTEntry::operator!=(NeuronLabelToGL1EventLUTEntry const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, NeuronLabelToGL1EventLUTEntry const& config)
{
	std::stringstream ss;
	ss << "NeuronLabelToGL1EventLUTEntry(global_merger_matrix_mutable: ";
	ss << config.m_global_merger_matrix_mutable;
	ss << ", gl1_index: ";
	ss << config.m_gl1_index;
	ss << ")";
	return (os << ss.str());
}

template <typename AddressT>
std::array<AddressT, NeuronLabelToGL1EventLUTEntry::config_size_in_words>
NeuronLabelToGL1EventLUTEntry::addresses(coordinate_type const& coord)
{
	return {AddressT(neuron_label_to_gl1_index_lut_entry_base_address + coord.toEnum().value())};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    NeuronLabelToGL1EventLUTEntry::config_size_in_words>
NeuronLabelToGL1EventLUTEntry::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    NeuronLabelToGL1EventLUTEntry::config_size_in_words>
NeuronLabelToGL1EventLUTEntry::addresses<halco::hicann_dls::vx::OmnibusAddress>(
    coordinate_type const& coord);

namespace {

struct NeuronLabelToGL1EventLUTEntryBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
#define WORD \
			(uint32_t gl1_index                    : GlobalMergerMatrixNode::IDX_WIDTH;) \
			(uint32_t /* unused */                 : 16 - GlobalMergerMatrixNode::IDX_WIDTH;) \
			(uint32_t global_merger_matrix_mutable : halco::hicann_dls::vx::GMM_IDXBITS;) \
			(uint32_t /* unused */                 : 16 - halco::hicann_dls::vx::GMM_IDXBITS;)
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	NeuronLabelToGL1EventLUTEntryBitfield()
	{
		u.raw = 0u;
	}

	NeuronLabelToGL1EventLUTEntryBitfield(uint32_t data)
	{
		u.raw = data;
	}
};

} // namespace

template <typename WordT>
std::array<WordT, NeuronLabelToGL1EventLUTEntry::config_size_in_words>
NeuronLabelToGL1EventLUTEntry::encode() const
{
	NeuronLabelToGL1EventLUTEntryBitfield bitfield;

	bitfield.u.m.gl1_index = static_cast<uint32_t>(m_gl1_index);
	bitfield.u.m.global_merger_matrix_mutable =
	    static_cast<uint32_t>(m_global_merger_matrix_mutable);

	return {WordT(bitfield.u.raw)};
}

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    NeuronLabelToGL1EventLUTEntry::config_size_in_words>
NeuronLabelToGL1EventLUTEntry::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE std::
    array<fisch::vx::word_access_type::Omnibus, NeuronLabelToGL1EventLUTEntry::config_size_in_words>
    NeuronLabelToGL1EventLUTEntry::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void NeuronLabelToGL1EventLUTEntry::decode(
    std::array<WordT, NeuronLabelToGL1EventLUTEntry::config_size_in_words> const& data)
{
	NeuronLabelToGL1EventLUTEntryBitfield bitfield(data.at(0));

	m_gl1_index = GlobalMergerMatrixNode::GL1Index(bitfield.u.m.gl1_index);
	m_global_merger_matrix_mutable = halco::hicann_dls::vx::SpikeLabel::GlobalMergerMatrixMutable(
	    bitfield.u.m.global_merger_matrix_mutable);
}

template SYMBOL_VISIBLE void
NeuronLabelToGL1EventLUTEntry::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        NeuronLabelToGL1EventLUTEntry::config_size_in_words> const& data);

template SYMBOL_VISIBLE void
NeuronLabelToGL1EventLUTEntry::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        NeuronLabelToGL1EventLUTEntry::config_size_in_words> const& data);


GlobalMergerMatrixInputDropCounter::GlobalMergerMatrixInputDropCounter() : m_value() {}

GlobalMergerMatrixInputDropCounter::GlobalMergerMatrixInputDropCounter(Value const value) :
    m_value(value)
{}

GlobalMergerMatrixInputDropCounter::Value GlobalMergerMatrixInputDropCounter::get_value() const
{
	return m_value;
}

void GlobalMergerMatrixInputDropCounter::set_value(Value const value)
{
	m_value = value;
}

bool GlobalMergerMatrixInputDropCounter::operator==(
    GlobalMergerMatrixInputDropCounter const& other) const
{
	return (m_value == other.m_value);
}

bool GlobalMergerMatrixInputDropCounter::operator!=(
    GlobalMergerMatrixInputDropCounter const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, GlobalMergerMatrixInputDropCounter const& config)
{
	return (os << "GlobalMergerMatrixInputDropCounter(" << config.m_value << ")");
}

template <typename AddressT>
std::array<AddressT, GlobalMergerMatrixInputDropCounter::read_config_size_in_words>
GlobalMergerMatrixInputDropCounter::read_addresses(coordinate_type const& coord)
{
	return {AddressT(global_merger_matrix_input_drop_counter_base_address + coord.toEnum())};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    GlobalMergerMatrixInputDropCounter::read_config_size_in_words>
GlobalMergerMatrixInputDropCounter::read_addresses<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(coordinate_type const& coord);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    GlobalMergerMatrixInputDropCounter::read_config_size_in_words>
GlobalMergerMatrixInputDropCounter::read_addresses<halco::hicann_dls::vx::OmnibusAddress>(
    coordinate_type const& coord);

template <typename AddressT>
std::array<AddressT, GlobalMergerMatrixInputDropCounter::write_config_size_in_words>
GlobalMergerMatrixInputDropCounter::write_addresses(coordinate_type const& /* coord */)
{
	return {};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    GlobalMergerMatrixInputDropCounter::write_config_size_in_words>
GlobalMergerMatrixInputDropCounter::write_addresses<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(coordinate_type const& coord);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    GlobalMergerMatrixInputDropCounter::write_config_size_in_words>
GlobalMergerMatrixInputDropCounter::write_addresses<halco::hicann_dls::vx::OmnibusAddress>(
    coordinate_type const& coord);

template <typename WordT>
std::array<WordT, GlobalMergerMatrixInputDropCounter::write_config_size_in_words>
GlobalMergerMatrixInputDropCounter::encode() const
{
	return {};
}

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    GlobalMergerMatrixInputDropCounter::write_config_size_in_words>
GlobalMergerMatrixInputDropCounter::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>()
    const;

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::Omnibus,
    GlobalMergerMatrixInputDropCounter::write_config_size_in_words>
GlobalMergerMatrixInputDropCounter::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void GlobalMergerMatrixInputDropCounter::decode(
    std::array<WordT, GlobalMergerMatrixInputDropCounter::read_config_size_in_words> const& data)
{
	m_value = Value(data.at(0) & Value::max);
}

template SYMBOL_VISIBLE void
GlobalMergerMatrixInputDropCounter::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<fisch::vx::word_access_type::OmnibusChipOverJTAG, read_config_size_in_words> const&
        data);

template SYMBOL_VISIBLE void
GlobalMergerMatrixInputDropCounter::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<fisch::vx::word_access_type::Omnibus, read_config_size_in_words> const& data);


GlobalMergerMatrixOutputEventCounter::GlobalMergerMatrixOutputEventCounter() : m_value() {}

GlobalMergerMatrixOutputEventCounter::GlobalMergerMatrixOutputEventCounter(Value const value) :
    m_value(value)
{}

GlobalMergerMatrixOutputEventCounter::Value GlobalMergerMatrixOutputEventCounter::get_value() const
{
	return m_value;
}

void GlobalMergerMatrixOutputEventCounter::set_value(Value const value)
{
	m_value = value;
}

bool GlobalMergerMatrixOutputEventCounter::operator==(
    GlobalMergerMatrixOutputEventCounter const& other) const
{
	return (m_value == other.m_value);
}

bool GlobalMergerMatrixOutputEventCounter::operator!=(
    GlobalMergerMatrixOutputEventCounter const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, GlobalMergerMatrixOutputEventCounter const& config)
{
	return (os << "GlobalMergerMatrixOutputEventCounter(" << config.m_value << ")");
}

template <typename AddressT>
std::array<AddressT, GlobalMergerMatrixOutputEventCounter::read_config_size_in_words>
GlobalMergerMatrixOutputEventCounter::read_addresses(coordinate_type const& coord)
{
	return {AddressT(global_merger_matrix_output_event_counter_base_address + coord.toEnum())};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    GlobalMergerMatrixOutputEventCounter::read_config_size_in_words>
GlobalMergerMatrixOutputEventCounter::read_addresses<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(coordinate_type const& coord);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    GlobalMergerMatrixOutputEventCounter::read_config_size_in_words>
GlobalMergerMatrixOutputEventCounter::read_addresses<halco::hicann_dls::vx::OmnibusAddress>(
    coordinate_type const& coord);

template <typename AddressT>
std::array<AddressT, GlobalMergerMatrixOutputEventCounter::write_config_size_in_words>
GlobalMergerMatrixOutputEventCounter::write_addresses(coordinate_type const& /* coord */)
{
	return {};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    GlobalMergerMatrixOutputEventCounter::write_config_size_in_words>
GlobalMergerMatrixOutputEventCounter::write_addresses<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(coordinate_type const& coord);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    GlobalMergerMatrixOutputEventCounter::write_config_size_in_words>
GlobalMergerMatrixOutputEventCounter::write_addresses<halco::hicann_dls::vx::OmnibusAddress>(
    coordinate_type const& coord);

template <typename WordT>
std::array<WordT, GlobalMergerMatrixOutputEventCounter::write_config_size_in_words>
GlobalMergerMatrixOutputEventCounter::encode() const
{
	return {};
}

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    GlobalMergerMatrixOutputEventCounter::write_config_size_in_words>
GlobalMergerMatrixOutputEventCounter::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>()
    const;

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::Omnibus,
    GlobalMergerMatrixOutputEventCounter::write_config_size_in_words>
GlobalMergerMatrixOutputEventCounter::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void GlobalMergerMatrixOutputEventCounter::decode(
    std::array<WordT, GlobalMergerMatrixOutputEventCounter::read_config_size_in_words> const& data)
{
	m_value = Value(data.at(0) & Value::max);
}

template SYMBOL_VISIBLE void
GlobalMergerMatrixOutputEventCounter::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        GlobalMergerMatrixOutputEventCounter::read_config_size_in_words> const& data);

template SYMBOL_VISIBLE void
GlobalMergerMatrixOutputEventCounter::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        GlobalMergerMatrixOutputEventCounter::read_config_size_in_words> const& data);


GlobalMergerMatrixNode::GlobalMergerMatrixNode() :
    m_mask(), m_target(), m_enable_drop_counter(), m_accept_gl1_index()
{
	m_accept_gl1_index.fill(true);
}

typename GlobalMergerMatrixNode::neuron_label_type GlobalMergerMatrixNode::get_mask() const
{
	return m_mask;
}

void GlobalMergerMatrixNode::set_mask(neuron_label_type const value)
{
	m_mask = value;
}

typename GlobalMergerMatrixNode::neuron_label_type GlobalMergerMatrixNode::get_target() const
{
	return m_target;
}

void GlobalMergerMatrixNode::set_target(neuron_label_type const value)
{
	m_target = value;
}

bool GlobalMergerMatrixNode::get_enable_drop_counter() const
{
	return m_enable_drop_counter;
}

void GlobalMergerMatrixNode::set_enable_drop_counter(bool const value)
{
	m_enable_drop_counter = value;
}

GlobalMergerMatrixNode::accept_gl1_index_type const& GlobalMergerMatrixNode::get_accept_gl1_index()
    const
{
	return m_accept_gl1_index;
}

void GlobalMergerMatrixNode::set_accept_gl1_index(
    GlobalMergerMatrixNode::accept_gl1_index_type const& value)
{
	m_accept_gl1_index = value;
}

bool GlobalMergerMatrixNode::operator==(GlobalMergerMatrixNode const& other) const
{
	return (m_mask == other.m_mask) && (m_target == other.m_target) &&
	       (m_enable_drop_counter == other.m_enable_drop_counter) &&
	       (m_accept_gl1_index == other.m_accept_gl1_index);
}

bool GlobalMergerMatrixNode::operator!=(GlobalMergerMatrixNode const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, GlobalMergerMatrixNode const& config)
{
	std::stringstream ss;
	ss << "GlobalMergerMatrixNode(mask: " << config.m_mask << ", target: " << config.m_target
	   << ", enable_drop_counter: " << std::boolalpha << config.m_enable_drop_counter
	   << ", accept_gl1_index: [" << hate::join(config.m_accept_gl1_index, ", ") << "])";
	return (os << ss.str());
}

GlobalMergerMatrixNode const GlobalMergerMatrixNode::drop_all = []() {
	GlobalMergerMatrixNode ret;
	ret.set_mask(neuron_label_type(0));
	ret.set_target(neuron_label_type(1));
	return ret;
}();

template <typename AddressT>
std::array<AddressT, GlobalMergerMatrixNode::config_size_in_words>
GlobalMergerMatrixNode::addresses(coordinate_type const& coord)
{
	return {
	    AddressT(global_merger_matrix_node_base_address + 2 * coord.toEnum().value()),
	    AddressT(global_merger_matrix_node_base_address + 2 * coord.toEnum().value() + 1)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    GlobalMergerMatrixNode::config_size_in_words>
GlobalMergerMatrixNode::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, GlobalMergerMatrixNode::config_size_in_words>
    GlobalMergerMatrixNode::addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& coord);

namespace {

struct GlobalMergerMatrixNodeBitfield
{
	union
	{
		std::array<uint32_t, GlobalMergerMatrixNode::config_size_in_words> raw;
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
#define WORD \
			(uint32_t accept_gl1_index    : GlobalMergerMatrixNode::IDX_WIDTH;) \
			(uint32_t /* unused */        : 32 - GlobalMergerMatrixNode::IDX_WIDTH;)
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	GlobalMergerMatrixNodeBitfield()
	{
		u.raw.fill(0u);
	}

	GlobalMergerMatrixNodeBitfield(
	    std::array<uint32_t, GlobalMergerMatrixNode::config_size_in_words> data)
	{
		u.raw = data;
	}
};

} // namespace

template <typename WordT>
std::array<WordT, GlobalMergerMatrixNode::config_size_in_words> GlobalMergerMatrixNode::encode()
    const
{
	GlobalMergerMatrixNodeBitfield bitfield;
	bitfield.u.m.mask = m_mask;
	bitfield.u.m.target = m_target;
	bitfield.u.m.enable_drop_counter = m_enable_drop_counter;

	hate::bitset<GL1Index::size> accept_gl1_index;
	for (auto const gl1_index : halco::common::iter_all<GL1Index>()) {
		accept_gl1_index.set(gl1_index.value(), m_accept_gl1_index[gl1_index]);
	}
	bitfield.u.m.accept_gl1_index = static_cast<uint32_t>(accept_gl1_index);

	return {WordT(bitfield.u.raw[0]), WordT(bitfield.u.raw[1])};
}

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    GlobalMergerMatrixNode::config_size_in_words>
GlobalMergerMatrixNode::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, GlobalMergerMatrixNode::config_size_in_words>
    GlobalMergerMatrixNode::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void GlobalMergerMatrixNode::decode(
    std::array<WordT, GlobalMergerMatrixNode::config_size_in_words> const& data)
{
	GlobalMergerMatrixNodeBitfield bitfield(std::array<uint32_t, 2>{data.at(0), data.at(1)});
	m_mask = neuron_label_type(bitfield.u.m.mask);
	m_target = neuron_label_type(bitfield.u.m.target);
	m_enable_drop_counter = bitfield.u.m.enable_drop_counter;

	hate::bitset<GL1Index::size> accept_gl1_index(bitfield.u.m.accept_gl1_index);
	for (auto const gl1_index : halco::common::iter_all<GL1Index>()) {
		m_accept_gl1_index[gl1_index] = accept_gl1_index.test(gl1_index.value());
	}
}

template SYMBOL_VISIBLE void
GlobalMergerMatrixNode::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        GlobalMergerMatrixNode::config_size_in_words> const& data);

template SYMBOL_VISIBLE void GlobalMergerMatrixNode::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        GlobalMergerMatrixNode::config_size_in_words> const& data);

} // namespace haldls::vx::v4

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::v4::GlobalMergerMatrixOutputConfig)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::v4::NeuronLabelToGL1EventLUTEntry)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::v4::GlobalMergerMatrixInputDropCounter)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::v4::GlobalMergerMatrixOutputEventCounter)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::v4::GlobalMergerMatrixNode)
