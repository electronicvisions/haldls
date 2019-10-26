#pragma once
#include <boost/hana.hpp>

template <typename T>
std::enable_if_t<boost::hana::Struct<T>::value, bool> equal(T const& a, T const& b)
{
	using namespace boost::hana::literals;
	bool ret = true;
	boost::hana::for_each(boost::hana::keys(a), [&](auto const key) {
		ret &= (boost::hana::at_key(a, key) == boost::hana::at_key(b, key));
	});
	return ret;
}

template <typename T>
std::enable_if_t<boost::hana::Struct<T>::value, bool> unequal(T const& a, T const& b)
{
	return !equal(a, b);
}
