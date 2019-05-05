#pragma once
#include <memory>

#include "haldls/vx/playback.h"
#include "hate/visibility.h"
#include "stadls/vx/genpybind.h"

namespace stadls::vx GENPYBIND_TAG_STADLS_VX {

/**
 * Executor for playback program.
 */
class GENPYBIND(visible) PlaybackProgramExecutor
{
public:
	typedef std::string ip_t;
	typedef uint16_t port_t;

	/**
	 * Construct executor without connection to backend.
	 */
	PlaybackProgramExecutor() SYMBOL_VISIBLE;

	/** Move constructor. */
	PlaybackProgramExecutor(PlaybackProgramExecutor&& other) SYMBOL_VISIBLE;

	/** Copy constructor not possible with IO connection. */
	PlaybackProgramExecutor(PlaybackProgramExecutor const& other) = delete;

	/**
	 * Connect to hardware.
	 * @param ip IP address of ARQ backend
	 */
	void connect_hardware(ip_t ip) SYMBOL_VISIBLE;

	/**
	 * Connect to simulator.
	 * @param ip IP address of simulator backend
	 * @param port Port of simulator backend
	 */
	void connect_simulator(ip_t ip, port_t port) SYMBOL_VISIBLE;

	/**
	 * Connect to backend by automatically finding possible executor.
	 * If both hardware and simulator executor are available, hardware is favoured.
	 * A simulator executor is found if the environment features FLANGE_SIMULATION_RCF_PORT.
	 * A hardware executor is found if the environment features DLSvx_HARDWARE_AVAILABLE
	 * @throws std::runtime_error On no executor backend found
	 */
	void connect() SYMBOL_VISIBLE;

	/**
	 * Disconnect from executor backend.
	 */
	void disconnect() SYMBOL_VISIBLE;

	/**
	 * Destruct executor closing connection to backend.
	 */
	~PlaybackProgramExecutor() SYMBOL_VISIBLE;

	/**
	 * Transfer and execute the given playback program and fetch results.
	 * @param program PlaybackProgram to run
	 */
	void run(haldls::vx::PlaybackProgram& program) SYMBOL_VISIBLE;

private:
	struct Impl;
	std::unique_ptr<Impl> m_impl;
};

} // namespace stadls::vx
