#include <gtest/gtest.h>

#include "haldls/vx/jtag.h"
#include "haldls/vx/ppu.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/spi.h"
#include "haldls/vx/timer.h"

using namespace haldls::vx;

template <class T>
class CommonSerializationTests : public ::testing::Test
{};

typedef ::testing::Types<
    PPUMemoryWord,
    ResetJTAGTap,
    Timer,
    ResetChip,
    JTAGClockScaler,
    JTAGIdCode,
    ShiftRegister,
    DACChannel,
    DACControl>
    SerializableTypes;

TYPED_TEST_CASE(CommonSerializationTests, SerializableTypes);

TYPED_TEST(CommonSerializationTests, IsDefaultConstructible)
{
	TypeParam obj;
	static_cast<void>(&obj);
}

TYPED_TEST(CommonSerializationTests, IsAssignable)
{
	TypeParam obj1, obj2;
	obj1 = obj2;
}

TYPED_TEST(CommonSerializationTests, HasSerialization)
{
	TypeParam obj1, obj2;

	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(obj1);
	}

	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(obj2);
	}
	// This does only test that Serialization does not insert wrong values
	// but does not check coverage since both instances are default constructed.
	// Coverage check is done in each container's test file.
	ASSERT_EQ(obj2, obj1);
}
