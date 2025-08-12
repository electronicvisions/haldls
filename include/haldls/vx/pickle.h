#pragma once
#include "cereal/types/haldls/cereal.h"
#include "hate/type_list.h"
#include <iosfwd>
#include <sstream>
#include <vector>
#include <cereal/archives/portable_binary.hpp>
#include <pybind11/pybind11.h>

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
		auto cls = parent.attr(name.c_str());
		auto ism = pybind11::is_method(cls);
		auto custom_getstate = [](pybind11::object const& self) {
			auto& thing = self.cast<T const&>();
			std::stringstream ss;
			{
				cereal::PortableBinaryOutputArchive ar(ss);
				ar(thing);
			}
			std::string serialized{ss.str()};
			if (pybind11::hasattr(self, "__dict__")) {
				return pybind11::make_tuple(pybind11::bytes(serialized), self.attr("__dict__"));
			} else {
				return pybind11::make_tuple(pybind11::bytes(serialized));
			}
		};

		auto custom_setstate = [](pybind11::detail::value_and_holder& v_h,
		                          pybind11::tuple const& data) {
			std::unique_ptr<T> thing = std::make_unique<T>();

			{
				// restore serialized C++ state
				std::stringstream ss(data[0].cast<std::string>());
				cereal::PortableBinaryInputArchive ar(ss);
				ar(*thing);
			}

			using Base = pybind11::class_<T>;
			// use new-style pickle implementation (detail namespace :/)
			if (data.size() == 1) {
				pybind11::detail::initimpl::setstate<Base>(
				    v_h, std::move(thing), Py_TYPE(v_h.inst) != v_h.type->type);
			} else if (data.size() == 2) {
				auto state = std::make_pair(std::move(thing), data[1].cast<pybind11::dict>());
				pybind11::detail::initimpl::setstate<Base>(
				    v_h, std::move(state), Py_TYPE(v_h.inst) != v_h.type->type);
			} else {
				std::stringstream ss;
				ss << "Wrong state tuple size: " << data.size();
				throw std::runtime_error(ss.str());
			}
		};


		cls.attr("__getstate__") = pybind11::cpp_function(custom_getstate, ism);
		cls.attr("__setstate__") = pybind11::cpp_function(
		    custom_setstate, pybind11::detail::is_new_style_constructor(),
		    pybind11::name("__setstate__"), ism, pybind11::arg("state"), pybind11::pos_only());
	}
};

} // namespace haldls::vx
