#pragma once
#include <string>
#include "hate/visibility.h"

namespace lola::vx::detail {

/**
 * Map bars of monotonously increasing height to visualize scalar positive values between zero
 * and one. The dot maps to zero.
 * @param value Value to visualize.
 */
std::string gray_scale(double value) SYMBOL_VISIBLE;

} // namespace lola::vx::detail
