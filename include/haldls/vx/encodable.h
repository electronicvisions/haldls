#pragma once
#include "halco/common/coordinate.h"
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


namespace haldls::vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Encodable base class to use for polymorphism.
 * An encodable can be encoded to write instructions in the backend, where a write is an instruction
 * without determinisitc response. Base class to Container and BlockUntil.
 */
struct SYMBOL_VISIBLE GENPYBIND(visible) Encodable
{
	typedef halco::common::Coordinate Coordinate GENPYBIND(visible);

#ifndef __ppu__
	typedef std::variant<
#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	std::pair<                                                                                     \
	    std::vector<fisch::vx::Name::coordinate_type>,                                             \
	    std::vector<fisch::vx::word_access_type::Name>>,
#define LAST_PLAYBACK_CONTAINER(Name, Type)                                                        \
	std::pair<                                                                                     \
	    std::vector<fisch::vx::Name::coordinate_type>,                                             \
	    std::vector<fisch::vx::word_access_type::Name>>
#include "fisch/vx/container.def"
	    >
	    BackendCocoListVariant;

	typedef std::variant<
#define PLAYBACK_CONTAINER(Name, Type) std::vector<fisch::vx::word_access_type::Name>,
#define LAST_PLAYBACK_CONTAINER(Name, Type) std::vector<fisch::vx::word_access_type::Name>
#include "fisch/vx/container.def"
	    >
	    BackendContainerListVariant;

	typedef std::variant<
#define PLAYBACK_CONTAINER(Name, Type) std::vector<fisch::vx::Name::coordinate_type>,
#define LAST_PLAYBACK_CONTAINER(Name, Type) std::vector<fisch::vx::Name::coordinate_type>
#include "fisch/vx/container.def"
	    >
	    BackendCoordinateListVariant;

	virtual ~Encodable() = 0;

	/**
	 * Encode write to backend coordinate-container-pair list variant.
	 * @param coordinate Coordinate to container to use
	 * @return Backend coordinate-container-pair list variant
	 */
	virtual BackendCocoListVariant encode_write(
	    Coordinate const& coordinate, std::optional<Backend> const& backend) const
	    GENPYBIND(hidden) = 0;

	/**
	 * Get unsupported write targets.
	 * Used when only an abstract reference is available, maps
	 * `RealEncodable::unsupported_write_targets`.
	 */
	virtual std::initializer_list<hxcomm::vx::Target> get_unsupported_write_targets() const
	    GENPYBIND(hidden) = 0;

	/**
	 * Get whether encodable supports differential write operation.
	 * Used when only an abstract reference is available, maps inheritance of
	 * DifferentialWriteTrait.
	 */
	virtual bool get_supports_differential_write() const GENPYBIND(hidden) = 0;

	/**
	 * Clone data.
	 */
	virtual std::unique_ptr<Encodable> clone_encodable() const GENPYBIND(hidden) = 0;

	/**
	 * Get whether backend is valid.
	 * @param backend Backend to check
	 */
	virtual bool get_is_valid_backend(Backend backend) const GENPYBIND(hidden) = 0;

	/**
	 * Print to ostream.
	 */
	virtual std::ostream& print(std::ostream& os) const GENPYBIND(hidden) = 0;

	/**
	 * Ostream operator.
	 */
	friend std::ostream& operator<<(std::ostream& os, Encodable const& encodable) SYMBOL_VISIBLE;

	virtual bool operator==(Encodable const& other) const = 0;
	virtual bool operator!=(Encodable const& other) const = 0;
#endif
};

/**
 * Encodable base class implementing encode_write (and others) via CRTP.
 */
template <typename Derived, typename Base = Encodable>
struct SYMBOL_VISIBLE EncodableBase : public Base
{
#ifndef __ppu__
	typedef Encodable::Coordinate Coordinate;
	typedef Encodable::BackendCocoListVariant BackendCocoListVariant;
	typedef Encodable::BackendContainerListVariant BackendContainerListVariant;
	typedef Encodable::BackendCoordinateListVariant BackendCoordinateListVariant;

	/**
	 * Encode write to backend coordinate-container-pair list variant.
	 * @param coordinate Coordinate to container to use
	 * @return Backend coordinate-container-pair list variant
	 */
	virtual BackendCocoListVariant encode_write(
	    Coordinate const& coordinate, std::optional<Backend> const& backend) const
	    GENPYBIND(hidden) SYMBOL_VISIBLE;

	/**
	 * Get unsupported write targets.
	 * Used when only an abstract reference is available, maps
	 * `RealEncodable::unsupported_write_targets`.
	 */
	virtual std::initializer_list<hxcomm::vx::Target> get_unsupported_write_targets() const
	    GENPYBIND(hidden) SYMBOL_VISIBLE;

	/**
	 * Get whether encodable supports differential write operation.
	 * Used when only an abstract reference is available, maps inheritance of
	 * DifferentialWriteTrait.
	 */
	virtual bool get_supports_differential_write() const GENPYBIND(hidden) SYMBOL_VISIBLE;

	/**
	 * Clone data.
	 */
	virtual std::unique_ptr<Encodable> clone_encodable() const GENPYBIND(hidden) SYMBOL_VISIBLE;

	/**
	 * Get whether backend is valid.
	 * @param backend Backend to check
	 */
	virtual bool get_is_valid_backend(Backend backend) const GENPYBIND(hidden) SYMBOL_VISIBLE;

	/**
	 * Print to ostream.
	 */
	virtual std::ostream& print(std::ostream& os) const GENPYBIND(hidden) SYMBOL_VISIBLE;

	virtual bool operator==(Encodable const& other) const SYMBOL_VISIBLE;
	virtual bool operator!=(Encodable const& other) const SYMBOL_VISIBLE;
#endif
};

} // namespace haldls::vx
