#pragma once
#include "fisch/vx/container.h"
#include "fisch/vx/container_ticket.h"
#include "haldls/vx/container.h"
#include "haldls/vx/event.h"
#include "hate/visibility.h"
#include "stadls/vx/genpybind.h"
#include <cstdbool>
#include <memory>
#include <variant>
#include <vector>

namespace stadls::vx GENPYBIND_TAG_STADLS_VX {

namespace detail {

template <typename, typename>
class PlaybackProgramBuilderAdapterImpl;

} // namespace detail

/**
 * Ticket for to-be-available container data corresponding to a read instruction.
 */
class GENPYBIND(visible) ContainerTicket
{
public:
	ContainerTicket() = default;

	/**
	 * Get container data if available.
	 * Returns constant reference to owned container instance.
	 * @throws std::runtime_error On container data not available yet
	 * @return Container data
	 */
	haldls::vx::Container const& get() const SYMBOL_VISIBLE GENPYBIND(hidden);

	GENPYBIND_MANUAL({
		parent.def(
		    "get", [](GENPYBIND_PARENT_TYPE const& self) -> std::unique_ptr<haldls::vx::Container> {
			    return self.get().clone_container();
		    });
	})

	/**
	 * Get whether container data is available.
	 * @return Boolean value
	 */
	bool valid() const SYMBOL_VISIBLE;

	/**
	 * Get coordinate corresponding to location of (to-be) read container data.
	 * Returns constant reference to owned coordinate instance.
	 * @return Coordinate value
	 */
	haldls::vx::Container::Coordinate const& get_coordinate() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND_MANUAL({
		parent.def(
		    "get_coordinate",
		    [](GENPYBIND_PARENT_TYPE const& self)
		        -> std::unique_ptr<haldls::vx::Container::Coordinate> {
			    return self.get_coordinate().clone();
		    });
	})

	/**
	 * Get FPGA executor timestamp of last container response if time annotation is enabled.
	 * If time annotation is not enabled, get message count since last time annotation or
	 * from the beginning of the response stream.
	 * @return FPGATime value
	 */
	GENPYBIND(getter_for(fpga_time))
	haldls::vx::FPGATime get_fpga_time() const SYMBOL_VISIBLE;

	ContainerTicket(ContainerTicket const& other) SYMBOL_VISIBLE;
	ContainerTicket(ContainerTicket&& other) = default;
	ContainerTicket& operator=(ContainerTicket const& other) SYMBOL_VISIBLE;
	ContainerTicket& operator=(ContainerTicket&& other) = default;

private:
	template <typename, typename>
	friend class detail::PlaybackProgramBuilderAdapterImpl;

	typedef std::variant<
#define PLAYBACK_CONTAINER(Name, Type) fisch::vx::ContainerTicket<Type>,
#define LAST_PLAYBACK_CONTAINER(Name, Type) fisch::vx::ContainerTicket<Type>
#include "fisch/vx/container.def"
	    >
	    ticket_impl_type;

	ContainerTicket(
	    std::unique_ptr<haldls::vx::Container::Coordinate> coord,
	    std::unique_ptr<haldls::vx::Container> config,
	    ticket_impl_type ticket_impl) :
	    m_coord(std::move(coord)),
	    m_config(std::move(config)),
	    m_ticket_impl(std::move(ticket_impl))
	{}

	std::unique_ptr<haldls::vx::Container::Coordinate> m_coord;
	mutable std::unique_ptr<haldls::vx::Container> m_config;
	ticket_impl_type m_ticket_impl;
}; // ContainerTicket

} // namespace stadls::vx
