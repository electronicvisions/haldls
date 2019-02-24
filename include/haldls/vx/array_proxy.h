#pragma once
#include <genpybind.h>

namespace haldls::vx {

template <typename Array, typename Index = size_t>
class ArrayAccessProxy : public Array
{
public:
	template <typename... Args>
	ArrayAccessProxy(Args... args) : Array(args...) {}

	GENPYBIND(expose_as(__getitem__), return_value_policy(reference))
	typename Array::value_type const& getitem(Index const index) const
	{
		return this->operator[](index);
	}

	GENPYBIND(expose_as(__setitem__))
	void setitem(Index const index, typename Array::value_type const& value)
	{
		this->operator[](index) = value;
	}
};

} // namespace haldls::vx
