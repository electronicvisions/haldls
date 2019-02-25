#include <string>

#include <boost/program_options.hpp>
#include <gtest/gtest.h>

std::string simulation_ip;
int simulation_port;

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);

	namespace bpo = boost::program_options;
	bpo::options_description desc("Options");
	// clang-format off
	desc.add_options()("simulation_ip", bpo::value<std::string>(&simulation_ip)->default_value("127.0.0.1"))
	    ("simulation_port", bpo::value<int>(&simulation_port)->default_value(50001));
	// clang-format on

	bpo::variables_map vm;
	bpo::store(
	    bpo::basic_command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm);
	bpo::notify(vm);

	return RUN_ALL_TESTS();
}
