#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "haldls/vx/common.h"
#include "haldls/vx/container.h"
#include "hate/visibility.h"
#include "stadls/vx/genpybind.h"

namespace fisch::vx {
class PlaybackProgram;
} // namespace fisch::vx

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
	typedef PlaybackProgram::ContainerTicket<Type> _##Name##ContainerTicket GENPYBIND(opaque);
#include "haldls/vx/container.def"
#endif // __GENPYBIND__

	/** Default constructor. */
	PlaybackProgram() SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PlaybackProgram const& program)
	    SYMBOL_VISIBLE;

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

	bool m_valid;
};

} // namespace vx
} // namespace stadls
