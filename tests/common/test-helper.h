#pragma once

#include <cmath>
#include <iomanip>
#include <random>
#include <string>
#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "haldls/cerealization.h"
#include "hate/math.h"
#include "stadls/visitors.h"

/**
 * Draw a type T variable out of all values but the specified default.
 * @tparam T type
 * @param default_value Default value to exclude from random draw
 * @return random value
 */
template <class T>
T draw_non_default_value(T default_value)
{
	static thread_local std::mt19937 g{std::random_device{}()};
	static thread_local std::uniform_int_distribution<uintmax_t> d;

	T rnd;
	do {
		rnd = T(d(g));
	} while (rnd == default_value);
	return rnd;
}

/**
 * Draw a ranged type T variable out of all values but the specified default.
 * @tparam T Ranged type
 * @param default_value Default value to exclude from random draw
 * @return random value from {T::min,...,T::max} \ default_value
 */
template <class T>
T draw_ranged_non_default_value(size_t default_value = T().value())
{
	size_t rnd;
	do {
		rnd = rand() % T::size;
	} while(rnd == default_value);
	return T(T::min + rnd);
}

/**
 * Test a single-value object with ranged type and set_value, get_value for generic functionality.
 */
template <class T>
void test_generic_functionality_single_value()
{
	typedef typename T::Value value_type;
	// test range
	EXPECT_NO_THROW(value_type(T::Value::max));
	EXPECT_NO_THROW(value_type(T::Value::min));

	// test getter/setter
	T o;
	auto val = draw_ranged_non_default_value<value_type>();
	o.set_value(val);
	ASSERT_EQ(o.get_value(), val);

	// test assign
	T o_copy = o;
	auto other_val = draw_ranged_non_default_value<value_type>(val);
	T o_other(other_val);

	// test comparison
	ASSERT_EQ(o_copy, o);
	ASSERT_FALSE(o_copy != o);
	ASSERT_FALSE(o_copy == o_other);
	ASSERT_TRUE(o_copy != o_other);
	ASSERT_NE(o, o_other);
}

/**
 * Test ostream operator for hex output of single value.
 */
template <class T>
void test_hex_ostream_operator_single_value()
{
    using namespace hate::math;
	T o;
	auto val = draw_ranged_non_default_value<typename T::Value>();
	o.set_value(val);
	std::stringstream expect_to_find;
	expect_to_find << std::showbase << std::internal << std::setfill('0')
	               << std::setw(round_up_integer_division(num_bits(T::Value::max), 4)) << std::hex
	               << val.value();
	std::stringstream out;
	out << o;
	ASSERT_TRUE(out.str().find(expect_to_find.str()) != std::string::npos)
	    << "out " << out.str() << "exp " << expect_to_find.str();
}

/*
 * Test (de-)serialization of given type.
 */
template <class T>
void test_non_default_cerealization_single_value()
{
	T o1, o2;
	o1.set_value(draw_ranged_non_default_value<typename T::Value>());
	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(o1);
	}
	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(o2);
	}
	ASSERT_EQ(o1, o2);
}
