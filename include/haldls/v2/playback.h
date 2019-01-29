#pragma once

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>


#include "haldls/v2/common.h"
#include "haldls/v2/genpybind.h"
#include "haldls/v2/spike.h"
#include "haldls/v2/synapse.h"
#include "hate/visibility.h"

namespace stadls {
namespace v2 {
class LocalBoardControl;
} // namespace v2
} // namespace stadls

namespace haldls {
namespace v2 GENPYBIND_TAG_HALDLS_V2 {

class PlaybackProgramBuilder;

class GENPYBIND(visible, holder_type("std::shared_ptr<haldls::v2::PlaybackProgram>"))
	PlaybackProgram : public std::enable_shared_from_this<PlaybackProgram>
{
public:
	typedef std::vector<v2::RecordedSpike> spikes_type;

	template <typename T>
	class ContainerTicket
	{
	public:
		T get() const;

	private:
		friend class PlaybackProgram;

		typedef typename T::coordinate_type coordinate_type;

		ContainerTicket(
			std::shared_ptr<PlaybackProgram const> pbp,
			coordinate_type const& coord,
			std::size_t offset,
			std::size_t length)
			: pbp(pbp), coord(coord), offset(offset), length(length)
		{}

		std::shared_ptr<PlaybackProgram const> pbp;
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
	PlaybackProgram(PlaybackProgram const& other) = delete;
	PlaybackProgram& operator=(PlaybackProgram const& other) = delete;
	PlaybackProgram(PlaybackProgram&& other) = delete;
	PlaybackProgram& operator=(PlaybackProgram&& other) = delete;
	~PlaybackProgram() SYMBOL_VISIBLE;

	spikes_type const& get_spikes() const SYMBOL_VISIBLE;

	std::string dump_program() const SYMBOL_VISIBLE;

	std::vector<std::vector<instruction_word_type> > const& instruction_byte_blocks() const
		SYMBOL_VISIBLE;

	/**
	 * Get information on whether the program stores valid executable instructions.
	 * @return Boolean value
	 */
	bool const& valid() const SYMBOL_VISIBLE;

	friend stadls::v2::LocalBoardControl;
	friend PlaybackProgramBuilder;

private:

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
	bool m_valid;
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
	void fire_post_correlation_signal(
		std::bitset<halco::hicann_dls::v2::NeuronOnDLS::size> const& neuron_mask) SYMBOL_VISIBLE;
	void fire_post_correlation_signal(
		halco::hicann_dls::v2::NeuronOnDLS const& neuron) SYMBOL_VISIBLE;
	void halt() SYMBOL_VISIBLE;

	template <class T>
	void write(typename T::coordinate_type const& coord, T const& config);

	template <class T>
	PlaybackProgram::ContainerTicket<T> read(typename T::coordinate_type const& coord) SYMBOL_VISIBLE;

	std::shared_ptr<PlaybackProgram> done() SYMBOL_VISIBLE;

private:
	std::shared_ptr<PlaybackProgram> m_program;
}; // PlaybackProgramBuilder

#ifdef __GENPYBIND__
// Explicit instantiation of template member functions for all valid playback container types.
#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	extern template void PlaybackProgramBuilder::write<Type>(                                      \
	    Type::coordinate_type const&, Type const&);                                                \
	extern template PlaybackProgram::ContainerTicket<Type> PlaybackProgramBuilder::read<Type>(     \
	    Type::coordinate_type const&);                                                             \
	extern template Type PlaybackProgram::ContainerTicket<Type>::get() const;
#include "haldls/v2/container.def"
#endif // __GENPYBIND__

} // namespace v2
} // namespace haldls
