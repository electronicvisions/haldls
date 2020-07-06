#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

#include "halco/hicann-dls/vx/barrier.h"
#include "haldls/vx/barrier.h"
#include "haldls/vx/common.h"
#include "haldls/vx/container.h"
#include "hate/visibility.h"
#include "lola/vx/container.h"
#include "stadls/vx/genpybind.h"
#include "stadls/vx/playback_program.h"
// needed for python wrapping
#include "fisch/vx/playback_program_builder.h"
#include "hxcomm/vx/target.h"

namespace fisch::vx {
class PlaybackProgramBuilder;
} // namespace fisch::vx

namespace stadls::vx GENPYBIND_TAG_STADLS_VX {

namespace detail {

template <typename>
class PlaybackProgramBuilderAdapterImpl;

template <typename>
class PlaybackProgramBuilderAdapter;

template <typename T>
std::ostream& operator<<(std::ostream& os, PlaybackProgramBuilderAdapter<T> const& builder)
    SYMBOL_VISIBLE;

/**
 * Sequential PlaybackProgram builder.
 */
template <typename BuilderStorage>
class SYMBOL_VISIBLE PlaybackProgramBuilderAdapter
{
public:
	/** Construct builder. */
	PlaybackProgramBuilderAdapter();

	PlaybackProgramBuilderAdapter(PlaybackProgramBuilderAdapter&& other);
	PlaybackProgramBuilderAdapter& operator=(PlaybackProgramBuilderAdapter&& other);
	PlaybackProgramBuilderAdapter(PlaybackProgramBuilderAdapter const&) = delete;
	~PlaybackProgramBuilderAdapter();

	/**
	 * Add instruction to block execution until specified timer has reached specified value.
	 * @deprecated In favor of block_until
	 * @param coord Timer coordinate for which to wait
	 * @param time Timer value until which to block execution
	 */
	void wait_until(
	    typename haldls::vx::Timer::coordinate_type const& coord, haldls::vx::Timer::Value time);

	/**
	 * Add instruction to block execution until specified timer has reached specified value.
	 * @param coord Timer coordinate for which to block
	 * @param time Timer value until which to block execution
	 */
	void block_until(
	    typename haldls::vx::Timer::coordinate_type const& coord, haldls::vx::Timer::Value time);

	/**
	 * Add instruction to block execution until specified barrier is completed.
	 * @param coord Barrierhronisation coordinate for which to block
	 * @param sync Barrierhronisation value for to block execution
	 */
	void block_until(halco::hicann_dls::vx::BarrierOnFPGA const& coord, haldls::vx::Barrier sync);

#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	/**                                                                                            \
	 * Add instructions to write given container to given location.                                \
	 * @param coord Coordinate value selecting location                                            \
	 * @param config Container configuration data                                                  \
	 * @param backend Backend selection                                                            \
	 */                                                                                            \
	void write(                                                                                    \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    haldls::vx::Backend backend);                                                              \
                                                                                                   \
	/**                                                                                            \
	 * Add instructions to write given container to given location.                                \
	 * The container's default backend is used.                                                    \
	 * @param coord Coordinate value selecting location                                            \
	 * @param config Container configuration data                                                  \
	 * @note This function without backend parameter is needed due to python wrapping not being    \
	 * able to handle templated default arguments.                                                 \
	 */                                                                                            \
	void write(typename Type::coordinate_type const& coord, Type const& config);                   \
                                                                                                   \
	/**                                                                                            \
	 * Add instructions to write given container to given location.                                \
	 * @param coord Coordinate value selecting location                                            \
	 * @param config Container configuration data                                                  \
	 * @param config_reference Reference configuration for differential write                      \
	 * @param backend Backend selection                                                            \
	 */                                                                                            \
	void write(                                                                                    \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference, haldls::vx::Backend backend);                                \
                                                                                                   \
	/**                                                                                            \
	 * Add instructions to write given container to given location.                                \
	 * The container's default backend is used.                                                    \
	 * @param coord Coordinate value selecting location                                            \
	 * @param config Container configuration data                                                  \
	 * @param config_reference Reference configuration for differential write                      \
	 * @note This function without backend parameter is needed due to python wrapping not being    \
	 * able to handle templated default arguments.                                                 \
	 */                                                                                            \
	void write(                                                                                    \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference);                                                             \
                                                                                                   \
	/**                                                                                            \
	 * Add instructions to read container data from given location.                                \
	 * @param coord Coordinate value selecting location                                            \
	 * @param backend Backend selection                                                            \
	 */                                                                                            \
	PlaybackProgram::ContainerTicket<Type> read(                                                   \
	    typename Type::coordinate_type const& coord, haldls::vx::Backend backend);                 \
                                                                                                   \
	/**                                                                                            \
	 * Add instructions to read container data from given location.                                \
	 * The container's default backend is used.                                                    \
	 * @param coord Coordinate value selecting location                                            \
	 * @note This function without backend parameter is needed due to python wrapping not being    \
	 * able to handle templated default arguments.                                                 \
	 */                                                                                            \
	PlaybackProgram::ContainerTicket<Type> read(typename Type::coordinate_type const& coord);
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/container.def"

