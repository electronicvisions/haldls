#pragma once

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "fisch/vx/jtag.h"
#include "fisch/vx/playback_program.h"
#include "fisch/vx/reset.h"
#include "fisch/vx/timer.h"

#include "haldls/vx/common.h"
#include "haldls/vx/genpybind.h"
#include "hate/visibility.h"

#include "haldls/vx/ppu.h"

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

class PlaybackProgramBuilder;

class GENPYBIND(visible) PlaybackProgram
{
public:
	template <typename T>
	class ContainerTicket
	{
	public:
		typedef typename T::coordinate_type coordinate_type;

		T get() const SYMBOL_VISIBLE;

		bool valid() const SYMBOL_VISIBLE;

		coordinate_type get_coordinate() const SYMBOL_VISIBLE;

	private:
		typedef std::conditional_t<
		    detail::BackendTrait<T>::valid(Backend::OmnibusOnChipOverJTAG),
		    fisch::vx::PlaybackProgram::ContainerVectorTicket<fisch::vx::OmnibusOnChipOverJTAG>,
		    fisch::vx::PlaybackProgram::ContainerTicket<T>>
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
#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	typedef PlaybackProgram::ContainerTicket<Type> _##Name##ContainerTicket GENPYBIND(opaque);
#include "fisch/vx/container.def"
#endif // __GENPYBIND__

	PlaybackProgram() SYMBOL_VISIBLE;
	PlaybackProgram(std::shared_ptr<fisch::vx::PlaybackProgram> const& other) SYMBOL_VISIBLE;

	friend std::ostream& operator<<(std::ostream& os, PlaybackProgram const& program)
	    SYMBOL_VISIBLE;

	/**
	 * Access the implementation for ease of insertion in fisch executor.
	 * FIXME: This is not supposed to be publically accessible.
	 */
	std::shared_ptr<fisch::vx::PlaybackProgram> impl() const SYMBOL_VISIBLE;

private:
	friend PlaybackProgramBuilder;

	std::shared_ptr<fisch::vx::PlaybackProgram> m_program_impl;

	bool m_valid;
}; // PlaybackProgram

class GENPYBIND(visible) PlaybackProgramBuilder
{
public:
	PlaybackProgramBuilder() SYMBOL_VISIBLE;

	void wait_until(
	    typename fisch::vx::Timer::coordinate_type const& coord,
	    fisch::vx::Timer::Value t) SYMBOL_VISIBLE;

	template <class T>
	void write(
	    typename T::coordinate_type const& coord,
	    T const& config,
	    std::optional<Backend> backend = std::nullopt);

	template <class T>
	PlaybackProgram::ContainerTicket<T> read(
	    typename T::coordinate_type const& coord,
	    std::optional<Backend> backend = std::nullopt) SYMBOL_VISIBLE;

	void halt() SYMBOL_VISIBLE;

	PlaybackProgram done() SYMBOL_VISIBLE;

private:
	std::unique_ptr<fisch::vx::PlaybackProgramBuilder> m_builder_impl;
}; // PlaybackProgramBuilder

#ifdef __GENPYBIND__
// Explicit instantiation of template member functions for all valid playback container types.
#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	extern template SYMBOL_VISIBLE void PlaybackProgramBuilder::write<Type>(                       \
	    Type::coordinate_type const& coord, Type const& config,                                    \
	    std::optional<Backend> backend = std::nullopt);
#include "haldls/vx/container.def"

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	extern template SYMBOL_VISIBLE void PlaybackProgramBuilder::write<Type>(                       \
	    Type::coordinate_type const& coord, Type const& config,                                    \
	    std::optional<Backend> backend = std::nullopt);
#include "fisch/vx/container.def"

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	extern template PlaybackProgram::ContainerTicket<Type> PlaybackProgramBuilder::read<Type>(     \
	    typename Type::coordinate_type const& coord,                                               \
	    std::optional<Backend> backend = std::nullopt);                                            \
	extern template class PlaybackProgram::ContainerTicket<Type>;
#include "haldls/vx/container.def"

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	extern template PlaybackProgram::ContainerTicket<Type> PlaybackProgramBuilder::read<Type>(     \
	    typename Type::coordinate_type const& coord,                                               \
	    std::optional<Backend> backend = std::nullopt);                                            \
	extern template class PlaybackProgram::ContainerTicket<Type>;
#include "fisch/vx/container.def"
#endif // __GENPYBIND__

} // namespace vx
} // namespace haldls
