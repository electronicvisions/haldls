#pragma once

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "halco/common/genpybind.h"

#include "haldls/common/visibility.h"
#include "haldls/container/v2/common.h"
#include "haldls/container/v2/spike.h"
#include "haldls/container/v2/synapse.h"

namespace haldls {
namespace io {
namespace v2 {
	class ExperimentControl;
} // io
} // v2

namespace container {
namespace v2 GENPYBIND(tag(haldls_container_v2)) {

class PlaybackProgramBuilder;

class GENPYBIND(visible) PlaybackProgram {
public:
	typedef std::vector<container::v2::RecordedSpike> spikes_type;
	typedef std::size_t serial_number_type;
	static constexpr serial_number_type invalid_serial_number HALDLS_VISIBLE = 0;

	template <typename T>
	class ContainerTicket {
	private:
		friend PlaybackProgram;

		typedef typename T::coordinate_type coordinate_type;

		ContainerTicket(
			serial_number_type serial_number, coordinate_type const& coord, std::size_t offset, std::size_t length)
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
#include "haldls/container/v2/container.def"
#endif // __GENPYBIND__

	PlaybackProgram() HALDLS_VISIBLE;
	PlaybackProgram(PlaybackProgram const& other) HALDLS_VISIBLE;
	PlaybackProgram& operator=(PlaybackProgram const& other) HALDLS_VISIBLE;
	PlaybackProgram(PlaybackProgram&& other) noexcept HALDLS_VISIBLE;
	PlaybackProgram& operator=(PlaybackProgram&& other) noexcept HALDLS_VISIBLE;
	~PlaybackProgram() HALDLS_VISIBLE;

	template <typename T>
	T get(ContainerTicket<T> const& ticket) const;

	spikes_type const& get_spikes() const HALDLS_VISIBLE;

	serial_number_type serial_number() const HALDLS_VISIBLE;

	std::string dump_program() const HALDLS_VISIBLE;

	friend io::v2::ExperimentControl;
	friend PlaybackProgramBuilder;

private:
	PlaybackProgram(serial_number_type serial_number);

	/// \see PlaybackProgramBuilder
	template <typename T>
	ContainerTicket<T> create_ticket(
		typename T::coordinate_type const& coord, std::size_t offset, std::size_t length) const;

	template <typename T>
	static void ensure_container_invariants(T& config);

	/// \see ExperimentControl
	std::vector<std::vector<std::uint8_t> > const& instruction_byte_blocks() const HALDLS_VISIBLE;

	/// \see ExperimentControl
	void set_results(std::vector<container::v2::hardware_word_type>&& results) HALDLS_VISIBLE;

	/// \see ExperimentControl
	void set_spikes(spikes_type&& spikes) HALDLS_VISIBLE;

	struct Impl;
	std::unique_ptr<Impl> m_impl;
	/// Serial number of the build, used to differentiate container tickets.
	serial_number_type m_serial_number = invalid_serial_number;
}; // PlaybackProgram

class GENPYBIND(visible) PlaybackProgramBuilder {
public:
	typedef container::v2::hardware_time_type time_type;

	PlaybackProgramBuilder() HALDLS_VISIBLE;

	void set_time(time_type t) HALDLS_VISIBLE;
	void wait_until(time_type t) HALDLS_VISIBLE;
	void wait_for(time_type t) HALDLS_VISIBLE;
	void fire(
		std::bitset<halco::hicann_dls::v2::SynapseDriverOnDLS::size> const& synapse_driver_mask,
		container::v2::SynapseBlock::Synapse::Address const& address) HALDLS_VISIBLE;
	void fire(
		halco::hicann_dls::v2::SynapseDriverOnDLS const& synapse_driver,
		container::v2::SynapseBlock::Synapse::Address const& address) HALDLS_VISIBLE;
	void halt() HALDLS_VISIBLE;

	template <class T>
	void set_container(typename T::coordinate_type const& coord, T const& config);

	/// \note As this variant requires an explicitly specified template parameter it is
	///       not exposed to the python wrapping.
	template <class T>
	PlaybackProgram::ContainerTicket<T> get_container(typename T::coordinate_type const& coord);

	/// \note The extra argument is only used to select the correct template instantiation.
	template <class T>
	PlaybackProgram::ContainerTicket<T> get_container(
		typename T::coordinate_type const& coord, T const& /*config*/)
	{
		return get_container<T>(coord);
	}

	PlaybackProgram done() HALDLS_VISIBLE;

private:
	static std::atomic<PlaybackProgram::serial_number_type> next_serial_number;
	PlaybackProgram m_program;
}; // PlaybackProgramBuilder

#ifdef __GENPYBIND__
// Explicit instantiation of template member functions for all valid playback container types.
#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	extern template void PlaybackProgramBuilder::set_container<Type>(                              \
		Type::coordinate_type const&, Type const&);                                                \
	extern template PlaybackProgram::ContainerTicket<Type>                                         \
	PlaybackProgramBuilder::get_container<Type>(Type::coordinate_type const&, Type const&);        \
	extern template Type PlaybackProgram::get(                                                     \
		PlaybackProgram::ContainerTicket<Type> const& ticket) const;
#include "haldls/container/v2/container.def"
#endif // __GENPYBIND__

} // namespace v2
} // namespace container
} // namespace haldls
