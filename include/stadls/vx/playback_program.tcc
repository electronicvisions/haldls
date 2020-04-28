#include "stadls/vx/playback_program.h"

#include "fisch/vx/container.h"
#include "fisch/vx/container_cast.h"
#include "fisch/vx/playback_program.h"
#include "haldls/cerealization.tcc"
#include "haldls/vx/common.h"
#include "haldls/vx/coordinate_to_container.h"
#include "stadls/visitors.h"
#include "stadls/vx/set_decode.h"
#include "stadls/vx/supports_empty.h"
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_set.hpp>

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
		    auto data = ticket_impl.get();
		    std::vector<decltype(
		        fisch::vx::container_cast(std::declval<typename decltype(data)::value_type>()))>
		        data_values;
		    data_values.reserve(data.size());
		    std::transform(
		        data.begin(), data.end(), std::back_inserter(data_values),
		        (typename decltype(data_values)::value_type (*)(
		            typename decltype(data)::value_type const&)) &
		            fisch::vx::container_cast);
		    auto config =
		        haldls::vx::detail::coordinate_to_container<decltype(m_coord), T>(m_coord);
		    set_decode(config, m_coord, data_values);
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
	auto const& spikes_impl = m_program_impl->get_spikes();
	return spikes_type(spikes_impl.begin(), spikes_impl.end());
}

typename PlaybackProgram::madc_samples_type PlaybackProgram::get_madc_samples() const
{
	auto const& samples_impl = m_program_impl->get_madc_samples();
	return madc_samples_type(samples_impl.begin(), samples_impl.end());
}

typename PlaybackProgram::highspeed_link_notifications_type
PlaybackProgram::get_highspeed_link_notifications() const
{
	auto const& notifications_impl = m_program_impl->get_highspeed_link_notifications();
	return highspeed_link_notifications_type(notifications_impl.begin(), notifications_impl.end());
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

template <typename Archive>
void PlaybackProgram::serialize(Archive& ar, uint32_t)
{
	ar(CEREAL_NVP(m_program_impl));
	ar(CEREAL_NVP(m_unsupported_targets));
}

} // namespace stadls::vx
