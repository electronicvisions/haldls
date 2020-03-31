#pragma once
#include <sstream>
#include <vector>
#include <cereal/archives/binary.hpp>
#include <pybind11/pybind11.h>

#include "hate/type_list.h"


namespace haldls::vx {

/**
 * Add pickle support to list of classes.
 * @tparam TL Type list of classes to add pickle support for
 */
template <typename TL>
struct AddPickle;

template <typename... Ts>
struct AddPickle<hate::type_list<Ts...>>
{
	/**
	 * Add pickle support to list of classes.
	 * @note Specified name list has to match type list of wrapped types.
	 * @tparam T Type of wrapped object
	 * @param parent Pybind11 module
	 * @param name Name of object in python module
	 */
	static void apply(pybind11::module& parent, std::vector<std::string> const& container_names)
	{
		apply_impl(parent, container_names, std::make_index_sequence<sizeof...(Ts)>{});
	}

private:
	template <size_t I, size_t... Is>
	static void apply_impl(
	    pybind11::module& parent,
	    std::vector<std::string> const& container_names,
	    std::index_sequence<I, Is...>)
	{
		typedef typename hate::index_type_list_by_integer<I, hate::type_list<Ts...>>::type type;
		apply_single<type>(parent, container_names.at(I));
		if constexpr (sizeof...(Is)) {
			apply_impl(parent, container_names, std::index_sequence<Is...>{});
		}
	}

	/**
	 * Add pickle support to single class.
	 * @note Specified name has to match object of correct wrapped type.
	 * @tparam T Type of wrapped object
	 * @param parent Pybind11 module
	 * @param name Name of object in python module
	 */
	template <typename T>
	static void apply_single(pybind11::module& parent, std::string const& name)
	{
		auto attr = parent.attr(name.c_str());
		auto ism = pybind11::is_method(attr);
		auto const getattr = [](pybind11::object const& self) {
			auto& thing = self.cast<T const&>();
			std::ostringstream os;
			{
				cereal::BinaryOutputArchive ar(os);
				ar << thing;
			}
			if (pybind11::hasattr(self, "__dict__")) {
				return pybind11::make_tuple(pybind11::bytes(os.str()), self.attr("__dict__"));
			} else {
				return pybind11::make_tuple(pybind11::bytes(os.str()));
			}
		};

		auto const setattr = [](pybind11::object& self, pybind11::tuple const& data) {
			auto& thing = self.cast<T&>();
			new (&thing) T();
			std::istringstream is(data[0].cast<std::string>());
			{
				cereal::BinaryInputArchive ar(is);
				ar(thing);
			}
			if (pybind11::hasattr(self, "__dict__")) {
				self.attr("__dict__") = data[1];
			}
		};

		attr.attr("__getstate__") = pybind11::cpp_function(getattr, ism);
		attr.attr("__setstate__") = pybind11::cpp_function(setattr, ism);
	}
};

} // namespace haldls::vx
