#pragma once
#include "hate/indent.h"
#include "hate/type_index.h"
#include <ostream>
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

template <typename T>
std::enable_if_t<boost::hana::Struct<T>::value, std::ostream&> print(
    std::ostream& os, T const& value)
{
	using namespace boost::hana::literals;
	hate::IndentingOstream ios(os);
	ios << hate::name<T>() << "(\n";
	ios << hate::Indentation("\t");
	boost::hana::for_each(boost::hana::keys(value), [&](auto const key) {
		ios << boost::hana::to<char const*>(key) << ": " << boost::hana::at_key(value, key) << "\n";
	});
	ios << hate::Indentation() << ")";
	return os;
}
