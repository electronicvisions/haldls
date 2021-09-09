#pragma once
#include "haldls/cerealization.h"

#include <sstream>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>

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
std::string to_portablebinary(T const& t)
{
	return detail::to_whatever<cereal::PortableBinaryOutputArchive, T>(t);
}

template <typename T>
void from_portablebinary(T& t, std::string const& s)
{
	detail::from_whatever<cereal::PortableBinaryInputArchive, T>(t, s);
}

} // namespace haldls::vx
