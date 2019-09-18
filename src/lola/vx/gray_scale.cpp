#include "lola/vx/gray_scale.h"

#include <array>

namespace lola::vx::detail {

std::string gray_scale(double value)
{
	static const std::array<std::string, 8> gray_scale_map = {"·", "▁", "▂", "▃",
	                                                          "▄", "▅", "▆", "▇"};
	return gray_scale_map.at(size_t(value * gray_scale_map.size()));
}

} // namespace lola::vx::detail
