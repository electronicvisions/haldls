#include <sstream>
#include <gtest/gtest.h>

#include "haldls/vx/v3/container.h"

using namespace haldls::vx::v3;

TEST(HaldlsContainerTests, HasOstreamOperator)
{
#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	{                                                                                              \
		std::stringstream ss;                                                                      \
		Type obj;                                                                                  \
		ss << obj;                                                                                 \
	}
#include "haldls/vx/v3/container.def"
}
