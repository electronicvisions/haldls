#pragma once
#include "cereal/types/lola/detail/hana.h"


namespace cereal {

template <typename Archive, typename T>
std::enable_if_t<boost::hana::Struct<T>::value> CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, T& value, std::uint32_t)
{
	using namespace boost::hana::literals;
	boost::hana::for_each(boost::hana::keys(value), [&](auto const key) {
		ar(cereal::make_nvp(key.c_str(), boost::hana::at_key(value, key)));
	});
}

} // namespace cereal
