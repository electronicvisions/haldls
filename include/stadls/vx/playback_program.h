#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "fisch/vx/playback_program.h"
#include "haldls/vx/common.h"
#include "haldls/vx/event.h"
#include "hate/visibility.h"
#include "stadls/vx/genpybind.h"
#ifdef __GENPYBIND__
#include "haldls/vx/container.h"
#include "lola/vx/container.h"
#endif

namespace stadls {
namespace vx GENPYBIND_TAG_STADLS_VX {

class PlaybackProgramExecutor;
class PlaybackProgramBuilder;

/**
 * Sequential stream of executable instructions for the executor and result-container for event
 * response data during execution.
 */
class GENPYBIND(visible) PlaybackProgram
{
public:
	typedef fisch::vx::FPGATime fpga_time_type;

	/**
	 * Ticket for to-be-available container data corresponding to a read instruction.
	 * @tparam T Container type
	 */
	template <typename T>
	class ContainerTicket
	{
	public:
		typedef typename T::coordinate_type coordinate_type;

		/**
		 * Get container data if available.
		 * @throws std::runtime_error On container data not available yet
		 * @return Container data
		 */
		T get() const SYMBOL_VISIBLE;

		/**
		 * Get whether container data is available.
		 * @return Boolean value
		 */
		bool valid() const SYMBOL_VISIBLE;

		/**
		 * Get coordinate corresponding to location of (to-be) read container data.
		 * @return Coordinate value
		 */
		coordinate_type get_coordinate() const SYMBOL_VISIBLE;

		/**
		 * Get FPGA executor timestamp of last container response if time annotation is enabled.
		 * If time annotation is not enabled, get message count since last time annotation or
		 * from the beginning of the response stream.
		 * @return FPGATime value
		 */
		GENPYBIND(getter_for(fpga_time))
		fpga_time_type get_fpga_time() const SYMBOL_VISIBLE;

	private:
		typedef typename haldls::vx::detail::to_ticket_variant<
		    typename haldls::vx::detail::BackendContainerTrait<T>::container_list>::type
		    ticket_impl_type;

		friend PlaybackProgramBuilder;

		ContainerTicket(coordinate_type const& coord, ticket_impl_type const& ticket_impl) :
		    m_coord(coord),
		    m_ticket_impl(ticket_impl)
		{}

		coordinate_type m_coord;
		ticket_impl_type m_ticket_impl;
	}; // ContainerTicket

#ifdef __GENPYBIND__
// Explicit instantiation of template class for all valid playback container types.
#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	typedef PlaybackProgram::ContainerTicket<Type> ContainerTicket_##Name GENPYBIND(opaque);
#include "haldls/vx/container.def"
#endif // __GENPYBIND__

#ifdef __GENPYBIND__
// Explicit instantiation of template class for all valid playback container types.
#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	typedef PlaybackProgram::ContainerTicket<Type> ContainerTicket_##Name GENPYBIND(opaque);
#include "lola/vx/container.def"
#endif // __GENPYBIND__

	/** Default constructor. */
	PlaybackProgram() SYMBOL_VISIBLE;

	typedef std::vector<haldls::vx::SpikeFromChip> spikes_type;
	typedef std::vector<haldls::vx::MADCSampleFromChipEvent> madc_samples_type;

	typedef fisch::vx::PlaybackProgram::spike_pack_counts_type spike_pack_counts_type
	    GENPYBIND(visible);
	typedef fisch::vx::PlaybackProgram::madc_sample_pack_counts_type madc_sample_pack_counts_type
	    GENPYBIND(visible);

	/**
	 * Get vector of time-annotated spike events.
	 * @return Vector of spike events
	 */
	GENPYBIND(getter_for(spikes))
	spikes_type get_spikes() const SYMBOL_VISIBLE;

	/**
	 * Get vector of time-annotated MADC sample events.
	 * @return Vector of sample events
	 */
	GENPYBIND(getter_for(madc_samples))
	madc_samples_type const& get_madc_samples() const SYMBOL_VISIBLE;

	/**
	 * Get number of occurences of spike packing from chip.
	 * @return Array of packing occurences
	 */
	GENPYBIND(getter_for(spikes_pack_counts))
	spike_pack_counts_type const& get_spikes_pack_counts() const SYMBOL_VISIBLE;

	/**
	 * Get number of occurences of MADC sample packing from chip.
	 * @return Array of packing occurences
	 */
	GENPYBIND(getter_for(madc_samples_pack_counts))
	madc_sample_pack_counts_type const& get_madc_samples_pack_counts() const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PlaybackProgram const& program)
	    SYMBOL_VISIBLE;

	bool operator==(PlaybackProgram const& other) const SYMBOL_VISIBLE;
	bool operator!=(PlaybackProgram const& other) const SYMBOL_VISIBLE;

private:
	friend PlaybackProgramBuilder;
	friend PlaybackProgramExecutor;

	/**
	 * Construct PlaybackProgram from implementation.
	 * Used in PlaybackProgramBuilder
	 * @param program_impl Implementation playback program
	 */
	PlaybackProgram(std::shared_ptr<fisch::vx::PlaybackProgram> const& program_impl) SYMBOL_VISIBLE;

	std::shared_ptr<fisch::vx::PlaybackProgram> m_program_impl;
};

} // namespace vx
} // namespace stadls
