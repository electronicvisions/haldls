#pragma once
#include "haldls/vx/is_readable.h"
#include "haldls/vx/is_writeable.h"

namespace haldls::vx::detail {

template <typename T>
struct IsWriteReadable
{
	constexpr static bool value = IsReadable<T>::value && IsWriteable<T>::value;
};

} // namespace haldls::vx
