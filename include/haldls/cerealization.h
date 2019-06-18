#pragma once
#include <cereal/cereal.hpp>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>

// explicitly instantiate our template SYMBOL_VISIBLE functions
#define EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CLASS_NAME)                                          \
	template SYMBOL_VISIBLE void CLASS_NAME ::serialize(cereal::BinaryOutputArchive&);             \
	template SYMBOL_VISIBLE void CLASS_NAME ::serialize(cereal::BinaryInputArchive&);              \
	template SYMBOL_VISIBLE void CLASS_NAME ::serialize(cereal::PortableBinaryOutputArchive&);     \
	template SYMBOL_VISIBLE void CLASS_NAME ::serialize(cereal::PortableBinaryInputArchive&);      \
	template SYMBOL_VISIBLE void CLASS_NAME ::serialize(cereal::JSONOutputArchive&);               \
	template SYMBOL_VISIBLE void CLASS_NAME ::serialize(cereal::JSONInputArchive&);                \
	template SYMBOL_VISIBLE void CLASS_NAME ::serialize(cereal::XMLOutputArchive&);                \
	template SYMBOL_VISIBLE void CLASS_NAME ::serialize(cereal::XMLInputArchive&);
