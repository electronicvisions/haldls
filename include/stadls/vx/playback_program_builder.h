#pragma once

#include <iosfwd>
#include <memory>
#include <unordered_set>

#include "halco/hicann-dls/vx/barrier.h"
#include "haldls/vx/barrier.h"
#include "haldls/vx/block_until.h"
#include "haldls/vx/common.h"
#include "haldls/vx/container.h"
#include "hate/visibility.h"
#include "hxcomm/vx/target.h"
#include "stadls/vx/container_ticket.h"
#include "stadls/vx/dumper.h"
#include "stadls/vx/genpybind.h"
#include "stadls/vx/playback_program.h"

// clang-format off
#if defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)
#include "haldls/vx/pickle.h"
#endif

#ifdef __GENPYBIND_GENERATED__
#include "fisch/vx/playback_program_builder.h"
#endif
// clang-format on

namespace stadls::vx::detail {

template <typename BuilderStorage, typename DoneType>
struct PlaybackProgramBuilderAdapter;

} // namespace stadls::vx::detail

namespace cereal {

template <typename Archive, typename BuilderStorage, typename DoneType>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    stadls::vx::detail::PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>& value,
    std::uint32_t const version);

} // namespace cereal

namespace fisch::vx {
class PlaybackProgramBuilder;
} // namespace fisch::vx

namespace stadls::vx GENPYBIND_TAG_STADLS_VX {

#if defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)
namespace v3 {
class DumperDone;
using Dumper = class stadls::vx::detail::Dumper<stadls::vx::v3::DumperDone>;
} // namespace v3
#endif

namespace detail {

template <typename, typename>
class PlaybackProgramBuilderAdapterImpl;

template <typename, typename>
class PlaybackProgramBuilderAdapter;

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, PlaybackProgramBuilderAdapter<T, U> const& builder)
    SYMBOL_VISIBLE;

/**
 * Sequential PlaybackProgram builder.
 */
template <typename BuilderStorage, typename DoneType>
class SYMBOL_VISIBLE PlaybackProgramBuilderAdapter
{
public:
	/** Construct builder. */
	PlaybackProgramBuilderAdapter();

	PlaybackProgramBuilderAdapter(PlaybackProgramBuilderAdapter&& other);
	PlaybackProgramBuilderAdapter& operator=(PlaybackProgramBuilderAdapter&& other);
	PlaybackProgramBuilderAdapter(PlaybackProgramBuilderAdapter const&) = delete;
	~PlaybackProgramBuilderAdapter();

	typedef BuilderStorage Builder;

	/**
	 * Add instruction to block execution until specified condition is satisfied.
	 * @param coord Coordinate for which to block
	 * @param condition Condition to block execution for until satisfaction
	 */
	void block_until(
	    haldls::vx::BlockUntil::Coordinate const& coord, haldls::vx::BlockUntil const& condition);

	/**
	 * Add instructions to write given container to given location.
	 * @param coord Coordinate value selecting location
	 * @param config Container configuration data
	 */
	void write(haldls::vx::Container::Coordinate const& coord, haldls::vx::Container const& config);

	/**
	 * Add instructions to write given container to given location.
	 * @param coord Coordinate value selecting location
	 * @param config Container configuration data
	 * @param backend Backend selection
	 */
	void write(
	    haldls::vx::Container::Coordinate const& coord,
	    haldls::vx::Container const& config,
	    haldls::vx::Backend backend);

	/**
	 * Add instructions to write given container to given location.
	 * @param coord Coordinate value selecting location
	 * @param config Container configuration data
	 * @param config_reference Reference configuration for differential write
	 */
	void write(
	    haldls::vx::Container::Coordinate const& coord,
	    haldls::vx::Container const& config,
	    haldls::vx::Container const& config_reference);

	/**
	 * Add instructions to write given container to given location.
	 * @param coord Coordinate value selecting location
	 * @param config Container configuration data
	 * @param config_reference Reference configuration for differential write
	 * @param backend Backend selection
	 */
	void write(
	    haldls::vx::Container::Coordinate const& coord,
	    haldls::vx::Container const& config,
	    haldls::vx::Container const& config_reference,
	    haldls::vx::Backend backend);

	/**
	 * Add instructions to read container data from given location.
	 * @param coord Coordinate value selecting location
	 */
	ContainerTicket read(haldls::vx::Container::Coordinate const& coord);

	/**
	 * Add instructions to read container data from given location.
	 * @param coord Coordinate value selecting location
	 * @param backend Backend selection
	 */
	ContainerTicket read(
	    haldls::vx::Container::Coordinate const& coord, haldls::vx::Backend backend);

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
	DoneType done();

	template <typename T, typename U>
	friend std::ostream& operator<<(
	    std::ostream& os, PlaybackProgramBuilderAdapter<T, U> const& builder);

	GENPYBIND_MANUAL({
		parent.def("__repr__", [](GENPYBIND_PARENT_TYPE const& p) {
			std::stringstream ss;
			typedef hate::type_list<::stadls::vx::v3::Dumper> dumper_types;
			if constexpr (hate::is_in_type_list<
			                  typename std::decay<decltype(p)>::type::Builder,
			                  dumper_types>::value) {
				ss << "PlaybackProgramBuilderDumper()";
			} else {
				ss << "PlaybackProgramBuilder(to FPGA size: " << p.size_to_fpga()
				   << ", is write only: " << std::boolalpha << p.is_write_only() << ")";
			}
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
	 * Get number of expected UT messages from FPGA.
	 * @return Size
	 */
	size_t size_from_fpga() const;

	/**
	 * Get whether builder only stores write instructions.
	 * @return Boolean value
	 */
	bool is_write_only() const;

private:
	using Impl = detail::PlaybackProgramBuilderAdapterImpl<BuilderStorage, DoneType>;
	friend Impl;

	template <class Archive, typename T, typename U>
	friend void ::cereal::serialize(
	    Archive& ar, PlaybackProgramBuilderAdapter<T, U>& value, std::uint32_t const version);

	std::unique_ptr<BuilderStorage> m_builder_impl;

	std::unordered_set<hxcomm::vx::Target> m_unsupported_targets;
};

} // namespace detail

} // namespace stadls::vx
