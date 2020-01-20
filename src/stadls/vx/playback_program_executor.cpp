#include "stadls/vx/playback_program_executor.h"

#include <memory>
#include <variant>
#include "fisch/vx/playback_executor.h"
#include "hxcomm/common/fpga_ip_list.h"
#include "stadls/vx/playback_program.h"

namespace stadls::vx {

struct PlaybackProgramExecutor::Impl
{
	Impl() : m_fisch_executor() {}

	typedef std::
	    variant<fisch::vx::PlaybackProgramARQExecutor, fisch::vx::PlaybackProgramSimExecutor>
	        executor_variant_type;

	std::unique_ptr<executor_variant_type> m_fisch_executor;
};

PlaybackProgramExecutor::PlaybackProgramExecutor() : m_impl(std::make_unique<Impl>()) {}

PlaybackProgramExecutor::PlaybackProgramExecutor(PlaybackProgramExecutor&& other) :
    m_impl(std::move(other.m_impl))
{}

void PlaybackProgramExecutor::connect_hardware(ip_t const ip)
{
	if (!m_impl) {
		throw std::logic_error("Unexpected access to moved-from object.");
	}

	if (m_impl->m_fisch_executor) {
		throw std::logic_error("Trying to connect an already connected executor to hardware.");
	}
	m_impl->m_fisch_executor = std::make_unique<Impl::executor_variant_type>(
	    std::in_place_type_t<fisch::vx::PlaybackProgramARQExecutor>(), ip);
}

void PlaybackProgramExecutor::connect_hardware()
{
	connect_hardware(hxcomm::get_fpga_ip());
}

void PlaybackProgramExecutor::connect_simulator(ip_t const ip, port_t const port)
{
	if (!m_impl) {
		throw std::logic_error("Unexpected access to moved-from object.");
	}

	if (m_impl->m_fisch_executor) {
		throw std::logic_error("Trying to connect an already connected executor to simulator.");
	}
	m_impl->m_fisch_executor = std::make_unique<Impl::executor_variant_type>(
	    std::in_place_type_t<fisch::vx::PlaybackProgramSimExecutor>(), ip, port);
}

void PlaybackProgramExecutor::connect_simulator()
{
	char const* env_sim_port = std::getenv("FLANGE_SIMULATION_RCF_PORT");
	char const* env_sim_host = std::getenv("FLANGE_SIMULATION_RCF_HOST");

	static std::string default_host = "127.0.0.1";
	if (env_sim_host == nullptr) {
		env_sim_host = default_host.c_str();
	}

	if (env_sim_port != nullptr) {
		connect_simulator(env_sim_host, static_cast<uint16_t>(atoi(env_sim_port)));
	} else {
		throw std::runtime_error("No executor backend found to connect to.");
	}
}

void PlaybackProgramExecutor::connect()
{
	if (!m_impl) {
		throw std::logic_error("Unexpected access to moved-from object.");
	}

	auto fpga_ip_list = hxcomm::get_fpga_ip_list();
	char const* env_sim_port = std::getenv("FLANGE_SIMULATION_RCF_PORT");
	char const* env_sim_host = std::getenv("FLANGE_SIMULATION_RCF_HOST");

	static std::string default_host = "127.0.0.1";
	if (env_sim_host == nullptr) {
		env_sim_host = default_host.c_str();
	}

	if (fpga_ip_list.size() == 1) {
		connect_hardware(fpga_ip_list.front());
	} else if (fpga_ip_list.size() > 1) {
		throw std::runtime_error("Found more than one FPGA IP in environment to connect to.");
	} else if (env_sim_port != nullptr) {
		connect_simulator(env_sim_host, static_cast<uint16_t>(atoi(env_sim_port)));
	} else {
		throw std::runtime_error("No executor backend found to connect to.");
	}
}

void PlaybackProgramExecutor::disconnect()
{
	if (!m_impl) {
		throw std::logic_error("Unexpected access to moved-from object.");
	}

	if (!m_impl->m_fisch_executor) {
		throw std::logic_error("Trying to disconnect an executor without connection to a backend.");
	}
	m_impl->m_fisch_executor.reset(nullptr);
}

PlaybackProgramExecutor::~PlaybackProgramExecutor() = default;

void PlaybackProgramExecutor::run(PlaybackProgram& program)
{
	if (program.get_executable_restriction()) {
		if (((*(program.get_executable_restriction()) == ExecutorBackend::simulation) &&
		     std::holds_alternative<fisch::vx::PlaybackProgramARQExecutor>(
		         *(m_impl->m_fisch_executor))) ||
		    ((*(program.get_executable_restriction()) == ExecutorBackend::hardware) &&
		     std::holds_alternative<fisch::vx::PlaybackProgramSimExecutor>(
		         *(m_impl->m_fisch_executor)))) {
			throw std::runtime_error(
			    "Trying to execute program with non-matching executable restriction.");
		}
	}
	run(program.m_program_impl);
}

void PlaybackProgramExecutor::run(std::shared_ptr<fisch::vx::PlaybackProgram> const& program)
{
	if (!m_impl) {
		throw std::logic_error("Unexpected access to moved-from object.");
	}

	if (!m_impl->m_fisch_executor) {
		throw std::logic_error(
		    "Trying to call run on an executor without open connection to backend.");
	}

	std::visit(
	    [&program](auto& fisch_executor) { fisch_executor.run(program); },
	    *(m_impl->m_fisch_executor));
}

} // namespace stadls::vx
