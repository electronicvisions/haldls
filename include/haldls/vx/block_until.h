#pragma once
#include "haldls/vx/encodable.h"
#include "haldls/vx/genpybind.h"
#include "hate/visibility.h"
#ifndef __ppu__
#include <memory>
#endif


namespace haldls { namespace vx GENPYBIND_TAG_HALDLS_VX {

/**
 * BlockUntil base class to use for polymorphism in en-/decoding
 */
struct SYMBOL_VISIBLE GENPYBIND(visible) BlockUntil : public Encodable
{
#ifndef __ppu__
	virtual ~BlockUntil() = 0;

	/**
	 * Clone container data.
	 */
	virtual std::unique_ptr<BlockUntil> clone_block_until() const GENPYBIND(hidden) = 0;

	/**
	 * Ostream operator.
	 */
	friend std::ostream& operator<<(std::ostream& os, BlockUntil const& block_until) SYMBOL_VISIBLE;

	virtual bool operator==(BlockUntil const& other) const = 0;
	virtual bool operator!=(BlockUntil const& other) const = 0;
#endif
};

/**
 * BlockUntil base class implementing encode_{read,write}, decode_read via CRTP.
 * @tparam Derived Derived class type
 * @tparam Base Base class type of which to generate a derived class. Needed in order to forward to
 * EncodableBase because inheritance aside the Encodable (which would be base of EncodableBase)
 * leads to non-linear inheritance tree and problems with association of implemented interface.
 */
template <typename Derived, typename Base = BlockUntil>
struct SYMBOL_VISIBLE GENPYBIND(inline_base("*EncodableBase*")) BlockUntilBase
    : public EncodableBase<Derived, Base>
{
#ifndef __ppu__
	/**
	 * Clone container data.
	 */
	virtual std::unique_ptr<BlockUntil> clone_block_until() const GENPYBIND(hidden) SYMBOL_VISIBLE;

	virtual bool operator==(BlockUntil const& other) const SYMBOL_VISIBLE;
	virtual bool operator!=(BlockUntil const& other) const SYMBOL_VISIBLE;
#endif
};

} // namespace vx
} // namespace haldls
