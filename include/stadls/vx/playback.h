#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "haldls/vx/common.h"
#include "haldls/vx/container.h"
#include "hate/visibility.h"
#include "stadls/vx/genpybind.h"

namespace fisch::vx {
class PlaybackProgram;
class PlaybackProgramBuilder;
} // namespace fisch::vx

namespace stadls {
namespace vx GENPYBIND_TAG_STADLS_VX {

class PlaybackProgramExecutor;
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
		typedef typename haldls::vx::detail::to_ticket_variant<
		    typename haldls::vx::detail::BackendContainerTrait<T>::container_list>::type
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
#endif // __GENPYBIND__

	PlaybackProgram() SYMBOL_VISIBLE;
	PlaybackProgram(std::shared_ptr<fisch::vx::PlaybackProgram> const& other) SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PlaybackProgram const& program)
	    SYMBOL_VISIBLE;

private:
	friend PlaybackProgramBuilder;
	friend PlaybackProgramExecutor;

	std::shared_ptr<fisch::vx::PlaybackProgram> m_program_impl;

	bool m_valid;
}; // PlaybackProgram

namespace detail {

#define LAST_PLAYBACK_CONTAINER(Name, Type) Type
#define PLAYBACK_CONTAINER(Name, Type) LAST_PLAYBACK_CONTAINER(Name, Type),
typedef hate::type_list<
#include "haldls/vx/container.def"
    >
    container_list;

#define LAST_PLAYBACK_CONTAINER(Name, Type) typename Type::coordinate_type
#define PLAYBACK_CONTAINER(Name, Type) LAST_PLAYBACK_CONTAINER(Name, Type),
typedef hate::type_list<
#include "haldls/vx/container.def"
    >
    coordinate_list;

template <typename CoordinateT>
struct coordinate_type_to_container_type
{
	typedef typename hate::index_type_list_by_integer<
	    hate::index_type_list_by_type<CoordinateT, coordinate_list>::value,
	    container_list>::type type;
};

} // namespace detail

class GENPYBIND(visible) PlaybackProgramBuilder
{
public:
	PlaybackProgramBuilder() SYMBOL_VISIBLE;

	void wait_until(
	    typename haldls::vx::Timer::coordinate_type const& coord,
	    haldls::vx::Timer::Value t) SYMBOL_VISIBLE;

#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	/**                                                                                            \
	 * Add instructions to write given container to given location.                                \
	 * @param coord Coordinate value selecting location                                            \
	 * @param config Container configuration data                                                  \
	 * @param backend Backend selection                                                            \
	 */                                                                                            \
	void write(                                                                                    \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    haldls::vx::Backend backend) SYMBOL_VISIBLE;                                               \
                                                                                                   \
	/**                                                                                            \
	 * Add instructions to write given container to given location.                                \
	 * The container's default backend is used.                                                    \
	 * @param coord Coordinate value selecting location                                            \
	 * @param config Container configuration data                                                  \
	 * @note This function without backend parameter is needed due to python wrapping not being    \
	 * able to handle templated default arguments.                                                 \
	 */                                                                                            \
	void write(typename Type::coordinate_type const& coord, Type const& config) SYMBOL_VISIBLE;
#include "haldls/vx/container.def"

#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	/**                                                                                            \
	 * Add instructions to read container data from given location.                                \
	 * @param coord Coordinate value selecting location                                            \
	 * @param backend Backend selection                                                            \
	 */                                                                                            \
	PlaybackProgram::ContainerTicket<Type> read(                                                   \
	    typename Type::coordinate_type const& coord, haldls::vx::Backend backend) SYMBOL_VISIBLE;  \
                                                                                                   \
	/**                                                                                            \
	 * Add instructions to read container data from given location.                                \
	 * The container's default backend is used.                                                    \
	 * @param coord Coordinate value selecting location                                            \
	 * @note This function without backend parameter is needed due to python wrapping not being    \
	 * able to handle templated default arguments.                                                 \
	 */                                                                                            \
	PlaybackProgram::ContainerTicket<Type> read(typename Type::coordinate_type const& coord)       \
	    SYMBOL_VISIBLE;
#include "haldls/vx/container.def"

	PlaybackProgram done() SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PlaybackProgramBuilder const& builder)
	    SYMBOL_VISIBLE;

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
