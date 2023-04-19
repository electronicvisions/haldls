#include "haldls/vx/container.h"

#ifndef __ppu__
#include <map>
#include <ostream>
#include <typeinfo>

namespace haldls::vx {

Container::~Container() {}

std::ostream& operator<<(std::ostream& os, Container const& container)
{
	return container.print(os);
}

namespace {

typedef std::
    map<std::type_index, std::unique_ptr<Container> (*)(Container::Coordinate const& coordinate)>
        Storage;

Storage& get_storage()
{
	static std::map<
	    std::type_index, std::unique_ptr<Container> (*)(Container::Coordinate const& coordinate)>
	    storage;
	return storage;
}

} // namespace

hate::Nil register_container(
    std::type_index coordinate_type,
    std::unique_ptr<Container> (*construct_container)(Container::Coordinate const& coordinate))
{
	get_storage()[coordinate_type] = construct_container;

	// return type only needed for invoking the function at construction of statics time
	return {};
}

std::unique_ptr<Container> construct_container(Container::Coordinate const& coordinate)
{
	try {
		return get_storage().at(typeid(coordinate))(coordinate);
	} catch (std::out_of_range const& err) {
		throw std::out_of_range("Container not registered.");
	}
}

} // namespace haldls::vx
#endif