	/**
	 * Merge other PlaybackProgramBuilderAdapter to the end of this builder instance.
	 * The moved-from builder is emptied during the process.
	 * @param other Builder to move to this instance at the back
	 */
	void merge_back(PlaybackProgramBuilderAdapter& other);

	/**
	 * Merge other PlaybackProgramBuilderAdapter to the end of this builder instance.
	 * The moved-from builder is emptied during the process.
	 * @param other Builder to move to this instance at the back
	 */
	void merge_back(PlaybackProgramBuilderAdapter&& other) GENPYBIND(hidden);

	/**
	 * Merge BuilderStorage to the end of this builder instance.
	 * The moved-from storage is emptied during the process.
	 * @param other BuilderStorage to move to this instance at the back
	 */
	void merge_back(BuilderStorage& other);

	/**
	 * Merge BuilderStorage to the end of this builder instance.
	 * The moved-from storage is emptied during the process.
	 * @param other BuilderStorage to move to this instance at the back
	 */
	void merge_back(BuilderStorage&& other) GENPYBIND(hidden);

	/**
	 * Copy other PlaybackProgramBuilderAdapter to the end of this builder instance.
	 * The copied-from builder is untouched during the process.
	 * @throws std::runtime_error On other builder not being write only
	 * @param other Builder to copy to this instance at the back
	 */
	void copy_back(PlaybackProgramBuilderAdapter const& other);

	/**
	 * Copy BuilderStorage to the end of this builder instance.
	 * The copied-from storage is untouched during the process.
	 * @throws std::runtime_error On other builder not being write only
	 * @param other BuilderStorage to copy to this instance at the back
	 */
	void copy_back(BuilderStorage const& other);

	/**
	 * Close PlaybackProgram build process and return executable program.
	 * @return Executable PlaybackProgram
	 */
	PlaybackProgram done();

	template <typename T>
	friend std::ostream& operator<<(
	    std::ostream& os, PlaybackProgramBuilderAdapter<T> const& builder);

	GENPYBIND_MANUAL({
		parent.def("__repr__", [](GENPYBIND_PARENT_TYPE const& p) {
			std::stringstream ss;
			ss << "PlaybackProgramBuilder(to FPGA size: " << p.size_to_fpga()
			   << ", is write only: " << p.is_write_only() << ")";
			return ss.str();
		});
		parent.def("__str__", [](GENPYBIND_PARENT_TYPE const& p) {
			std::stringstream ss;
			ss << p;
			return ss.str();
		});
	})

	/**
	 * Get whether builder is empty, i.e. no instructions are embodied.
	 * @return Boolean value
	 */
	bool empty() const;

	/**
	 * Get number of UT messages to FPGA.
	 * @return Size
	 */
	size_t size_to_fpga() const;

	/**
	 * Get whether builder only stores write instructions.
	 * @return Boolean value
	 */
	bool is_write_only() const;

private:
	using Impl = detail::PlaybackProgramBuilderAdapterImpl<BuilderStorage>;
	friend Impl;

	std::unique_ptr<BuilderStorage> m_builder_impl;

	std::unordered_set<hxcomm::vx::Target> m_unsupported_targets;
};

extern template SYMBOL_VISIBLE std::ostream& operator<<(
    std::ostream& os,
    PlaybackProgramBuilderAdapter<fisch::vx::PlaybackProgramBuilder> const& builder);

} // namespace detail

typedef detail::PlaybackProgramBuilderAdapter<fisch::vx::PlaybackProgramBuilder>
    PlaybackProgramBuilder GENPYBIND(opaque);

} // namespace stadls::vx

extern template class SYMBOL_VISIBLE
    stadls::vx::detail::PlaybackProgramBuilderAdapter<fisch::vx::PlaybackProgramBuilder>;
