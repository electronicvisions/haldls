#include "stadls/vx/absolute_time_playback_program_container_ticket.h"

#include <stdexcept>

namespace stadls::vx {

haldls::vx::Container const& AbsoluteTimePlaybackProgramContainerTicket::get() const
{
	if (!valid()) {
		throw std::logic_error("Access is not possible until ContainerTicket is valid");
	}
	return ticket_storage->container_ticket->get();
}

bool AbsoluteTimePlaybackProgramContainerTicket::valid() const
{
	return ticket_storage && ticket_storage->container_ticket &&
	       ticket_storage->container_ticket->valid();
}

haldls::vx::Container::Coordinate const&
AbsoluteTimePlaybackProgramContainerTicket::get_coordinate() const
{
	if (!valid()) {
		throw std::logic_error("Access is not possible until ContainerTicket is valid");
	}
	return ticket_storage->container_ticket->get_coordinate();
}

haldls::vx::FPGATime AbsoluteTimePlaybackProgramContainerTicket::get_fpga_time() const
{
	if (!valid()) {
		throw std::logic_error("Access is not possible until ContainerTicket is valid");
	}
	return ticket_storage->container_ticket->get_fpga_time();
}


AbsoluteTimePlaybackProgramContainerTicket::AbsoluteTimePlaybackProgramContainerTicket(
    std::shared_ptr<AbsoluteTimePlaybackProgramContainerTicketStorage> const& ticket_storage) :
    ticket_storage(ticket_storage)
{}
} // namespace stadls::vx
