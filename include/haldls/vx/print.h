#pragma once
#include <ostream>

namespace haldls::vx {

template <class T>
std::ostream& print_words_for_each_backend(std::ostream& os, T const& config);

#define HALDLS_VX_DEFAULT_OSTREAM_OP(Type)                                                         \
	std::ostream& operator<<(std::ostream& os, Type const& config)                                 \
	{                                                                                              \
		return print_words_for_each_backend(os, config);                                           \
	}

} // namespace haldls::vx
