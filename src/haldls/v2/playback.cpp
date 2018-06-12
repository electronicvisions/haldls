#include "haldls/v2/playback.h"

#include <sstream>

#include "uni/decoder.h"
#include "uni/program_builder.h"

#include "haldls/v2/capmem.h"
#include "haldls/v2/chip.h"
#include "haldls/v2/common.h"
#include "haldls/v2/correlation.h"
#include "haldls/v2/dac.h"
#include "haldls/v2/neuron.h"
#include "haldls/v2/ppu.h"
#include "haldls/v2/rate_counter.h"
#include "haldls/v2/synapsedriver.h"
#include "stadls/visitors.h"

namespace haldls {
namespace v2 {

struct PlaybackProgram::Impl
{
	typedef v2::hardware_word_type hardware_word_type;
	typedef v2::hardware_address_type hardware_address_type;

	Impl() : bld(alloc) {}

	uni::Byte_vector_allocator alloc;
	uni::Program_builder<uni::Byte_vector_allocator> bld;

	/// \brief Offset for the result of the next read instruction, i.e. number of already
	///        executed read instructions.
	size_t read_offset = 0;

	std::vector<hardware_word_type> results;
	PlaybackProgram::spikes_type spikes;
};

constexpr PlaybackProgram::serial_number_type PlaybackProgram::invalid_serial_number;

PlaybackProgram::PlaybackProgram()
	: m_impl(new Impl()), m_serial_number(invalid_serial_number)
{
}

PlaybackProgram::PlaybackProgram(serial_number_type serial_number)
	: m_impl(new Impl()), m_serial_number(serial_number)
{
	if (serial_number == invalid_serial_number)
		throw std::logic_error("invalid serial number");
}

PlaybackProgram::PlaybackProgram(PlaybackProgram const& other)
	: m_impl(new Impl(*other.m_impl)), m_serial_number(other.m_serial_number)
{}

PlaybackProgram& PlaybackProgram::operator=(PlaybackProgram const& other)
{
	if (this != &other) {
		m_impl.reset(new Impl(*other.m_impl));
		m_serial_number = other.m_serial_number;
	}
	return *this;
}

PlaybackProgram::PlaybackProgram(PlaybackProgram&& other) noexcept
	: m_impl(std::move(other.m_impl)), m_serial_number(other.m_serial_number) {
	other.m_serial_number = invalid_serial_number;
}

PlaybackProgram& PlaybackProgram::operator=(PlaybackProgram&& other) noexcept {
	if (this != &other) {
		m_impl = std::move(other.m_impl);
		m_serial_number = other.m_serial_number;
		other.m_serial_number = invalid_serial_number;
	}
	return *this;
}

PlaybackProgram::~PlaybackProgram() = default;

template <typename T>
void PlaybackProgram::ensure_container_invariants(T& /*config*/)
{}

template <>
void PlaybackProgram::ensure_container_invariants<v2::NeuronDigitalConfig>(
	v2::NeuronDigitalConfig& config)
{
	// Only instances contained in v2::Chip are allowed to have these bits enabled.
	config.set_enable_buffered_readout(false, {});
}

template <typename T>
T PlaybackProgram::get(ContainerTicket<T> const& ticket) const
{
	if (!m_impl)
		throw std::logic_error("unexpected access to moved-from object");

	if (ticket.serial_number != m_serial_number)
		throw std::invalid_argument("container ticket does not belong to this playback program");

	auto const& results = m_impl->results;

	if (ticket.offset >= results.size() || ticket.offset + ticket.length > results.size())
		throw std::runtime_error(
			"container data not available yet (out of bounds of available results data)");

	typedef std::vector<v2::hardware_word_type> words_type;
	words_type data{std::next(results.begin(), ticket.offset),
					std::next(results.begin(), ticket.offset + ticket.length)};

	T config;
	visit_preorder(config, ticket.coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ensure_container_invariants(config);
	return config;
}

template <>
PPUMemoryBlock PlaybackProgram::get(ContainerTicket<PPUMemoryBlock> const& ticket) const
{
	if (!m_impl)
		throw std::logic_error("unexpected access to moved-from object");

	if (ticket.serial_number != m_serial_number)
		throw std::invalid_argument("container ticket does not belong to this playback program");

	auto const& results = m_impl->results;

	if (ticket.offset >= results.size() || ticket.offset + ticket.length > results.size())
		throw std::runtime_error(
		    "container data not available yet (out of bounds of available results data)");

	typedef std::vector<v2::hardware_word_type> words_type;
	words_type data{std::next(results.begin(), ticket.offset),
	                std::next(results.begin(), ticket.offset + ticket.length)};

	PPUMemoryBlock config(PPUMemoryBlock::Size(ticket.coord.second - ticket.coord.first + 1));
	visit_preorder(config, ticket.coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ensure_container_invariants(config);
	return config;
}

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	template SYMBOL_VISIBLE Type PlaybackProgram::get<Type>(ContainerTicket<Type> const&) const;
#include "haldls/v2/container.def"

PlaybackProgram::serial_number_type PlaybackProgram::serial_number() const
{
	return m_serial_number;
}

std::string PlaybackProgram::dump_program() const
{
	std::ostringstream os;
	uni::Stream_decoder decoder(os);
	for (auto const& container : instruction_byte_blocks())
		uni::decode(std::begin(container), std::end(container), decoder);
	return os.str();
}

template <typename T>
PlaybackProgram::ContainerTicket<T> PlaybackProgram::create_ticket(
	typename T::coordinate_type const& coord, std::size_t offset, std::size_t length) const
{
	if (!m_impl)
		throw std::logic_error("unexpected access to moved-from object");

	assert(m_serial_number != invalid_serial_number);
	return {m_serial_number, coord, offset, length};
}

std::vector<std::vector<instruction_word_type> > const& PlaybackProgram::instruction_byte_blocks()
	const
{
	if (!m_impl)
		throw std::logic_error("unexpected access to moved-from object");

	return m_impl->bld.containers;
}

void PlaybackProgram::set_results(std::vector<v2::hardware_word_type>&& results)
{
	if (!m_impl)
		throw std::logic_error("unexpected access to moved-from object");

	m_impl->results = std::move(results);
}

PlaybackProgram::spikes_type const& PlaybackProgram::get_spikes() const
{
	if (!m_impl)
		throw std::logic_error("unexpected access to moved-from object");

	return m_impl->spikes;
}

void PlaybackProgram::set_spikes(spikes_type&& spikes)
{
	if (!m_impl)
		throw std::logic_error("unexpected access to moved-from object");

	m_impl->spikes = std::move(spikes);
}

void PlaybackProgramBuilder::set_time(time_type t)
{
	assert(m_program.m_impl != nullptr);
	m_program.m_impl->bld.set_time(t);
}

void PlaybackProgramBuilder::wait_until(time_type t)
{
	assert(m_program.m_impl != nullptr);
	m_program.m_impl->bld.wait_until(t);
}

void PlaybackProgramBuilder::wait_for(time_type t)
{
	assert(m_program.m_impl != nullptr);
	m_program.m_impl->bld.wait_for(t);
}

void PlaybackProgramBuilder::fire(
	std::bitset<halco::hicann_dls::v2::SynapseDriverOnDLS::size> const& synapse_driver_mask,
	v2::SynapseBlock::Synapse::Address const& address)
{
	assert(m_program.m_impl != nullptr);
	m_program.m_impl->bld.fire(synapse_driver_mask.to_ulong(), address);
}

void PlaybackProgramBuilder::fire(
	halco::hicann_dls::v2::SynapseDriverOnDLS const& synapse_driver,
	v2::SynapseBlock::Synapse::Address const& address)
{
	assert(m_program.m_impl != nullptr);
	m_program.m_impl->bld.fire_one(synapse_driver.value(), address);
}

void PlaybackProgramBuilder::fire_post_correlation_signal(
	std::bitset<halco::hicann_dls::v2::NeuronOnDLS::size> const& neuron_mask)
{
	assert(m_program.m_impl != nullptr);
	m_program.m_impl->bld.write(0x1a000101, neuron_mask.to_ulong());
}

void PlaybackProgramBuilder::fire_post_correlation_signal(
	halco::hicann_dls::v2::NeuronOnDLS const& neuron)
{
	assert(m_program.m_impl != nullptr);
	m_program.m_impl->bld.write(0x1a000101, 1 << neuron.value());
}

void PlaybackProgramBuilder::halt()
{
	assert(m_program.m_impl != nullptr);
	m_program.m_impl->bld.halt();
}

template <class T>
void PlaybackProgramBuilder::write(
	typename T::coordinate_type const& coord, T const& config)
{
	assert(m_program.m_impl != nullptr);

	typedef std::vector<v2::hardware_address_type> addresses_type;
	addresses_type write_addresses;
	visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});

	typedef std::vector<v2::hardware_word_type> words_type;
	words_type words;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{words});

