#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/exception/exceptions.h"

TEST(InvalidConfiguration, General)
{
	std::string test_string("test 123");
	EXPECT_THROW(throw haldls::exception::InvalidConfiguration(test_string), haldls::exception::InvalidConfiguration);
	try {
		throw haldls::exception::InvalidConfiguration(test_string);
	}
	catch (haldls::exception::InvalidConfiguration const& e) {
		EXPECT_EQ(e.what(), "InvalidConfiguration: " + test_string);
	}
}
