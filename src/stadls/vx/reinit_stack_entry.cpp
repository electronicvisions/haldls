#include "stadls/vx/reinit_stack_entry.h"

#include <stdexcept>

namespace stadls::vx {

ReinitStackEntry::~ReinitStackEntry()
{
	/* needs to be specified here because Impl not complete in inline-default definition */
}

void ReinitStackEntry::set(
    PlaybackProgram const& pbmem_request,
    std::optional<PlaybackProgram> const& pbmem_snapshot,
    bool const enforce)
{
	if (!m_impl) {
		throw std::runtime_error("Unexpected access to moved-from object.");
	}
	if (pbmem_snapshot) {
		m_impl->set(pbmem_request.m_program_impl, pbmem_snapshot->m_program_impl, enforce);
	} else {
		m_impl->set(pbmem_request.m_program_impl, std::nullopt, enforce);
	}
}

void ReinitStackEntry::enforce()
{
	if (!m_impl) {
		throw std::runtime_error("Unexpected access to moved-from object.");
	}
	m_impl->enforce();
}

void ReinitStackEntry::pop()
{
	if (!m_impl) {
		throw std::runtime_error("Unexpected access to moved-from object.");
	}
	m_impl->pop();
}

} // namespace stadls::vx
