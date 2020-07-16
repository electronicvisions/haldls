#include <sstream>
#include <gtest/gtest.h>

#include "lola/vx/container.h"

using namespace haldls::vx;

TEST(LolaContainerTests, HasOstreamOperator)
{
#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	{                                                                                              \
		std::stringstream ss;                                                                      \
		Type obj;                                                                                  \
		ss << obj;                                                                                 \
	}
#include "lola/vx/container.def"
}
