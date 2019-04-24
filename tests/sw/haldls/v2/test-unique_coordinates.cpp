#include <boost/type_index.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/v2/container.h"
#include "test-helper.h"

template <typename T>
void test_equal_coordinate()
{
#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	if constexpr (!std::is_same<T, Type>::value) {                                                 \
		EXPECT_FALSE(                                                                              \
		    (std::is_same<typename T::coordinate_type, typename Type::coordinate_type>::value))    \
		    << "Coordinate not unique: " << boost::typeindex::type_id<T>().pretty_name()           \
		    << " and " << #Type << " share the same coordinate_type: "                             \
		    << boost::typeindex::type_id<typename T::coordinate_type>().pretty_name();             \
	}
#include "haldls/v2/container.def"
}

TEST(Container, UniqueCoordinate)
{
#define PLAYBACK_CONTAINER(Name, Type) test_equal_coordinate<Type>();
#include "haldls/v2/container.def"
}
