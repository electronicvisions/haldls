#include <gtest/gtest.h>

#include "haldls/cerealization.h"
#include "haldls/vx/v2/jtag.h"
#include "haldls/vx/v2/perftest.h"
#include "haldls/vx/v2/ppu.h"
#include "haldls/vx/v2/reset.h"
#include "haldls/vx/v2/spi.h"
#include "haldls/vx/v2/timer.h"

#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>

using namespace haldls::vx::v2;

template <class T>
class CommonSerializationTests : public ::testing::Test
{};

typedef ::testing::Types<
    PPUMemoryWord,
    PPUControlRegister,
    PPUStatusRegister,
    ResetJTAGTap,
    Timer,
    ResetChip,
    JTAGClockScaler,
    JTAGIdCode,
    ShiftRegister,
    DACChannel,
    DACControl,
    PerfTest,
    PerfTestStatus>
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

TYPED_TEST(CommonSerializationTests, HasToFromSerialization)
{
	TypeParam obj1, obj2;
	std::string s;

	{
		s = haldls::vx::to_json(obj1);
		haldls::vx::from_json(obj2, s);

		// This does only test that Serialization does not insert wrong values;
		// cf. above.
		ASSERT_EQ(obj2, obj1);
	}
	{
		s = haldls::vx::to_binary(obj1);
		haldls::vx::from_binary(obj2, s);

		// This does only test that Serialization does not insert wrong values;
		// cf. above.
		ASSERT_EQ(obj2, obj1);
	}
	{
		s = haldls::vx::to_portablebinary(obj1);
		haldls::vx::from_portablebinary(obj2, s);

		// This does only test that Serialization does not insert wrong values;
		// cf. above.
		ASSERT_EQ(obj2, obj1);
	}
	{
		s = haldls::vx::to_xml(obj1);
		haldls::vx::from_xml(obj2, s);

		// This does only test that Serialization does not insert wrong values;
		// cf. above.
		ASSERT_EQ(obj2, obj1);
	}
}
