#include "hate/visibility.h"

#include <optional>
#include <string>

#include <boost/program_options.hpp>
#include <gtest/gtest.h>

// logger include directory structure omits prefix
#include "logging_ctrl.h"

#if REDUCED_TESTS
std::optional<size_t> SYMBOL_VISIBLE max_words_per_reduced_test = MAX_WORDS_PER_REDUCED_TEST;
#else
std::optional<size_t> SYMBOL_VISIBLE max_words_per_reduced_test = std::nullopt;
#endif

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);

	std::string loglevel;
	namespace bpo = boost::program_options;
	bpo::options_description desc("Options");
	desc.add_options()("loglevel", bpo::value<std::string>(&loglevel));

	bpo::variables_map vm;
	bpo::store(
	    bpo::basic_command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm);
	bpo::notify(vm);

	if (!vm.count("loglevel")) {
		// defaults, if not overridden by command line arguments
		logger_default_config(log4cxx::Level::getInfo());
		logger_set_loglevel(log4cxx::Logger::getLogger("hxcomm"), log4cxx::Level::getWarn());
	} else {
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
	}

	return RUN_ALL_TESTS();
}
