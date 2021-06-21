#pragma once

#include <cmath>
#include <iomanip>
#include <random>
#include <string>
#include <boost/preprocessor/seq/for_each.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "halco/common/iter_sparse.h"
#include "haldls/cerealization.tcc"
#include "haldls/vx/common.h"
#include "hate/math.h"
#include "hate/type_index.h"
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
 * Test ostream operator for output of single value.
 */
template <class T>
void test_ostream_operator_single_value()
{
	using namespace hate::math;
	T o;
	auto val = draw_ranged_non_default_value<typename T::Value>();
	o.set_value(val);
	std::stringstream expect_to_find;
	expect_to_find << val.value();
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

/**
 * Generate random values of given type excluding a given value.
 * Exclusion of a value is to be used to exclude the default value to ensure that when randomizing
 * properties of a container, the container can never reach its default value.
 * @note Needs to be specialized for enums (and arrays)
 * @tparam T Type to generate
 */
template <typename T, typename = void>
struct random_value;

template <>
struct random_value<bool>
{
	bool operator()(bool const& exclude) const
	{
		return !exclude;
	}
};

template <typename T>
struct random_value<T, std::void_t<typename T::base_t>>
{
	/**
	 * Generate random value.
	 * @param exclude Value to exclude
	 * @return Random value unequal to `exclude`
	 */
	T operator()(T const& exclude) const
	{
		if constexpr (halco::common::detail::IsRantWrapper<typename T::base_t>::value) {
			return draw_ranged_non_default_value<T>(exclude);
		} else if constexpr (halco::common::detail::IsBaseType<typename T::base_t>::value) {
			return draw_non_default_value<T>(exclude);
		}
	}
};

/**
 * Test comparison operators for given config unequal to default.
 * @param config Non-default config
 */
#define HALDLS_TEST_COMPARISON(config)                                                             \
	auto const config_eq = config;                                                                 \
	decltype(config) config_default;                                                               \
                                                                                                   \
	ASSERT_EQ(config, config_eq);                                                                  \
	ASSERT_FALSE(config == config_default);                                                        \
                                                                                                   \
	ASSERT_NE(config, config_default);                                                             \
	ASSERT_FALSE(config != config_eq);

/**
 * Set non-default property.
 * @param config Config to modify
 * @param property Name of property to modify
 */
#define HALDLS_SET_PROPERTY_INTERNAL(config, property)                                             \
	{                                                                                              \
		auto def = config.get_##property();                                                        \
		config.set_##property(random_value<decltype(def)>{}(def));                                 \
	}

/**
 * Test property access with non-default random value.
 * @param config Config to modify and test
 * @param property Name of property to modify and test
 */
#define HALDLS_TEST_GETTER_SETTER_INTERNAL(config, property)                                       \
	{                                                                                              \
		auto def = config.get_##property();                                                        \
		auto const value = random_value<decltype(def)>{}(def);                                     \
		config.set_##property(value);                                                              \
		EXPECT_EQ(config.get_##property(), value);                                                 \
	}

/** Adapter to BOOST_PP_SEQ_FOR_EACH. */
#define HALDLS_SET_PROPERTY(r, config, property) HALDLS_SET_PROPERTY_INTERNAL(config, property)

/** Adapter to BOOST_PP_SEQ_FOR_EACH. */
#define HALDLS_TEST_GETTER_SETTER(r, config, property)                                             \
	HALDLS_TEST_GETTER_SETTER_INTERNAL(config, property)

/**
 * Test general container features, i.e. getter/setter properties and comparison operators.
 * @param type Container type to test
 * @param property_seq Property name sequence in the format (name1)(name2)...
 */
#define HALDLS_TEST_GENERAL(type, property_seq)                                                    \
	TEST(type, General)                                                                            \
	{                                                                                              \
		type config;                                                                               \
                                                                                                   \
		BOOST_PP_SEQ_FOR_EACH(HALDLS_TEST_GETTER_SETTER, config, property_seq)                     \
                                                                                                   \
		HALDLS_TEST_COMPARISON(config)                                                             \
	}

/**
 * Test cereal serialization coverage.
 * @param type Container type to test
 * @param property_seq Property name sequence in the format (name1)(name2)...
 */
#define HALDLS_TEST_CEREAL(type, property_seq)                                                     \
	TEST(type, CerealizeCoverage)                                                                  \
	{                                                                                              \
		type obj1, obj2;                                                                           \
		BOOST_PP_SEQ_FOR_EACH(HALDLS_SET_PROPERTY, obj1, property_seq)                             \
                                                                                                   \
		std::ostringstream ostream;                                                                \
		{                                                                                          \
			cereal::JSONOutputArchive oa(ostream);                                                 \
			oa(obj1);                                                                              \
		}                                                                                          \
                                                                                                   \
		std::istringstream istream(ostream.str());                                                 \
		{                                                                                          \
			cereal::JSONInputArchive ia(istream);                                                  \
			ia(obj2);                                                                              \
		}                                                                                          \
		ASSERT_EQ(obj1, obj2);                                                                     \
	}

/**
 * Test general container features and cereal serialization coverage.
 * @param type Container type to test
 * @param property_seq Property name sequence in the format (name1)(name2)...
 */
#define HALDLS_TEST(type, property_seq)                                                            \
	HALDLS_TEST_GENERAL(type, property_seq)                                                        \
	HALDLS_TEST_CEREAL(type, property_seq)

/**
 * Test encode and decode of given container with given expectation.
 * @param config Config to test
 * @param coord Coordinate to test
 * @param ref_addresses Reference address values
 * @param ref_data Reference data values
 */
#define HALDLS_TEST_ENCODE_DECODE(config, coord, ref_addresses, ref_data)                          \
	typedef std::vector<decltype(ref_addresses)::value_type> addresses_type;                       \
	typedef std::vector<decltype(ref_data)::value_type> words_type;                                \
	{ /* write addresses */                                                                        \
		addresses_type write_addresses;                                                            \
		visit_preorder(                                                                            \
		    config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});          \
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));                  \
	}                                                                                              \
                                                                                                   \
	{ /* read addresses */                                                                         \
		addresses_type read_addresses;                                                             \
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses}); \
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));                   \
	}                                                                                              \
                                                                                                   \
	words_type data;                                                                               \
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});                        \
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));                                      \
                                                                                                   \
	decltype(config) config_copy;                                                                  \
	ASSERT_NE(config, config_copy);                                                                \
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});        \
	ASSERT_EQ(config, config_copy);

