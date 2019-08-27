#include "stadls/vx/playback_program.h"

#include "fisch/vx/playback_program.h"
#include "haldls/vx/common.h"
#include "stadls/visitors.h"

namespace stadls::vx {

PlaybackProgram::PlaybackProgram() : m_program_impl() {}

PlaybackProgram::PlaybackProgram(std::shared_ptr<fisch::vx::PlaybackProgram> const& program_impl) :
    m_program_impl(program_impl)
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
		    T config;
		    visit_preorder(config, m_coord, stadls::DecodeVisitor<decltype(data)>{std::move(data)});
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

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	template SYMBOL_VISIBLE Type PlaybackProgram::ContainerTicket<Type>::get() const;              \
	template SYMBOL_VISIBLE bool PlaybackProgram::ContainerTicket<Type>::valid() const;            \
	template SYMBOL_VISIBLE typename Type::coordinate_type                                         \
	PlaybackProgram::ContainerTicket<Type>::get_coordinate() const;
#include "haldls/vx/container.def"

std::ostream& operator<<(std::ostream& os, PlaybackProgram const& program)
{
	os << *(program.m_program_impl);
	return os;
}

} // namespace stadls::vx
