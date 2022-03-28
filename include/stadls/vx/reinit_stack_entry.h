#pragma once
#include "hate/visibility.h"
#include "stadls/vx/genpybind.h"
#include <memory>

namespace stadls::vx GENPYBIND_TAG_STADLS_VX {

class PlaybackProgram;

class ReinitStackEntry
{
public:
	ReinitStackEntry() = delete;
	template <typename Connection>
	ReinitStackEntry(Connection& connection);
	ReinitStackEntry(ReinitStackEntry const&) = delete;
	ReinitStackEntry(ReinitStackEntry&&) = default;
	ReinitStackEntry& operator=(ReinitStackEntry&&) = default;
	~ReinitStackEntry() SYMBOL_VISIBLE;

	void set(PlaybackProgram const& pbmem, bool enforce = true) SYMBOL_VISIBLE;

	void enforce() SYMBOL_VISIBLE;

	void pop() SYMBOL_VISIBLE;

private:
	class Impl;
	std::unique_ptr<Impl> m_impl;
};

} // namespace stadls::vx

#include "stadls/vx/reinit_stack_entry.tcc"
