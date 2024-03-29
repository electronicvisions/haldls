#include "stadls/vx/playback_program.h"

#include "fisch/vx/playback_program.h"

namespace stadls::vx {

PlaybackProgram::PlaybackProgram() :
    m_program_impl(std::make_shared<fisch::vx::PlaybackProgram>()), m_unsupported_targets()
{}

PlaybackProgram::PlaybackProgram(
    std::shared_ptr<fisch::vx::PlaybackProgram> const& program_impl,
    std::unordered_set<hxcomm::vx::Target> const unsupported_targets) :
    m_program_impl(program_impl), m_unsupported_targets(unsupported_targets)
{}

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

} // namespace stadls::vx