/**
 * Test decode of given read-only container with given expectation.
 * @param config Config to test
 * @param coord Coordinate to test
 * @param ref_addresses Reference address values
 * @param ref_data Reference data values
 */
#define HALDLS_TEST_DECODE(config, coord, ref_addresses, ref_data)                                 \
	typedef std::vector<decltype(ref_addresses)::value_type> addresses_type;                       \
	typedef std::vector<decltype(ref_data)::value_type> words_type;                                \
	{ /* write addresses */                                                                        \
		addresses_type write_addresses;                                                            \
		visit_preorder(                                                                            \
		    config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});          \
		EXPECT_TRUE(write_addresses.empty());                                                      \
	}                                                                                              \
                                                                                                   \
	{ /* read addresses */                                                                         \
		addresses_type read_addresses;                                                             \
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses}); \
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));                   \
	}                                                                                              \
                                                                                                   \
	words_type data;                                                                               \
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});                        \
	EXPECT_TRUE(data.empty());                                                                     \
                                                                                                   \
	data.insert(data.begin(), ref_data.begin(), ref_data.end());                                   \
	decltype(config) config_copy;                                                                  \
	ASSERT_NE(config, config_copy);                                                                \
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});        \
	ASSERT_EQ(config, config_copy);

/**
 * Test encode of given write-only container with given expectation.
 * @param config Config to test
 * @param coord Coordinate to test
 * @param ref_addresses Reference address values
 * @param ref_data Reference data values
 */
#define HALDLS_TEST_ENCODE(config, coord, ref_addresses, ref_data)                                 \
	typedef std::vector<decltype(ref_addresses)::value_type> addresses_type;                       \
	typedef std::vector<decltype(ref_data)::value_type> words_type;                                \
	{ /* write addresses */                                                                        \
		addresses_type write_addresses;                                                            \
		visit_preorder(                                                                            \
		    config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});          \
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));                  \
	}                                                                                              \
                                                                                                   \
	{ /* read addresses */                                                                         \
		addresses_type read_addresses;                                                             \
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses}); \
		EXPECT_TRUE(read_addresses.empty());                                                       \
	}                                                                                              \
                                                                                                   \
	words_type data;                                                                               \
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});                        \
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
