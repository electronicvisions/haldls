#pragma once

#include "haldls/vx/container.h"
#include "haldls/vx/event.h"
#include "hate/visibility.h"
#include "stadls/vx/container_ticket.h"
#include "stadls/vx/genpybind.h"

#include <memory>
#include <optional>

namespace stadls::vx GENPYBIND_TAG_STADLS_VX {

struct AbsoluteTimePlaybackProgramContainerTicketStorage
{
	std::optional<ContainerTicket> container_ticket = std::nullopt;
};

template <typename>
class AbsoluteTimePlaybackProgramBuilder;

/**
 * Ticket for to-be-available container data corresponding to a read instruction.
 */
class GENPYBIND(visible) AbsoluteTimePlaybackProgramContainerTicket
{
public:
	/**
	 * Get container data if available
	 * @throws std::runtime_error On container data not available yet
	 * @return Container data
	 */
	haldls::vx::Container const& get() const SYMBOL_VISIBLE GENPYBIND(hidden);

	/**
	 * Get whether container data is available
	 * @return Boolean value
	 */
	bool valid() const SYMBOL_VISIBLE;

	/**
	 * Get coordinate corresponding to location of (to be) read container data
	 * @return Coordinate value
	 */
	haldls::vx::Container::Coordinate const& get_coordinate() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	/**
	 * Get FPGA executor timestamp of last container response if time annotation is enabled.
	 * If time annotation is not enabled, get message count since last time annotation or
	 * from the beginning of the response stream.
	 * @return FPGATime value
	 */
	GENPYBIND(getter_for(fpga_time))
	haldls::vx::FPGATime get_fpga_time() const SYMBOL_VISIBLE;

private:
	template <typename>
	friend class AbsoluteTimePlaybackProgramBuilder;
	std::shared_ptr<AbsoluteTimePlaybackProgramContainerTicketStorage> ticket_storage = nullptr;

	AbsoluteTimePlaybackProgramContainerTicket(
	    std::shared_ptr<AbsoluteTimePlaybackProgramContainerTicketStorage> const& ticket_storage);
}; // AbsoluteTimePlaybackProgramContainerTicket

} // namespace stadls::vx
