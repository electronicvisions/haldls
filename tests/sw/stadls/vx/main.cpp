#include "hate/visibility.h"
#include <string>
#include <boost/program_options.hpp>
#include <gtest/gtest.h>

// logger include directory structure omits prefix
#include "logging_ctrl.h"

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);

	std::string loglevel;
	namespace bpo = boost::program_options;
	bpo::options_description desc("Options");
	desc.add_options()("loglevel", bpo::value<std::string>(&loglevel)->default_value("warning"));

	bpo::variables_map vm;
	bpo::store(
	    bpo::basic_command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm);
	bpo::notify(vm);

	if (loglevel == "trace") {
		logger_default_config(log4cxx::Level::getTrace());
	} else if (loglevel == "debug") {
		logger_default_config(log4cxx::Level::getDebug());
	} else if (loglevel == "info") {
		logger_default_config(log4cxx::Level::getInfo());
	} else if (loglevel == "warning") {
		logger_default_config(log4cxx::Level::getWarn());
	} else if (loglevel == "error") {
		logger_default_config(log4cxx::Level::getError());
	} else if (loglevel == "fatal") {
		logger_default_config(log4cxx::Level::getFatal());
	} else {
		throw std::runtime_error(
		    "loglevel option has to be one of {trace,debug,info,warning,error,fatal}");
	}

	return RUN_ALL_TESTS();
}
