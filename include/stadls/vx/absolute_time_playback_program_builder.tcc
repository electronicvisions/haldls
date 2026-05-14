#include "stadls/vx/absolute_time_playback_program_builder.h"
#include "halco/hicann-dls/vx/timing.h"

#include "hate/variant.h"
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
bool AbsoluteTimePlaybackProgramBuilder<PPBType>::CommandData::operator<(
    CommandData const& other) const
{
	return time < other.time;
}

template <typename PPBType>
void AbsoluteTimePlaybackProgramBuilder<PPBType>::write(
    const haldls::vx::Timer::Value execTime,
    haldls::vx::Container::Coordinate const& coord,
    haldls::vx::Container const& config)
{
	if (typeid(coord) == typeid(halco::hicann_dls::vx::TimerOnDLS)) {
		throw std::runtime_error("Editing the FPGA-Timer is forbidden when using the "
		                         "AbsoluteTimePlaybackProgramBuilder");
	}
	m_commands.emplace_back(execTime, WriteCommandAction{coord.clone(), config.clone_container()});
}

template <typename PPBType>
void AbsoluteTimePlaybackProgramBuilder<PPBType>::write(
    const haldls::vx::Timer::Value execTime,
    haldls::vx::Container::Coordinate const& coord,
    haldls::vx::Container const& config, haldls::vx::Container const& config_reference)
{
	if (typeid(coord) == typeid(halco::hicann_dls::vx::TimerOnDLS)) {
		throw std::runtime_error("Editing the FPGA-Timer is forbidden when using the "
		                         "AbsoluteTimePlaybackProgramBuilder");
	}
	m_commands.emplace_back(
	    execTime, WriteDifferentialCommandAction{
	                  coord.clone(), config.clone_container(), config_reference.clone_container()});
}

template <typename PPBType>
AbsoluteTimePlaybackProgramContainerTicket AbsoluteTimePlaybackProgramBuilder<PPBType>::read(haldls::vx::Timer::Value execTime, haldls::vx::Container::Coordinate const& coord){
	m_is_write_only = false;
	std::shared_ptr<AbsoluteTimePlaybackProgramContainerTicketStorage> ticket_storage = std::make_shared<AbsoluteTimePlaybackProgramContainerTicketStorage>();
	m_commands.emplace_back(execTime, ReadCommandAction{coord.clone(), ticket_storage});
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
	for (auto const& command : other.m_commands) {
		m_commands.emplace_back(
		    command.time,
		    std::visit(
		        hate::overloaded{
		            [](WriteCommandAction const& action) -> CommandAction {
			            return WriteCommandAction(
			                action.coord->clone(), action.write_config->clone_container());
		            },
		            [](WriteDifferentialCommandAction const& action) -> CommandAction {
			            return WriteDifferentialCommandAction(
			                action.coord->clone(), action.write_config->clone_container(),
			                action.write_config_reference->clone_container());
		            },
		            [](ReadCommandAction const& action) -> CommandAction {
			            return ReadCommandAction(action.coord->clone(), action.read_ticket_storage);
		            }},
		        command.action));
	}
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
	if(offset != haldls::vx::Timer::Value(0)){
		for (auto& command : m_commands){
			command.time += offset;
		}
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
AbsoluteTimePlaybackProgramBuilder<PPBType>& AbsoluteTimePlaybackProgramBuilder<PPBType>::operator*=(float const factor){
	if(factor != 1){
		for (auto& command : m_commands){
			command.time = haldls::vx::Timer::Value(static_cast<uint32_t>(round(command.time.value() * factor)));
		}
	}
	return *this;
}

template <typename PPBType>
AbsoluteTimePlaybackProgramBuilder<PPBType> AbsoluteTimePlaybackProgramBuilder<PPBType>::operator*(float const factor){
	if (!is_write_only()) {
		throw std::runtime_error("'*'-operation is invalid for non-write-only AbsoluteTimePlaybackProgramBuilder (cannot be copied)");
	}
	AbsoluteTimePlaybackProgramBuilder<PPBType> scaled_builder;
	scaled_builder.copy(*this);
	scaled_builder *= factor;
	return scaled_builder;
}

template <typename PPBType>
std::ostream& operator<<(
    std::ostream& os, AbsoluteTimePlaybackProgramBuilder<PPBType> const& builder)
{
	os << "AboluteTimePlaybackProgramBuilder command queue: (" << builder.m_commands.size()
	   << " elements)\n";
	for (auto const& command : builder.m_commands) {
		std::visit(
		    hate::overloaded{
		        [&command, &os](
		            typename AbsoluteTimePlaybackProgramBuilder<PPBType>::WriteCommandAction const&
		                action) {
			        os << "write | t: " << command.time << ", coord: " << *action.coord
			           << ", config: " << *action.write_config << "\n";
		        },
		        [&command, &os](typename AbsoluteTimePlaybackProgramBuilder<
		                        PPBType>::WriteDifferentialCommandAction const& action) {
			        os << "write | t: " << command.time << ", coord: " << *action.coord
			           << ", config: " << *action.write_config << "\n";
		        },
		        [&command,
		         &os](typename AbsoluteTimePlaybackProgramBuilder<PPBType>::ReadCommandAction const&
		                  action) {
			        os << "read  | t: " << command.time << ", coord: " << *action.coord << "\n";
		        }},
		    command.action);
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
	for (auto& command : m_commands) {
		if (command.time > current_time) {
			current_time = command.time;
			builder.block_until(
			    halco::hicann_dls::vx::TimerOnDLS(),
			    haldls::vx::Timer::Value(current_time.value() - 1));
		} else if (command.time + max_delay < current_time) {
			LOG4CXX_WARN(
			    logger, "To be issued event at (" << command.time << ") delayed by more than "
			                                      << max_delay << ".");
		}
		std::visit(
		    hate::overloaded{
		        [&builder](WriteCommandAction const& action) {
			        builder.write(*action.coord, *action.write_config);
		        },
		        [&builder](WriteDifferentialCommandAction const& action) {
			        builder.write(
			            *action.coord, *action.write_config, *action.write_config_reference);
		        },
		        [&builder](ReadCommandAction& action) {
			        assert(action.read_ticket_storage);
			        action.read_ticket_storage->container_ticket = builder.read(*action.coord);
		        }},
		    command.action);
		current_time += haldls::vx::Timer::Value(1);
	}
	m_commands.clear();
	return builder;
}

} // namespace stadls::vx
