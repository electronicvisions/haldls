#pragma once
#define CEREAL_SERIALIZE_FUNCTION_NAME cerealize

#include <cereal/cereal.hpp>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>

#include <cereal/types/array.hpp>
#include <cereal/types/bitset.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/utility.hpp>

// rant's custom serialization code (within rant project)
#include "cereal/types/rant.hpp"

// explicitly instantiate our template SYMBOL_VISIBLE functions
#define EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CLASS_NAME) \
	template SYMBOL_VISIBLE void CLASS_NAME ::cerealize(cereal::BinaryOutputArchive&);         \
	template SYMBOL_VISIBLE void CLASS_NAME ::cerealize(cereal::BinaryInputArchive&);          \
	template SYMBOL_VISIBLE void CLASS_NAME ::cerealize(cereal::PortableBinaryOutputArchive&); \
	template SYMBOL_VISIBLE void CLASS_NAME ::cerealize(cereal::PortableBinaryInputArchive&);  \
	template SYMBOL_VISIBLE void CLASS_NAME ::cerealize(cereal::JSONOutputArchive&);           \
	template SYMBOL_VISIBLE void CLASS_NAME ::cerealize(cereal::JSONInputArchive&);            \
	template SYMBOL_VISIBLE void CLASS_NAME ::cerealize(cereal::XMLOutputArchive&);            \
	template SYMBOL_VISIBLE void CLASS_NAME ::cerealize(cereal::XMLInputArchive&);

#define EXPLICIT_INSTANTIATE_CEREAL_SAVE(CLASS_NAME) \
	template SYMBOL_VISIBLE void CLASS_NAME ::save(cereal::BinaryOutputArchive&) const;         \
	template SYMBOL_VISIBLE void CLASS_NAME ::save(cereal::BinaryInputArchive&) const;          \
	template SYMBOL_VISIBLE void CLASS_NAME ::save(cereal::PortableBinaryOutputArchive&) const; \
	template SYMBOL_VISIBLE void CLASS_NAME ::save(cereal::PortableBinaryInputArchive&) const;  \
	template SYMBOL_VISIBLE void CLASS_NAME ::save(cereal::JSONOutputArchive&) const;           \
	template SYMBOL_VISIBLE void CLASS_NAME ::save(cereal::JSONInputArchive&) const;            \
	template SYMBOL_VISIBLE void CLASS_NAME ::save(cereal::XMLOutputArchive&) const;            \
	template SYMBOL_VISIBLE void CLASS_NAME ::save(cereal::XMLInputArchive&) const;

#define EXPLICIT_INSTANTIATE_CEREAL_LOAD(CLASS_NAME) \
	template SYMBOL_VISIBLE void CLASS_NAME ::load(cereal::BinaryOutputArchive&);         \
	template SYMBOL_VISIBLE void CLASS_NAME ::load(cereal::BinaryInputArchive&);          \
	template SYMBOL_VISIBLE void CLASS_NAME ::load(cereal::PortableBinaryOutputArchive&); \
	template SYMBOL_VISIBLE void CLASS_NAME ::load(cereal::PortableBinaryInputArchive&);  \
	template SYMBOL_VISIBLE void CLASS_NAME ::load(cereal::JSONOutputArchive&);           \
	template SYMBOL_VISIBLE void CLASS_NAME ::load(cereal::JSONInputArchive&);            \
	template SYMBOL_VISIBLE void CLASS_NAME ::load(cereal::XMLOutputArchive&);            \
	template SYMBOL_VISIBLE void CLASS_NAME ::load(cereal::XMLInputArchive&);

#define EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_EXTERNAL(CLASS_NAME) \
	template SYMBOL_VISIBLE void cerealize(cereal::BinaryOutputArchive&, CLASS_NAME &);         \
	template SYMBOL_VISIBLE void cerealize(cereal::BinaryInputArchive&, CLASS_NAME &);          \
	template SYMBOL_VISIBLE void cerealize(cereal::PortableBinaryOutputArchive&, CLASS_NAME &); \
	template SYMBOL_VISIBLE void cerealize(cereal::PortableBinaryInputArchive&, CLASS_NAME &);  \
	template SYMBOL_VISIBLE void cerealize(cereal::JSONOutputArchive&, CLASS_NAME &);           \
	template SYMBOL_VISIBLE void cerealize(cereal::JSONInputArchive&, CLASS_NAME &);            \
	template SYMBOL_VISIBLE void cerealize(cereal::XMLOutputArchive&, CLASS_NAME &);            \
	template SYMBOL_VISIBLE void cerealize(cereal::XMLInputArchive&, CLASS_NAME &);

