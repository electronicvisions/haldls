#pragma once

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "haldls/common/visibility.h"
#include "haldls/container/v2/common.h"
#include "haldls/container/v2/spike.h"
#include "haldls/container/v2/synapse.h"

namespace haldls {
namespace io {
namespace v2 {

class ExperimentControl;
class PlaybackProgramBuilder;

class PlaybackProgram {
public:
	typedef std::vector<container::v2::RecordedSpike> spikes_type;
	typedef std::size_t serial_number_type;
	static constexpr serial_number_type invalid_serial_number = 0;

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

	PlaybackProgram();
	PlaybackProgram(PlaybackProgram const& other);
	PlaybackProgram& operator=(PlaybackProgram const& other);
	PlaybackProgram(PlaybackProgram&& other) noexcept;
	PlaybackProgram& operator=(PlaybackProgram&& other) noexcept;
	~PlaybackProgram();

	template <typename T>
	T get(ContainerTicket<T> const& ticket) const HALDLS_VISIBLE;

	spikes_type const& get_spikes() const;

	serial_number_type serial_number() const HALDLS_VISIBLE;

	std::string dump_program() const;

	friend ExperimentControl;
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
	std::vector<std::vector<std::uint8_t> > const& instruction_byte_blocks() const;

	/// \see ExperimentControl
	void set_results(std::vector<container::v2::hardware_word_type>&& results);

	/// \see ExperimentControl
	void set_spikes(spikes_type&& spikes);

	struct Impl;
	std::unique_ptr<Impl> m_impl;
	/// Serial number of the build, used to differentiate container tickets.
	serial_number_type m_serial_number = invalid_serial_number;
}; // PlaybackProgram

class PlaybackProgramBuilder {
public:
	typedef container::v2::hardware_time_type time_type;

	PlaybackProgramBuilder();

	void set_time(time_type t) HALDLS_VISIBLE;
	void wait_until(time_type t) HALDLS_VISIBLE;
	void wait_for(time_type t) HALDLS_VISIBLE;
	void fire(
		std::bitset<halco::hicann_dls::v2::SynapseDriverOnDLS::size> const& synapse_driver_mask,
		container::v2::SynapseBlock::Synapse::Address const& address);
	void fire(
		halco::hicann_dls::v2::SynapseDriverOnDLS const& synapse_driver,
		container::v2::SynapseBlock::Synapse::Address const& address);
	void halt() HALDLS_VISIBLE;

	template <class T>
	void set_container(typename T::coordinate_type const& coord, T const& config) HALDLS_VISIBLE;

	template <class T>
	PlaybackProgram::ContainerTicket<T> get_container(typename T::coordinate_type const& coord) HALDLS_VISIBLE;

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

} // namespace v2
} // namespace io
} // namespace haldls
