#pragma once
#include <sstream>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/cereal.hpp>
#include "hate/visibility.h"

namespace haldls::vx {

template <typename T>
std::string to_json(T const& t)
{
	std::stringstream ss;
	{
		cereal::JSONOutputArchive ar(ss);
		ar(t);
	}
	return ss.str();
}

template <typename T>
void from_json(T& t, std::string const& s)
{
	std::stringstream ss(s);
	cereal::JSONInputArchive ar(ss);
	ar(t);
}

} // namespace haldls::vx


// explicitly instantiate our template SYMBOL_VISIBLE functions
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
	template SYMBOL_VISIBLE std::string to_json(CLASS_NAME const&);                                \
	template SYMBOL_VISIBLE void from_json(CLASS_NAME&, std::string const&);
