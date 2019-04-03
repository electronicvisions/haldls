#include <string>

#include <boost/program_options.hpp>
#include <gtest/gtest.h>

std::string fpga_ip;

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);

	namespace bpo = boost::program_options;
	bpo::options_description desc("Options");
	// clang-format off
	desc.add_options()("fpga_ip", bpo::value<std::string>(&fpga_ip)->required());
	// clang-format on

	bpo::variables_map vm;
	bpo::store(
	    bpo::basic_command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm);
	bpo::notify(vm);

	return RUN_ALL_TESTS();
}
