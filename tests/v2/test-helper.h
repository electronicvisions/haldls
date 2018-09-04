#pragma once
#include <random>

/**
 * Draw a ranged type T variable out of all values but the specified default.
 * @tparam T Ranged type
 * @param default_value Default value to exclude from random draw
 * @return random value from {0,...,T::max} \ default_value
 */
template <class T>
T draw_ranged_non_default_value(size_t default_value)
{
	size_t rnd;
	do {
		rnd = rand() % T::size;
	} while(rnd == default_value);
	return T(rnd);
}
