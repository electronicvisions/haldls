#pragma once

namespace haldls {
namespace common {

/// \brief Implementation of the passkey idiom that allows to give specific classes
/// access to private members.
template <typename T>
class Passkey
{
private:
	friend T;
	Passkey() {}
	Passkey(Passkey const& passkey) = default;
	Passkey& operator=(Passkey const&) = delete;
};

} // namespace common
} // namespace haldls
