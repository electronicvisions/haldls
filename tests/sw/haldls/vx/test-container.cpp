#include <sstream>
#include <gtest/gtest.h>

#include "haldls/vx/container.h"

using namespace haldls::vx;

TEST(HaldlsContainerTests, HasOstreamOperator)
{
#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	{                                                                                              \
		std::stringstream ss;                                                                      \
		Type obj;                                                                                  \
		ss << obj;                                                                                 \
	}
#include "haldls/vx/container.def"
}
