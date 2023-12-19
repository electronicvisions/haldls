#include "stadls/vx/absolute_time_playback_program_builder.h"
#include "halco/hicann-dls/vx/timing.h"

#include <algorithm>
#include <iterator>
#include <ostream>
#include <log4cxx/logger.h>

namespace stadls::vx {

template <typename PPBType>
AbsoluteTimePlaybackProgramBuilder<PPBType>::AbsoluteTimePlaybackProgramBuilder()
{}

template <typename PPBType>
AbsoluteTimePlaybackProgramBuilder<PPBType>::~AbsoluteTimePlaybackProgramBuilder()
{}

template <typename PPBType>
AbsoluteTimePlaybackProgramBuilder<PPBType>::AbsoluteTimePlaybackProgramBuilder(
    AbsoluteTimePlaybackProgramBuilder<PPBType>&& other)
{
	m_commands = std::move(other.m_commands);
	m_is_write_only = other.m_is_write_only;
}

template <typename PPBType>
AbsoluteTimePlaybackProgramBuilder<PPBType>& AbsoluteTimePlaybackProgramBuilder<PPBType>::operator=(
    AbsoluteTimePlaybackProgramBuilder<PPBType>&& other)
{
	if (this != &other) {
		m_commands = std::move(other.m_commands);
		m_is_write_only = other.m_is_write_only;
	}
	return *this;
}

template <typename PPBType>
AbsoluteTimePlaybackProgramBuilder<PPBType>::CommandData::CommandData(
    haldls::vx::Timer::Value time, haldls::vx::Container::Coordinate const& coord,
	    haldls::vx::Container const& write_config) : time(time), coord(coord.clone()), write_config(write_config.clone_container())
{

}

template <typename PPBType>
AbsoluteTimePlaybackProgramBuilder<PPBType>::CommandData::CommandData(
    haldls::vx::Timer::Value time, haldls::vx::Container::Coordinate const& coord,
	    haldls::vx::Container const& write_config, haldls::vx::Container const& write_config_reference) : time(time), coord(coord.clone()), write_config(write_config.clone_container()), write_config_reference(write_config_reference.clone_container())
{

}

template <typename PPBType>
AbsoluteTimePlaybackProgramBuilder<PPBType>::CommandData::CommandData(
    haldls::vx::Timer::Value time, haldls::vx::Container::Coordinate const& coord,
	std::shared_ptr<AbsoluteTimePlaybackProgramContainerTicketStorage> const& read_ticket_storage) : time(time), coord(coord.clone()), read_ticket_storage(read_ticket_storage)
{

}

template <typename PPBType>
bool AbsoluteTimePlaybackProgramBuilder<PPBType>::CommandData::operator<(
    CommandData const& other) const
{
	return time < other.time;
}

template <typename PPBType>
AbsoluteTimePlaybackProgramBuilder<PPBType>::CommandData& AbsoluteTimePlaybackProgramBuilder<PPBType>::CommandData::operator=(
    CommandData const& other)
{
	if (this != &other) {
		time = other.time;
		coord = other.coord ? other.coord->clone() : nullptr;
		write_config = other.write_config ? other.write_config->clone_container() : nullptr;
		write_config_reference = other.write_config_reference ? other.write_config_reference->clone_container() : nullptr;
		read_ticket_storage = other.read_ticket_storage;
	}
	return *this;
}

template <typename PPBType>
AbsoluteTimePlaybackProgramBuilder<PPBType>::CommandData& AbsoluteTimePlaybackProgramBuilder<PPBType>::CommandData::operator=(
    CommandData&& other)
{
	if (this != &other) {
		time = other.time;
		coord = std::move(other.coord);
		write_config = std::move(other.write_config);
		write_config_reference = std::move(other.write_config_reference);
		read_ticket_storage = std::move(other.read_ticket_storage);
	}
	return *this;
}

template <typename PPBType>
AbsoluteTimePlaybackProgramBuilder<PPBType>::CommandData::CommandData(
    CommandData const& other) : time(other.time), coord(other.coord ? other.coord->clone() : nullptr), write_config(other.write_config ? other.write_config->clone_container() : nullptr), write_config_reference(other.write_config_reference ? other.write_config_reference->clone_container() : nullptr), read_ticket_storage(other.read_ticket_storage)
{

}

template <typename PPBType>
AbsoluteTimePlaybackProgramBuilder<PPBType>::CommandData::CommandData(
    CommandData&& other) : time(other.time), coord(std::move(other.coord)), write_config(std::move(other.write_config)), write_config_reference(std::move(other.write_config_reference)), read_ticket_storage(std::move(other.read_ticket_storage))
{

}

template <typename PPBType>
void AbsoluteTimePlaybackProgramBuilder<PPBType>::write(
    const haldls::vx::Timer::Value execTime,
    haldls::vx::Container::Coordinate const& coord,
    haldls::vx::Container const& config)
{
	if (dynamic_cast<halco::hicann_dls::vx::TimerOnDLS const*>(&coord) != nullptr) {
		throw std::runtime_error("Editing the FPGA-Timer is forbidden when using the AbsoluteTimePlaybackProgramBuilder");
	}
	m_commands.push_back(
	    CommandData(execTime, coord, config));
}

template <typename PPBType>
void AbsoluteTimePlaybackProgramBuilder<PPBType>::write(
    const haldls::vx::Timer::Value execTime,
    haldls::vx::Container::Coordinate const& coord,
    haldls::vx::Container const& config, haldls::vx::Container const& config_reference)
{
	if (dynamic_cast<halco::hicann_dls::vx::TimerOnDLS const*>(&coord) != nullptr) {
		throw std::runtime_error("Editing the FPGA-Timer is forbidden when using the AbsoluteTimePlaybackProgramBuilder");
	}
	m_commands.push_back(
	    CommandData(execTime, coord, config, config_reference));
}

template <typename PPBType>
AbsoluteTimePlaybackProgramContainerTicket AbsoluteTimePlaybackProgramBuilder<PPBType>::read(haldls::vx::Timer::Value execTime, haldls::vx::Container::Coordinate const& coord){
	m_is_write_only = false;
	std::shared_ptr<AbsoluteTimePlaybackProgramContainerTicketStorage> ticket_storage = std::make_shared<AbsoluteTimePlaybackProgramContainerTicketStorage>();
	m_commands.push_back(CommandData(execTime, coord, ticket_storage));
	return AbsoluteTimePlaybackProgramContainerTicket(ticket_storage);
}

template <typename PPBType>
void AbsoluteTimePlaybackProgramBuilder<PPBType>::merge(
    AbsoluteTimePlaybackProgramBuilder<PPBType>& other)
{
	m_commands.insert(
	    m_commands.end(), std::make_move_iterator(other.m_commands.begin()),
	    std::make_move_iterator(other.m_commands.end()));
	std::vector<CommandData>().swap(other.m_commands);
}

template <typename PPBType>
void AbsoluteTimePlaybackProgramBuilder<PPBType>::merge(
    AbsoluteTimePlaybackProgramBuilder<PPBType>&& other)
{
	m_commands.insert(
	    m_commands.end(), std::make_move_iterator(other.m_commands.begin()),
	    std::make_move_iterator(other.m_commands.end()));
	std::vector<CommandData>().swap(other.m_commands);
}

template <typename PPBType>
void AbsoluteTimePlaybackProgramBuilder<PPBType>::copy(
    AbsoluteTimePlaybackProgramBuilder<PPBType> const& other)
{
	if (!other.is_write_only()) {
		throw std::runtime_error("Non-write-only AbsoluteTimePlaybackProgramBuilder cannot be copied");
	}
	m_commands.insert(m_commands.end(), other.m_commands.begin(), other.m_commands.end());
}

template <typename PPBType>
bool AbsoluteTimePlaybackProgramBuilder<PPBType>::empty() const
{
	return m_commands.empty();
}

template <typename PPBType>
bool AbsoluteTimePlaybackProgramBuilder<PPBType>::is_write_only() const
{
	return m_is_write_only;
}

template <typename PPBType>
AbsoluteTimePlaybackProgramBuilder<PPBType>& AbsoluteTimePlaybackProgramBuilder<PPBType>::operator+=(haldls::vx::Timer::Value const offset){
	for (auto& command : m_commands){
		command.time += offset;
	}
	return *this;
}

template <typename PPBType>
AbsoluteTimePlaybackProgramBuilder<PPBType> AbsoluteTimePlaybackProgramBuilder<PPBType>::operator+(haldls::vx::Timer::Value const offset) const {
	if (!is_write_only()) {
		throw std::runtime_error("'+'-operation is invalid for non-write-only AbsoluteTimePlaybackProgramBuilder (cannot be copied)");
	}
	AbsoluteTimePlaybackProgramBuilder<PPBType> offset_builder;
	offset_builder.copy(*this);
	offset_builder += offset;
	return offset_builder;
}

template <typename PPBType>
void AbsoluteTimePlaybackProgramBuilder<PPBType>::operator*=(float const factor){
	for (auto& command : m_commands){
		command.time = haldls::vx::Timer::Value(static_cast<uint32_t>(round(command.time.value() * factor)));
	}
}

template <typename PPBType>
AbsoluteTimePlaybackProgramBuilder<PPBType> AbsoluteTimePlaybackProgramBuilder<PPBType>::operator*(float const factor){
	if (!is_write_only()) {
		throw std::runtime_error("'*'-operation is invalid for non-write-only AbsoluteTimePlaybackProgramBuilder (cannot be copied)");
	}
	AbsoluteTimePlaybackProgramBuilder<PPBType> scaled_builder;
	scaled_builder.m_commands = m_commands;
	for (auto& command : scaled_builder.m_commands){
		command.time = haldls::vx::Timer::Value(static_cast<uint32_t>(round(command.time.value() * factor)));
	}
	return scaled_builder;
}

template <typename PPBType>
std::ostream& operator<<(
    std::ostream& os, AbsoluteTimePlaybackProgramBuilder<PPBType> const& builder)
{
	os << "AboluteTimePlaybackProgramBuilder command queue: (" << builder.m_commands.size()
	   << " elements)\n";
	for (auto const& command : builder.m_commands) {
		if(command.write_config){
			os << "write | t: " << command.time << ", coord: " << *command.coord
			    << ", config: " << *command.write_config << "\n";
		} else {
			os << "read  | t: " << command.time << ", coord: " << *command.coord
			    << "\n";
		}
	}
	return os;
}

template <typename PPBType>
PPBType AbsoluteTimePlaybackProgramBuilder<PPBType>::done()
{
	auto logger = log4cxx::Logger::getLogger("stadls.AbsoluteTimePlaybackProgramBuilder");
	// approx. 10ms in biological time deemed high enough to warn the user something is wrong
	haldls::vx::Timer::Value const max_delay(
	    haldls::vx::Timer::Value::fpga_clock_cycles_per_us * 10);
	PPBType builder;
	std::stable_sort(m_commands.begin(), m_commands.end());
	builder.write(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer());
	haldls::vx::Timer::Value current_time(0);
	for (size_t index = 0; index < m_commands.size(); index++) {
		if (m_commands[index].time > current_time) {
			builder.block_until(
			    halco::hicann_dls::vx::TimerOnDLS(),
			    haldls::vx::Timer::Value(m_commands[index].time - haldls::vx::Timer::Value(1)));
			current_time = m_commands[index].time;
		} else if (m_commands[index].time + max_delay < current_time) {
			LOG4CXX_WARN(
			    logger, "To be issued event at (" << m_commands[index].time
			                                      << ") delayed by more than " << max_delay << ".");
		}
		if(m_commands[index].write_config){
			if(m_commands[index].write_config_reference){
				builder.write(*m_commands[index].coord, *m_commands[index].write_config, *m_commands[index].write_config_reference);
			} else {
				builder.write(*m_commands[index].coord, *m_commands[index].write_config);
			}
		} else {
			assert(m_commands[index].read_ticket_storage);
			m_commands[index].read_ticket_storage->container_ticket = builder.read(*m_commands[index].coord);
		}
		current_time = current_time + haldls::vx::Timer::Value(1);
	}
	m_commands.clear();
	return builder;
}
} // namespace stadls::vx
