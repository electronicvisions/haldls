#pragma once
#include <sstream>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/cereal.hpp>
#include "hate/type_list.h"
#include "hate/visibility.h"

namespace haldls::vx {

namespace detail {

template <typename Archive, typename T>
std::string to_whatever(T const& t)
{
	std::stringstream ss;
	{
		Archive ar(ss);
		ar(t);
	}
	return ss.str();
}

template <typename Archive, typename T>
void from_whatever(T& t, std::string const& s)
{
	std::stringstream ss(s);
	Archive ar(ss);
	ar(t);
}

} // namespace detail

template <typename T>
std::string to_json(T const& t)
{
	return detail::to_whatever<cereal::JSONOutputArchive, T>(t);
}

template <typename T>
void from_json(T& t, std::string const& s)
{
	detail::from_whatever<cereal::JSONInputArchive, T>(t, s);
}

template <typename T>
std::string to_binary(T const& t)
{
	return detail::to_whatever<cereal::BinaryOutputArchive, T>(t);
}

template <typename T>
void from_binary(T& t, std::string const& s)
{
	detail::from_whatever<cereal::BinaryInputArchive, T>(t, s);
}

template <typename T>
std::string to_portablebinary(T const& t)
{
	return detail::to_whatever<cereal::PortableBinaryOutputArchive, T>(t);
}

template <typename T>
void from_portablebinary(T& t, std::string const& s)
{
	detail::from_whatever<cereal::PortableBinaryInputArchive, T>(t, s);
}

template <typename T>
std::string to_xml(T const& t)
{
	return detail::to_whatever<cereal::XMLOutputArchive, T>(t);
}

template <typename T>
void from_xml(T& t, std::string const& s)
{
	detail::from_whatever<cereal::XMLInputArchive, T>(t, s);
}

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
		(m.def("to_binary", [](Ts const& t) {
			   return pybind11::bytes(haldls::vx::to_binary<Ts>(t));
		}), ...);
		// clang-format on
		(m.def("from_binary", &haldls::vx::from_binary<Ts>), ...);
		// clang-format off
		(m.def("to_portablebinary", [](Ts const& t) {
			   return pybind11::bytes(haldls::vx::to_portablebinary<Ts>(t));
		}), ...);
		// clang-format on
		(m.def("from_portablebinary", &haldls::vx::from_portablebinary<Ts>), ...);
		(m.def("to_xml", &haldls::vx::to_xml<Ts>), ...);
		(m.def("from_xml", &haldls::vx::from_xml<Ts>), ...);
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
#define EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CLASS_NAME)                                          \
	template SYMBOL_VISIBLE void CLASS_NAME ::serialize(                                           \
	    cereal::BinaryOutputArchive&, std::uint32_t const);                                        \
	template SYMBOL_VISIBLE void CLASS_NAME ::serialize(                                           \
	    cereal::BinaryInputArchive&, std::uint32_t const);                                         \
	template SYMBOL_VISIBLE void CLASS_NAME ::serialize(                                           \
	    cereal::JSONOutputArchive&, std::uint32_t const);                                          \
	template SYMBOL_VISIBLE void CLASS_NAME ::serialize(                                           \
	    cereal::JSONInputArchive&, std::uint32_t const);                                           \
	template SYMBOL_VISIBLE void CLASS_NAME ::serialize(                                           \
	    cereal::PortableBinaryOutputArchive&, std::uint32_t const);                                \
	template SYMBOL_VISIBLE void CLASS_NAME ::serialize(                                           \
	    cereal::PortableBinaryInputArchive&, std::uint32_t const);                                 \
	template SYMBOL_VISIBLE void CLASS_NAME ::serialize(                                           \
	    cereal::XMLOutputArchive&, std::uint32_t const);                                           \
	template SYMBOL_VISIBLE void CLASS_NAME ::serialize(                                           \
	    cereal::XMLInputArchive&, std::uint32_t const);                                            \
	template SYMBOL_VISIBLE std::string haldls::vx::to_json(CLASS_NAME const&);                    \
	template SYMBOL_VISIBLE std::string haldls::vx::to_binary(CLASS_NAME const&);                  \
	template SYMBOL_VISIBLE std::string haldls::vx::to_portablebinary(CLASS_NAME const&);          \
	template SYMBOL_VISIBLE std::string haldls::vx::to_xml(CLASS_NAME const&);                     \
	template SYMBOL_VISIBLE void haldls::vx::from_json(CLASS_NAME&, std::string const&);           \
	template SYMBOL_VISIBLE void haldls::vx::from_binary(CLASS_NAME&, std::string const&);         \
	template SYMBOL_VISIBLE void haldls::vx::from_portablebinary(CLASS_NAME&, std::string const&); \
	template SYMBOL_VISIBLE void haldls::vx::from_xml(CLASS_NAME&, std::string const&);
