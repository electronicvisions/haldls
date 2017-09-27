#include <gtest/gtest.h>

#include "logging_ctrl.h"

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	logger_default_config(log4cxx::Level::getWarn());
	return RUN_ALL_TESTS();
}
