#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "haldls/vx/jtag.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/spi.h"
#include "haldls/vx/timer.h"

#include "haldls/vx/common.h"
#include "haldls/vx/genpybind.h"
#include "hate/visibility.h"

#include "haldls/vx/ppu.h"

namespace fisch::vx {
	class PlaybackProgram;
	class PlaybackProgramBuilder;
} // namespace fisch::vx

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
		typedef typename detail::to_ticket_variant<
		    typename detail::BackendContainerTrait<T>::container_list>::type ticket_impl_type;

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
#endif // __GENPYBIND__

	PlaybackProgram() SYMBOL_VISIBLE;
	PlaybackProgram(std::shared_ptr<fisch::vx::PlaybackProgram> const& other) SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
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
	    typename haldls::vx::Timer::coordinate_type const& coord,
	    haldls::vx::Timer::Value t) SYMBOL_VISIBLE;

	template <class T>
	void write(
	    typename T::coordinate_type const& coord,
	    T const& config,
	    std::optional<Backend> backend = std::nullopt) SYMBOL_VISIBLE;

#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	void write(typename Type::coordinate_type const& coord, Type const& config, Backend backend)   \
	    SYMBOL_VISIBLE;                                                                            \
	void write(typename Type::coordinate_type const& coord, Type const& config) SYMBOL_VISIBLE;
#include "haldls/vx/container.def"

	template <class T>
	PlaybackProgram::ContainerTicket<T> read(
	    typename T::coordinate_type const& coord,
	    std::optional<Backend> backend = std::nullopt) SYMBOL_VISIBLE;

#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	PlaybackProgram::ContainerTicket<Type> read(                                                   \
	    typename Type::coordinate_type const& coord, Type const& config, Backend backend)          \
	    SYMBOL_VISIBLE;                                                                            \
	PlaybackProgram::ContainerTicket<Type> read(                                                   \
	    typename Type::coordinate_type const& coord, Type const& config) SYMBOL_VISIBLE;
#include "haldls/vx/container.def"

	void halt() SYMBOL_VISIBLE;

	PlaybackProgram done() SYMBOL_VISIBLE;

private:
	template <class T, size_t... SupportedBackendIndex>
	void write_table_generator(
	    typename T::coordinate_type const& coord,
	    T const& config,
	    size_t backend_index,
	    std::index_sequence<SupportedBackendIndex...>);

	template <class T, size_t... SupportedBackendIndex>
	PlaybackProgram::ContainerTicket<T> read_table_generator(
	    typename T::coordinate_type const& coord,
	    size_t backend_index,
	    std::index_sequence<SupportedBackendIndex...>) SYMBOL_VISIBLE;


	std::unique_ptr<fisch::vx::PlaybackProgramBuilder> m_builder_impl;
}; // PlaybackProgramBuilder

} // namespace vx
} // namespace haldls
