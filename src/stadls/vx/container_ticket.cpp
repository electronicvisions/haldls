#include "stadls/vx/container_ticket.h"

#include "fisch/vx/container.h"
#include "fisch/vx/container_cast.h"
#include "haldls/vx/container.h"
#include "haldls/vx/event.h"
#include <stdexcept>

namespace stadls::vx {

ContainerTicket::ContainerTicket(ContainerTicket const& other) :
    m_coord(other.m_coord ? other.m_coord->clone() : nullptr),
    m_config(other.m_config ? other.m_config->clone_container() : nullptr),
    m_ticket_impl(other.m_ticket_impl)
{}

ContainerTicket& ContainerTicket::operator=(ContainerTicket const& other)
{
	if (this != &other) {
		m_coord = other.m_coord ? other.m_coord->clone() : nullptr;
		m_config = other.m_config ? other.m_config->clone_container() : nullptr;
		m_ticket_impl = other.m_ticket_impl;
	}
	return *this;
}

haldls::vx::Container const& ContainerTicket::get() const
{
	if (!m_coord || !m_config) {
		throw std::logic_error("Unexpected access to moved-from object.");
	}

	auto const visitor =
	    [](auto const& ticket_impl) -> haldls::vx::Container::BackendContainerListVariant {
		auto const data = ticket_impl.get();
		std::vector<decltype(
		    fisch::vx::container_cast(std::declval<typename decltype(data)::value_type>()))>
		    data_values;
		data_values.reserve(data.size());
		std::transform(
		    data.begin(), data.end(), std::back_inserter(data_values),
		    (typename decltype(data_values)::value_type (*)(
		        typename decltype(data)::value_type const&)) &
		        fisch::vx::container_cast);
		return data_values;
	};

	auto const data = std::visit(visitor, m_ticket_impl);

	m_config->decode_read(data, *m_coord);

	return *m_config;
}

bool ContainerTicket::valid() const
{
	return std::visit([](auto const& ticket_impl) { return ticket_impl.valid(); }, m_ticket_impl);
}

haldls::vx::Container::Coordinate const& ContainerTicket::get_coordinate() const
{
	if (!m_coord) {
		throw std::logic_error("Unexpected access to moved-from object.");
	}
	return *m_coord;
}

haldls::vx::FPGATime ContainerTicket::get_fpga_time() const
{
	return std::visit(
	    [](auto const& ticket_impl) { return ticket_impl.fpga_time(); }, m_ticket_impl);
}

} // namespace stadls::vx
