#pragma once

#include "hate/type_list.h"
#include "hate/visibility.h"
#include <cstdint>
#include <iosfwd>
#include <cereal/macros.hpp>

#if defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)
#include <pybind11/pybind11.h>
#endif

namespace cereal {

struct access;
class JSONOutputArchive;
class JSONInputArchive;
class PortableBinaryOutputArchive;
class PortableBinaryInputArchive;

} // namespace cereal


namespace haldls::vx {

template <typename T>
std::string to_json(T const& t);

template <typename T>
void from_json(T& t, std::string const& s);

template <typename T>
std::string to_portablebinary(T const& t);

template <typename T>
void from_portablebinary(T& t, std::string const& s);

#if defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)
template <typename... Ts>
struct WrapToFromFunctions
{
	static void apply(pybind11::module& m)
	{
		// convert binary-encoded std::string to bytes in Python
		(m.def("to_json", &haldls::vx::to_json<Ts>), ...);
		(m.def("from_json", &haldls::vx::from_json<Ts>), ...);
		// clang-format off
		(m.def("to_portablebinary", [](Ts const& t) {
			   return pybind11::bytes(haldls::vx::to_portablebinary<Ts>(t));
		}), ...);
		// clang-format on
		(m.def("from_portablebinary", &haldls::vx::from_portablebinary<Ts>), ...);
	}
};

template <typename... Ts>
struct WrapToFromFunctions<hate::type_list<Ts...>>
{
	static void apply(pybind11::module& m)
	{
		WrapToFromFunctions<Ts...>::apply(m);
	}
};

// Use with something like this:
// typedef hate::type_list<my_type1, my_type2> my_types;
// GENPYBIND_MANUAL({
//     wrap_tofrom_functions<my_types>(parent);
// })
#endif

} // namespace haldls::vx

// explicitly instantiate our template SYMBOL_VISIBLE functions; includes to_json/from_json
#define EXTERN_INSTANTIATE_CEREAL_SERIALIZE(CLASS_NAME)                                            \
	extern template SYMBOL_VISIBLE void CLASS_NAME ::CEREAL_SERIALIZE_FUNCTION_NAME(               \
	    cereal::JSONOutputArchive&, std::uint32_t const);                                          \
	extern template SYMBOL_VISIBLE void CLASS_NAME ::CEREAL_SERIALIZE_FUNCTION_NAME(               \
	    cereal::JSONInputArchive&, std::uint32_t const);                                           \
	extern template SYMBOL_VISIBLE void CLASS_NAME ::CEREAL_SERIALIZE_FUNCTION_NAME(               \
	    cereal::PortableBinaryOutputArchive&, std::uint32_t const);                                \
	extern template SYMBOL_VISIBLE void CLASS_NAME ::CEREAL_SERIALIZE_FUNCTION_NAME(               \
	    cereal::PortableBinaryInputArchive&, std::uint32_t const);                                 \
	extern template SYMBOL_VISIBLE std::string haldls::vx::to_json(CLASS_NAME const&);             \
	extern template SYMBOL_VISIBLE std::string haldls::vx::to_portablebinary(CLASS_NAME const&);   \
	extern template SYMBOL_VISIBLE void haldls::vx::from_json(CLASS_NAME&, std::string const&);    \
	extern template SYMBOL_VISIBLE void haldls::vx::from_portablebinary(                           \
	    CLASS_NAME&, std::string const&);

#define EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CLASS_NAME)                                          \
	template void CLASS_NAME ::CEREAL_SERIALIZE_FUNCTION_NAME(                                     \
	    cereal::JSONOutputArchive&, std::uint32_t const);                                          \
	template void CLASS_NAME ::CEREAL_SERIALIZE_FUNCTION_NAME(                                     \
	    cereal::JSONInputArchive&, std::uint32_t const);                                           \
	template void CLASS_NAME ::CEREAL_SERIALIZE_FUNCTION_NAME(                                     \
	    cereal::PortableBinaryOutputArchive&, std::uint32_t const);                                \
	template void CLASS_NAME ::CEREAL_SERIALIZE_FUNCTION_NAME(                                     \
	    cereal::PortableBinaryInputArchive&, std::uint32_t const);                                 \
	template std::string haldls::vx::to_json(CLASS_NAME const&);                                   \
	template std::string haldls::vx::to_portablebinary(CLASS_NAME const&);                         \
	template void haldls::vx::from_json(CLASS_NAME&, std::string const&);                          \
	template void haldls::vx::from_portablebinary(CLASS_NAME&, std::string const&);

#define EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(CLASS_NAME)                                       \
	extern template SYMBOL_VISIBLE void cereal::CEREAL_SERIALIZE_FUNCTION_NAME(                    \
	    cereal::JSONOutputArchive&, CLASS_NAME&, std::uint32_t);                                   \
	extern template SYMBOL_VISIBLE void cereal::CEREAL_SERIALIZE_FUNCTION_NAME(                    \
	    cereal::JSONInputArchive&, CLASS_NAME&, std::uint32_t);                                    \
	extern template SYMBOL_VISIBLE void cereal::CEREAL_SERIALIZE_FUNCTION_NAME(                    \
	    cereal::PortableBinaryOutputArchive&, CLASS_NAME&, std::uint32_t);                         \
	extern template SYMBOL_VISIBLE void cereal::CEREAL_SERIALIZE_FUNCTION_NAME(                    \
	    cereal::PortableBinaryInputArchive&, CLASS_NAME&, std::uint32_t);                          \
	extern template SYMBOL_VISIBLE std::string haldls::vx::to_json(CLASS_NAME const&);             \
	extern template SYMBOL_VISIBLE std::string haldls::vx::to_portablebinary(CLASS_NAME const&);   \
	extern template SYMBOL_VISIBLE void haldls::vx::from_json(CLASS_NAME&, std::string const&);    \
	extern template SYMBOL_VISIBLE void haldls::vx::from_portablebinary(                           \
	    CLASS_NAME&, std::string const&);

#define EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(CLASS_NAME)                                     \
	template void cereal::CEREAL_SERIALIZE_FUNCTION_NAME(                                          \
	    cereal::JSONOutputArchive&, CLASS_NAME&, std::uint32_t);                                   \
	template void cereal::CEREAL_SERIALIZE_FUNCTION_NAME(                                          \
	    cereal::JSONInputArchive&, CLASS_NAME&, std::uint32_t);                                    \
	template void cereal::CEREAL_SERIALIZE_FUNCTION_NAME(                                          \
	    cereal::PortableBinaryOutputArchive&, CLASS_NAME&, std::uint32_t);                         \
	template void cereal::CEREAL_SERIALIZE_FUNCTION_NAME(                                          \
	    cereal::PortableBinaryInputArchive&, CLASS_NAME&, std::uint32_t);                          \
	template std::string haldls::vx::to_json(CLASS_NAME const&);                                   \
	template std::string haldls::vx::to_portablebinary(CLASS_NAME const&);                         \
	template void haldls::vx::from_json(CLASS_NAME&, std::string const&);                          \
	template void haldls::vx::from_portablebinary(CLASS_NAME&, std::string const&);
