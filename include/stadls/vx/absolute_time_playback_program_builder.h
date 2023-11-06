#pragma once

#include "haldls/vx/container.h"
#include "haldls/vx/timer.h"
#include "hate/visibility.h"
#include "stadls/vx/absolute_time_playback_program_container_ticket.h"
#include "stadls/vx/genpybind.h"
#include "stadls/vx/v3/playback_program_builder.h"

#include <iosfwd>
#include <memory>
#include <vector>


namespace stadls::vx {
template <typename PPBType>
class AbsoluteTimePlaybackProgramBuilder;

template <typename BuilderType>
std::ostream& operator<<(
    std::ostream& os,
    AbsoluteTimePlaybackProgramBuilder<BuilderType> const& builder) SYMBOL_VISIBLE;


template <typename PPBType>
class SYMBOL_VISIBLE AbsoluteTimePlaybackProgramBuilder
{
private:
	struct CommandData
	{
		haldls::vx::Timer::Value time;
		std::unique_ptr<haldls::vx::Container::Coordinate> coord;
		std::unique_ptr<haldls::vx::Container> write_config = nullptr;
		std::shared_ptr<AbsoluteTimePlaybackProgramContainerTicketStorage> read_ticket_storage =
		    nullptr;

		CommandData(
		    haldls::vx::Timer::Value time,
		    haldls::vx::Container::Coordinate const& coord,
		    haldls::vx::Container const& write_config);
		CommandData(
		    haldls::vx::Timer::Value time,
		    haldls::vx::Container::Coordinate const& coord,
		    std::shared_ptr<AbsoluteTimePlaybackProgramContainerTicketStorage> const&
		        read_ticket_storage);
		bool operator<(CommandData const& other) const;
		CommandData(CommandData const& other);
		CommandData& operator=(CommandData const& other);
		CommandData& operator=(CommandData&& other);
		CommandData(CommandData&& other);
	};
	std::vector<CommandData> m_commands;
	bool m_is_write_only = true;

public:
	AbsoluteTimePlaybackProgramBuilder() SYMBOL_VISIBLE;
	~AbsoluteTimePlaybackProgramBuilder() SYMBOL_VISIBLE;


	AbsoluteTimePlaybackProgramBuilder(AbsoluteTimePlaybackProgramBuilder<PPBType>&& other)
	    SYMBOL_VISIBLE;
	AbsoluteTimePlaybackProgramBuilder<PPBType>& operator=(
	    AbsoluteTimePlaybackProgramBuilder<PPBType>&& other) SYMBOL_VISIBLE;
	AbsoluteTimePlaybackProgramBuilder(AbsoluteTimePlaybackProgramBuilder<PPBType> const&) = delete;

	/**
	 * Add command to change given container value at given location
	 * @param execTime Time stamp for FPGA when to execute command
	 * @param coord Coordinate value selecting location
	 * @param config Container configuration data
	 */
	void write(
	    haldls::vx::Timer::Value execTime,
	    haldls::vx::Container::Coordinate const& coord,
	    haldls::vx::Container const& config) SYMBOL_VISIBLE;

	/**
	 * Add command to read container data from given location
	 * @param execTime Time stamp for FPGA when to execute command
	 * @param coord Coordinate value selecting location
	 * @return Ticket for accessing measured data after experiment run
	 */
	AbsoluteTimePlaybackProgramContainerTicket read(
	    haldls::vx::Timer::Value execTime,
	    haldls::vx::Container::Coordinate const& coord) SYMBOL_VISIBLE;

	/**
	 * Merge other absolute_time_playback_program_builder into caller and empty command vector of
	 * other
	 * @param other Absolute_time_playback_program_builder to be merged into caller
	 */
	void merge(AbsoluteTimePlaybackProgramBuilder<PPBType>& other) SYMBOL_VISIBLE;

	/**
	 * Copy command vector from other absolute_time_playback_program_builder and merge it into
	 * command vector from caller
	 * @param other Absolute_time_playback_program_builder to be copied from
	 */
	void copy(AbsoluteTimePlaybackProgramBuilder<PPBType>& other) SYMBOL_VISIBLE;

	bool empty() const SYMBOL_VISIBLE;

	bool is_write_only() const SYMBOL_VISIBLE;

	/**
	 * Add a time offset to all commands
	 * @param offset Magnitude of time shift in FGPA clock cycles
	 */
	void operator+=(haldls::vx::Timer::Value const offset) SYMBOL_VISIBLE;

	/**
	 * Copy caller and add time offset to all commands of this copy
	 * @param offset Magnitude of time shift in FGPA clock cycles
	 * @return Absolute_time_playback_program_builder with time offset
	 */
	AbsoluteTimePlaybackProgramBuilder<PPBType> operator+(haldls::vx::Timer::Value const offset)
	    SYMBOL_VISIBLE;

	/**
	 * Print all commands in initial order
	 */
	template <typename BuilderType>
	GENPYBIND(stringstream)
	friend std::ostream& operator<<(
	    std::ostream& os,
	    AbsoluteTimePlaybackProgramBuilder<BuilderType> const& builder) SYMBOL_VISIBLE;

	/**
	 * Construct a playback_program_builder with according command queue
	 * @return Playback_program_builder
	 */
	PPBType done() SYMBOL_VISIBLE;
};
} // namespace stadls::vx
