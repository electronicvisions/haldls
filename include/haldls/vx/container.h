#pragma once
#include "halco/common/coordinate.h"
#include "haldls/vx/encodable.h"
#include "haldls/vx/genpybind.h"
#include "hate/visibility.h"
#ifndef __ppu__
#include "fisch/vx/container.h"
#include "fisch/vx/coordinates.h"
#include "fisch/vx/word_access_type.h"
#include "haldls/vx/traits.h"
#include "hate/nil.h"
#include "hxcomm/vx/target.h"
#include <iosfwd>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>
#include <vector>
#endif


namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Container base class to use for polymorphism in en-/decoding
 */
struct SYMBOL_VISIBLE GENPYBIND(visible) Container : public Encodable
{
#ifndef __ppu__
	typedef Encodable::Coordinate Coordinate;
	typedef Encodable::BackendCocoListVariant BackendCocoListVariant;
	typedef Encodable::BackendContainerListVariant BackendContainerListVariant;
	typedef Encodable::BackendCoordinateListVariant BackendCoordinateListVariant;

	virtual ~Container() = 0;

	/**
	 * Encode read to backend coordinate list variant.
	 * @param coordinate Coordinate to container to use
	 * @return Backend coordinate list variant
	 */
	virtual BackendCoordinateListVariant encode_read(
	    Coordinate const& coordinate, std::optional<Backend> const& backend) const
	    GENPYBIND(hidden) = 0;

	/**
	 * Decode read from backend container list variant.
	 * @param data Backend data to decode from
	 * @param coordinate Coordinate to container to use
	 */
	virtual void decode_read(BackendContainerListVariant const& data, Coordinate const& coordinate)
	    GENPYBIND(hidden) = 0;

	/**
	 * Get unsupported read targets.
	 * Used when only an abstract reference is available, maps
	 * `RealContainer::unsupported_read_targets`.
	 */
	virtual std::initializer_list<hxcomm::vx::Target> get_unsupported_read_targets() const
	    GENPYBIND(hidden) = 0;

	/**
	 * Clone container data.
	 */
	virtual std::unique_ptr<Container> clone_container() const GENPYBIND(hidden) = 0;

	/**
	 * Ostream operator.
	 */
	friend std::ostream& operator<<(std::ostream& os, Container const& container) SYMBOL_VISIBLE;
#endif
};

/**
 * Container base class implementing encode_read, decode_read via CRTP.
 * @tparam Derived Derived class type
 * @tparam Base Base class type of which to generate a derived class. Needed in order to forward to
 * EncodableBase because inheritance aside the Encodable (which would be base of EncodableBase)
 * leads to non-linear inheritance tree and problems with association of implemented interface.
 */
template <typename Derived, typename Base = Container>
struct SYMBOL_VISIBLE GENPYBIND(inline_base("*EncodableBase*")) ContainerBase
    : public EncodableBase<Derived, Base>
{
#ifndef __ppu__
	typedef typename EncodableBase<Derived, Base>::Coordinate Coordinate;
	typedef typename EncodableBase<Derived, Base>::BackendCocoListVariant BackendCocoListVariant;
	typedef typename EncodableBase<Derived, Base>::BackendContainerListVariant
	    BackendContainerListVariant;
	typedef typename EncodableBase<Derived, Base>::BackendCoordinateListVariant
	    BackendCoordinateListVariant;

	/**
	 * Encode read to backend coordinate list variant.
	 * @param coordinate Coordinate to container to use
	 * @return Backend coordinate list variant
	 */
	virtual BackendCoordinateListVariant encode_read(
	    Coordinate const& coordinate, std::optional<Backend> const& backend) const
	    GENPYBIND(hidden) SYMBOL_VISIBLE;

	/**
	 * Decode read from backend container list variant.
	 * @param data Backend data to decode from
	 * @param coordinate Coordinate to container to use
	 */
	virtual void decode_read(BackendContainerListVariant const& data, Coordinate const& coordinate)
	    GENPYBIND(hidden) SYMBOL_VISIBLE;

	/**
	 * Get unsupported read targets.
	 * Used when only an abstract reference is available, maps
	 * `RealContainer::unsupported_read_targets`.
	 */
	virtual std::initializer_list<hxcomm::vx::Target> get_unsupported_read_targets() const
	    GENPYBIND(hidden) SYMBOL_VISIBLE;

	/**
	 * Clone container data.
	 */
	virtual std::unique_ptr<Container> clone_container() const GENPYBIND(hidden) SYMBOL_VISIBLE;

	virtual bool operator==(Container const& other) const SYMBOL_VISIBLE;
	virtual bool operator!=(Container const& other) const SYMBOL_VISIBLE;
#endif
};

#ifndef __ppu__
/**
 * Register container construction from coordinate function.
 * @param coordinate_type Used type_index of coordinate
 * @param construct_container Function to construct container from coordinate
 */
hate::Nil register_container(
    std::type_index coordinate_type,
    std::unique_ptr<Container> (*construct_container)(Container::Coordinate const& coordinate))
    SYMBOL_VISIBLE;

/**
 * Construct container from given coordinate using registry.
 * @param coordinate Coordinate to construct container for
 * @return Constructed container matching given coordinate
 */
std::unique_ptr<Container> construct_container(Container::Coordinate const& coordinate)
    SYMBOL_VISIBLE;
#endif

} // namespace vx
} // namespace haldls
