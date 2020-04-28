#include "stadls/vx/playback_program.h"

#include "fisch/vx/container.h"
#include "fisch/vx/playback_program.h"
#include "haldls/vx/common.h"
#include "haldls/vx/container.h"
#include "haldls/vx/coordinate_to_container.h"
#include "lola/vx/container.h"
#include "stadls/visitors.h"

namespace stadls::vx {

PlaybackProgram::PlaybackProgram() :
    m_program_impl(std::make_shared<fisch::vx::PlaybackProgram>()), m_executable_restriction()
{}

PlaybackProgram::PlaybackProgram(
    std::shared_ptr<fisch::vx::PlaybackProgram> const& program_impl,
    std::optional<ExecutorBackend> const executable_restriction) :
    m_program_impl(program_impl), m_executable_restriction(executable_restriction)
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

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	template SYMBOL_VISIBLE Type PlaybackProgram::ContainerTicket<Type>::get() const;              \
	template SYMBOL_VISIBLE bool PlaybackProgram::ContainerTicket<Type>::valid() const;            \
	template SYMBOL_VISIBLE typename Type::coordinate_type                                         \
	PlaybackProgram::ContainerTicket<Type>::get_coordinate() const;                                \
	template SYMBOL_VISIBLE typename PlaybackProgram::fpga_time_type                               \
	PlaybackProgram::ContainerTicket<Type>::get_fpga_time() const;
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/container.def"

typename PlaybackProgram::spikes_type PlaybackProgram::get_spikes() const
{
	spikes_type spikes;
	auto const& spikes_impl = m_program_impl->get_spikes();
	std::copy(spikes_impl.begin(), spikes_impl.end(), std::back_inserter(spikes));
	return spikes;
}

typename PlaybackProgram::madc_samples_type const& PlaybackProgram::get_madc_samples() const
{
	return m_program_impl->get_madc_samples();
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

std::optional<ExecutorBackend> PlaybackProgram::get_executable_restriction() const
{
	return m_executable_restriction;
}

std::ostream& operator<<(std::ostream& os, PlaybackProgram const& program)
{
	os << *(program.m_program_impl);
	return os;
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
