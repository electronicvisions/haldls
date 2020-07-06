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

namespace stadls {
namespace vx GENPYBIND_TAG_STADLS_VX {

class PlaybackProgram;

/**
 * Sequential PlaybackProgram builder.
 */
class GENPYBIND(visible) PlaybackProgramBuilder
{
public:
	/** Construct builder. */
	PlaybackProgramBuilder() SYMBOL_VISIBLE;

	PlaybackProgramBuilder(PlaybackProgramBuilder&& other) SYMBOL_VISIBLE;
	PlaybackProgramBuilder& operator=(PlaybackProgramBuilder&& other) SYMBOL_VISIBLE;
	PlaybackProgramBuilder(PlaybackProgramBuilder const&) = delete;
	~PlaybackProgramBuilder() SYMBOL_VISIBLE;

	/**
	 * Add instruction to block execution until specified timer has reached specified value.
	 * @deprecated In favor of block_until
	 * @param coord Timer coordinate for which to wait
	 * @param time Timer value until which to block execution
	 */
	void wait_until(
	    typename haldls::vx::Timer::coordinate_type const& coord,
	    haldls::vx::Timer::Value time) SYMBOL_VISIBLE;

	/**
	 * Add instruction to block execution until specified timer has reached specified value.
	 * @param coord Timer coordinate for which to block
	 * @param time Timer value until which to block execution
	 */
	void block_until(
	    typename haldls::vx::Timer::coordinate_type const& coord,
	    haldls::vx::Timer::Value time) SYMBOL_VISIBLE;

	/**
	 * Add instruction to block execution until specified barrier is completed.
	 * @param coord Barrierhronisation coordinate for which to block
	 * @param sync Barrierhronisation value for to block execution
	 */
	void block_until(halco::hicann_dls::vx::BarrierOnFPGA const& coord, haldls::vx::Barrier sync)
	    SYMBOL_VISIBLE;

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
	void write(typename Type::coordinate_type const& coord, Type const& config) SYMBOL_VISIBLE;    \
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
	    Type const& config_reference, haldls::vx::Backend backend) SYMBOL_VISIBLE;                 \
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
	    Type const& config_reference) SYMBOL_VISIBLE;                                              \
                                                                                                   \
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
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/container.def"

	/**
	 * Merge other PlaybackProgramBuilder to the end of this builder instance.
	 * The moved-from builder is emptied during the process.
	 * @param other Builder to move to this instance at the back
	 */
	void merge_back(PlaybackProgramBuilder& other) SYMBOL_VISIBLE;

	/**
	 * Merge other PlaybackProgramBuilder to the end of this builder instance.
	 * The moved-from builder is emptied during the process.
	 * @param other Builder to move to this instance at the back
	 */
	void merge_back(PlaybackProgramBuilder&& other) GENPYBIND(hidden) SYMBOL_VISIBLE;

	/**
	 * Merge fisch PlaybackProgramBuilder to the end of this builder instance.
	 * The moved-from builder is emptied during the process.
	 * @param other Builder to move to this instance at the back
	 */
	void merge_back(fisch::vx::PlaybackProgramBuilder& other) SYMBOL_VISIBLE;

	/**
	 * Merge fisch PlaybackProgramBuilder to the end of this builder instance.
	 * The moved-from builder is emptied during the process.
	 * @param other Builder to move to this instance at the back
	 */
	void merge_back(fisch::vx::PlaybackProgramBuilder&& other) GENPYBIND(hidden) SYMBOL_VISIBLE;

	/**
	 * Copy other PlaybackProgramBuilder to the end of this builder instance.
	 * The copied-from builder is untouched during the process.
	 * @throws std::runtime_error On other builder not being write only
	 * @param other Builder to copy to this instance at the back
	 */
	void copy_back(PlaybackProgramBuilder const& other) SYMBOL_VISIBLE;

	/**
	 * Copy fisch PlaybackProgramBuilder to the end of this builder instance.
	 * The copied-from builder is untouched during the process.
	 * @throws std::runtime_error On other builder not being write only
	 * @param other Builder to copy to this instance at the back
	 */
	void copy_back(fisch::vx::PlaybackProgramBuilder const& other) SYMBOL_VISIBLE;

	/**
	 * Close PlaybackProgram build process and return executable program.
	 * @return Executable PlaybackProgram
	 */
	PlaybackProgram done() SYMBOL_VISIBLE;

	friend std::ostream& operator<<(std::ostream& os, PlaybackProgramBuilder const& builder)
	    SYMBOL_VISIBLE;

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
	bool empty() const SYMBOL_VISIBLE;

	/**
	 * Get number of UT messages to FPGA.
	 * @return Size
	 */
	size_t size_to_fpga() const SYMBOL_VISIBLE;

	/**
	 * Get whether builder only stores write instructions.
	 * @return Boolean value
	 */
	bool is_write_only() const SYMBOL_VISIBLE;

private:
	friend class PlaybackProgramBuilderImpl;

	std::unique_ptr<fisch::vx::PlaybackProgramBuilder> m_builder_impl;

	std::unordered_set<hxcomm::vx::Target> m_unsupported_targets;
};

} // namespace vx
} // namespace stadls
