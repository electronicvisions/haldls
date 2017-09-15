#pragma once

namespace haldls {
namespace common {

/// \brief Implementation of the passkey idiom that allows to give specific classes
/// access to private members.
template <typename... Ts>
class Passkey;

template <>
class Passkey<>
{
protected:
	Passkey() {}
	Passkey(Passkey const& passkey) = default;
	Passkey& operator=(Passkey const&) = delete;
};

template <typename T, typename... Ts>
class Passkey<T, Ts...> : public Passkey<Ts...>
{
protected:
	friend T;
	using Passkey<Ts...>::Passkey;
};

} // namespace common
} // namespace haldls
