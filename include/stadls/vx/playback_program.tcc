#include "stadls/vx/playback_program.h"

#include "fisch/vx/container.h"
#include "fisch/vx/playback_program.h"
#include "haldls/vx/common.h"
#include "haldls/vx/coordinate_to_container.h"
#include "stadls/visitors.h"

namespace stadls::vx {

PlaybackProgram::PlaybackProgram() :
    m_program_impl(std::make_shared<fisch::vx::PlaybackProgram>()), m_unsupported_targets()
{}

PlaybackProgram::PlaybackProgram(
    std::shared_ptr<fisch::vx::PlaybackProgram> const& program_impl,
    std::unordered_set<hxcomm::vx::Target> const unsupported_targets) :
    m_program_impl(program_impl), m_unsupported_targets(unsupported_targets)
{}

template <typename T>
T PlaybackProgram::ContainerTicket<T>::get() const
{
	return boost::apply_visitor(
	    [this](auto&& ticket_impl) -> T {
		    if (!ticket_impl.valid())
			    throw std::runtime_error(
			        "container data not available yet (out of bounds of available results data)");

		    auto data = ticket_impl.get();
		    auto config =
		        haldls::vx::detail::coordinate_to_container<decltype(m_coord), T>(m_coord);
		    haldls::vx::visit_preorder(
		        config, m_coord, stadls::DecodeVisitor<decltype(data)>{std::move(data)});
		    return config;
	    },
	    m_ticket_impl);
}

template <typename T>
bool PlaybackProgram::ContainerTicket<T>::valid() const
{
	return boost::apply_visitor(
	    [this](auto&& ticket_impl) -> bool { return ticket_impl.valid(); }, m_ticket_impl);
}

template <typename T>
typename T::coordinate_type PlaybackProgram::ContainerTicket<T>::get_coordinate() const
{
	return m_coord;
}

template <typename T>
typename PlaybackProgram::fpga_time_type PlaybackProgram::ContainerTicket<T>::get_fpga_time() const
{
	return boost::apply_visitor(
	    [this](auto&& ticket_impl) -> fpga_time_type { return ticket_impl.fpga_time(); },
	    m_ticket_impl);
}

typename PlaybackProgram::spikes_type PlaybackProgram::get_spikes() const
{
	spikes_type spikes;
	auto const& spikes_impl = m_program_impl->get_spikes();
	std::copy(spikes_impl.begin(), spikes_impl.end(), std::back_inserter(spikes));
	return spikes;
}

typename PlaybackProgram::madc_samples_type PlaybackProgram::get_madc_samples() const
{
	madc_samples_type samples;
	auto const& samples_impl = m_program_impl->get_madc_samples();
	std::copy(samples_impl.begin(), samples_impl.end(), std::back_inserter(samples));
	return samples;
}

typename PlaybackProgram::highspeed_link_notifications_type
PlaybackProgram::get_highspeed_link_notifications() const
{
	highspeed_link_notifications_type notifications;
	auto const& notifications_impl = m_program_impl->get_highspeed_link_notifications();
	std::copy(
	    notifications_impl.begin(), notifications_impl.end(), std::back_inserter(notifications));
	return notifications;
}

typename PlaybackProgram::spike_pack_counts_type const& PlaybackProgram::get_spikes_pack_counts()
    const
{
	return m_program_impl->get_spikes_pack_counts();
}

typename PlaybackProgram::madc_sample_pack_counts_type const&
PlaybackProgram::get_madc_samples_pack_counts() const
{
	return m_program_impl->get_madc_samples_pack_counts();
}

std::unordered_set<hxcomm::vx::Target> const& PlaybackProgram::get_unsupported_targets() const
{
	return m_unsupported_targets;
}

std::ostream& operator<<(std::ostream& os, PlaybackProgram const& program)
{
	os << *(program.m_program_impl);
	return os;
}

bool PlaybackProgram::empty() const
{
	return m_program_impl->empty();
}

bool PlaybackProgram::operator==(PlaybackProgram const& other) const
{
	return *m_program_impl == *(other.m_program_impl);
}

bool PlaybackProgram::operator!=(PlaybackProgram const& other) const
{
	return !(*this == other);
}

} // namespace stadls::vx
