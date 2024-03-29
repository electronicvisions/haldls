#include <sstream>
#include <gtest/gtest.h>

#include "lola/vx/v4/container.h"

using namespace lola::vx::v4;

TEST(LolaContainerTests, HasOstreamOperator)
{
#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	{                                                                                              \
		std::stringstream ss;                                                                      \
		Type obj;                                                                                  \
		ss << obj;                                                                                 \
	}
#include "lola/vx/v4/container.def"
}
