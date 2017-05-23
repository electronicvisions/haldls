#pragma once


#ifdef __cpp_lib_optional

#include <optional>

namespace haldls {
namespace common {

template <typename T>
using optional = std::optional<T>;

} // namespace common
} // namespace haldls

#else

#include <experimental/optional>

namespace haldls {
namespace common {

template <typename T>
using optional = std::experimental::optional<T>;

} // namespace common
} // namespace haldls

#endif
