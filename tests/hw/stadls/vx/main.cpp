#include <string>

#include <boost/program_options.hpp>
#include <gtest/gtest.h>

#include "executor.h"

#ifdef STADLS_TEST_ARQ_EXECUTOR
typename hxcomm::vx::ARQConnection::ip_t fpga_ip;
#else
typename hxcomm::vx::SimConnection::ip_t simulation_ip;
typename hxcomm::vx::SimConnection::port_t simulation_port;
#endif

PlaybackProgramTestExecutor generate_playback_program_test_executor()
{
#ifdef STADLS_TEST_ARQ_EXECUTOR
	return PlaybackProgramTestExecutor(fpga_ip);
#else
	return PlaybackProgramTestExecutor(simulation_ip, simulation_port);
#endif
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);

#ifdef STADLS_TEST_ARQ_EXECUTOR
	namespace bpo = boost::program_options;
	bpo::options_description desc("Options");
	// clang-format off
	desc.add_options()("fpga_ip",
	    bpo::value<typename hxcomm::vx::ARQConnection::ip_t>
	    // Currently (2019-05-14) the only setup, still ugly -> issue #3161
	    (&fpga_ip)->default_value("192.168.4.4"));
	// clang-format on

	bpo::variables_map vm;
	bpo::store(
	    bpo::basic_command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm);
	bpo::notify(vm);
#else
	namespace bpo = boost::program_options;
	bpo::options_description desc("Options");
	// clang-format off
	desc.add_options()("simulation_ip",
	    bpo::value<typename hxcomm::vx::SimConnection::ip_t>
	        (&simulation_ip)->default_value("127.0.0.1"));
	desc.add_options()("simulation_port",
	    bpo::value<typename hxcomm::vx::SimConnection::port_t>(&simulation_port)->required());
	// clang-format on

	bpo::variables_map vm;
	bpo::store(
	    bpo::basic_command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm);
	bpo::notify(vm);
#endif

	return RUN_ALL_TESTS();
}
