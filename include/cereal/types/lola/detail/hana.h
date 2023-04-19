#pragma once
#include <cstdint>
#include <type_traits>
#include <boost/hana.hpp>
#include <cereal/macros.hpp>

namespace cereal {

template <typename Archive, typename T>
std::enable_if_t<boost::hana::Struct<T>::value> CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, T& value, std::uint32_t);

} // namespace cereal
