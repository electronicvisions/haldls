#pragma once
#include "haldls/vx/block_until.h"

#include "haldls/vx/encodable.tcc"


namespace haldls::vx {

#ifndef __ppu__
template <typename Derived, typename Base>
std::unique_ptr<BlockUntil> BlockUntilBase<Derived, Base>::clone_block_until() const
{
	return std::make_unique<Derived>(static_cast<Derived const&>(*this));
}

template <typename Derived, typename Base>
bool BlockUntilBase<Derived, Base>::operator==(BlockUntil const& other) const
{
	if (auto const other_ptr = dynamic_cast<Derived const*>(&other)) {
		return static_cast<Derived const&>(*this) == *other_ptr;
	}
	return false;
}

template <typename Derived, typename Base>
bool BlockUntilBase<Derived, Base>::operator!=(BlockUntil const& other) const
{
	return !(*this == other);
}

#define EXPLICIT_INSTANTIATE_HALDLS_BLOCK_UNTIL_BASE(Derived)                                      \
	EXPLICIT_INSTANTIATE_HALDLS_ENCODABLE_BASE(Derived, haldls::vx::BlockUntil)                    \
	template struct haldls::vx::BlockUntilBase<Derived, haldls::vx::BlockUntil>;
#else
#define EXPLICIT_INSTANTIATE_HALDLS_BLOCK_UNTIL_BASE(Derived)
#endif

} // namespace haldls::vx
