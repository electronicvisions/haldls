#include "stadls/vx/reinit_stack_entry.h"

#include "fisch/vx/reinit_stack_entry.h"
#include "stadls/vx/playback_program.h"

namespace stadls::vx {

// We simply wrap the fisch ReinitStack
class ReinitStackEntry::Impl : public fisch::vx::ReinitStackEntry
{};

template <typename Connection>
ReinitStackEntry::ReinitStackEntry(Connection& connection) : m_impl(new Impl{connection})
{}

} // namespace stadls::vx