	if (words.size() != write_addresses.size())
		throw std::logic_error("number of addresses and words do not match");

	auto& impl = *m_program.m_impl;
	auto addr_it = write_addresses.cbegin();
	for (auto const& word : words) {
		impl.bld.write(*addr_it, word);
		++addr_it;
	}
}

template <class T>
PlaybackProgram::ContainerTicket<T> PlaybackProgramBuilder::read(
	typename T::coordinate_type const& coord)
{
	assert(m_program.m_impl != nullptr);

	typedef std::vector<v2::hardware_address_type> addresses_type;
	addresses_type read_addresses;
	{
		T config;
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
	}

	auto& impl = *m_program.m_impl;
	for (auto const& addr : read_addresses) {
		impl.bld.read(addr);
	}

	std::size_t const offset = impl.read_offset;
	std::size_t const length = read_addresses.size();
	impl.read_offset += length;
	return m_program.create_ticket<T>(coord, offset, length);
}

template <>
PlaybackProgram::ContainerTicket<PPUMemoryBlock> PlaybackProgramBuilder::read(
    typename PPUMemoryBlock::coordinate_type const& coord)
{
	assert(m_program.m_impl != nullptr);

	typedef std::vector<v2::hardware_address_type> addresses_type;
	addresses_type read_addresses;
	{
		PPUMemoryBlock config(PPUMemoryBlock::Size(coord.second - coord.first + 1));
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
	}

	auto& impl = *m_program.m_impl;
	for (auto const& addr : read_addresses) {
		impl.bld.read(addr);
	}

	std::size_t const offset = impl.read_offset;
	std::size_t const length = read_addresses.size();
	impl.read_offset += length;
	return m_program.create_ticket<PPUMemoryBlock>(coord, offset, length);
}

PlaybackProgramBuilder::PlaybackProgramBuilder() : m_program(next_serial_number.fetch_add(1)) {}

std::atomic<PlaybackProgram::serial_number_type> PlaybackProgramBuilder::next_serial_number{1};

PlaybackProgram PlaybackProgramBuilder::done()
{
	assert(m_program.m_impl != nullptr);

	PlaybackProgram result = std::move(m_program);
	m_program = PlaybackProgram(next_serial_number.fetch_add(1));
	return result;
}

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	template SYMBOL_VISIBLE void PlaybackProgramBuilder::write<Type>(                      \
		Type::coordinate_type const& coord, Type const& config);
#include "haldls/v2/container.def"

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	template SYMBOL_VISIBLE PlaybackProgram::ContainerTicket<Type>                                 \
	PlaybackProgramBuilder::read<Type>(Type::coordinate_type const& coord);
#include "haldls/v2/container.def"

} // namespace v2
} // namespace haldls
