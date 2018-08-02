#pragma once

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "halco/common/genpybind.h"

#include "haldls/v2/common.h"
#include "haldls/v2/spike.h"
#include "haldls/v2/synapse.h"
#include "hate/visibility.h"

namespace stadls {
namespace v2 {
class LocalBoardControl;
} // namespace v2
} // namespace stadls

namespace haldls {
namespace v2 GENPYBIND(tag(haldls_v2)) {

class PlaybackProgramBuilder;

class GENPYBIND(visible) PlaybackProgram
{
public:
	typedef std::vector<v2::RecordedSpike> spikes_type;
	typedef std::size_t serial_number_type;
	static constexpr serial_number_type invalid_serial_number SYMBOL_VISIBLE = 0;

	template <typename T>
	class ContainerTicket
	{
	private:
		friend PlaybackProgram;

		typedef typename T::coordinate_type coordinate_type;

		ContainerTicket(
			serial_number_type serial_number,
			coordinate_type const& coord,
			std::size_t offset,
			std::size_t length)
			: serial_number(serial_number), coord(coord), offset(offset), length(length)
		{}

		serial_number_type serial_number;
		coordinate_type coord;
		std::size_t offset;
		std::size_t length;
	}; // ContainerTicket

#ifdef __GENPYBIND__
// Explicit instantiation of template class for all valid playback container types.
#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	typedef PlaybackProgram::ContainerTicket<Type> _##Name##ContainerTicket GENPYBIND(opaque);
#include "haldls/v2/container.def"
#endif // __GENPYBIND__

	PlaybackProgram() SYMBOL_VISIBLE;
	PlaybackProgram(PlaybackProgram const& other) SYMBOL_VISIBLE;
	PlaybackProgram& operator=(PlaybackProgram const& other) SYMBOL_VISIBLE;
	PlaybackProgram(PlaybackProgram&& other) noexcept SYMBOL_VISIBLE;
	PlaybackProgram& operator=(PlaybackProgram&& other) noexcept SYMBOL_VISIBLE;
	~PlaybackProgram() SYMBOL_VISIBLE;

	template <typename T>
	T get(ContainerTicket<T> const& ticket) const;

	spikes_type const& get_spikes() const SYMBOL_VISIBLE;

	serial_number_type serial_number() const SYMBOL_VISIBLE;

	std::string dump_program() const SYMBOL_VISIBLE;

	std::vector<std::vector<instruction_word_type> > const& instruction_byte_blocks() const
		SYMBOL_VISIBLE;

	friend stadls::v2::LocalBoardControl;
	friend PlaybackProgramBuilder;

private:
	PlaybackProgram(serial_number_type serial_number);

	/// \see PlaybackProgramBuilder
	template <typename T>
	ContainerTicket<T> create_ticket(
		typename T::coordinate_type const& coord, std::size_t offset, std::size_t length) const;

	template <typename T>
	static void ensure_container_invariants(T& config);

	/// \see LocalBoardControl
	void set_results(std::vector<v2::hardware_word_type>&& results) SYMBOL_VISIBLE;

	/// \see LocalBoardControl
	void set_spikes(spikes_type&& spikes) SYMBOL_VISIBLE;

	struct Impl;
	std::unique_ptr<Impl> m_impl;
	/// Serial number of the build, used to differentiate container tickets.
	serial_number_type m_serial_number = invalid_serial_number;
}; // PlaybackProgram

class GENPYBIND(visible) PlaybackProgramBuilder
{
public:
	typedef v2::hardware_time_type time_type;

	PlaybackProgramBuilder() SYMBOL_VISIBLE;

	void set_time(time_type t) SYMBOL_VISIBLE;
	void wait_until(time_type t) SYMBOL_VISIBLE;
	void wait_for(time_type t) SYMBOL_VISIBLE;
	void fire(
		std::bitset<halco::hicann_dls::v2::SynapseDriverOnDLS::size> const& synapse_driver_mask,
		v2::SynapseBlock::Synapse::Address const& address) SYMBOL_VISIBLE;
	void fire(
		halco::hicann_dls::v2::SynapseDriverOnDLS const& synapse_driver,
		v2::SynapseBlock::Synapse::Address const& address) SYMBOL_VISIBLE;
	void halt() SYMBOL_VISIBLE;

	template <class T>
	void write(typename T::coordinate_type const& coord, T const& config);

	/// \note As this variant requires an explicitly specified template parameter it is
	///       not exposed to the python wrapping.
	template <class T>
	PlaybackProgram::ContainerTicket<T> read(typename T::coordinate_type const& coord);

	/// \note The extra argument is only used to select the correct template instantiation.
	template <class T>
	PlaybackProgram::ContainerTicket<T> read(
		typename T::coordinate_type const& coord, T const& /*config*/)
	{
		return read<T>(coord);
	}

	PlaybackProgram done() SYMBOL_VISIBLE;

private:
	static std::atomic<PlaybackProgram::serial_number_type> next_serial_number;
	PlaybackProgram m_program;
}; // PlaybackProgramBuilder

#ifdef __GENPYBIND__
// Explicit instantiation of template member functions for all valid playback container types.
#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	extern template void PlaybackProgramBuilder::write<Type>(                                      \
		Type::coordinate_type const&, Type const&);                                                \
	extern template PlaybackProgram::ContainerTicket<Type>                                         \
	PlaybackProgramBuilder::read<Type>(Type::coordinate_type const&, Type const&);                 \
	extern template Type PlaybackProgram::get(                                                     \
		PlaybackProgram::ContainerTicket<Type> const& ticket) const;
#include "haldls/v2/container.def"
#endif // __GENPYBIND__

} // namespace v2
} // namespace haldls
