#include "stadls/vx/reinit_stack_entry.h"

#include <stdexcept>

namespace stadls::vx {

ReinitStackEntry::~ReinitStackEntry()
{
	/* needs to be specified here because Impl not complete in inline-default definition */
}

void ReinitStackEntry::set(
    std::vector<std::reference_wrapper<PlaybackProgram>> const& pbmem_requests,
    std::optional<std::vector<std::reference_wrapper<PlaybackProgram>>> const& pbmem_snapshots,
    bool const enforce)
{
	if (!m_impl) {
		throw std::runtime_error("Unexpected access to moved-from object.");
	}

	std::vector<std::shared_ptr<fisch::vx::PlaybackProgram>> fisch_requests;
	for (auto& request : pbmem_requests) {
		fisch_requests.push_back(request.get().m_program_impl);
	}

	if (pbmem_snapshots) {
		std::vector<std::shared_ptr<fisch::vx::PlaybackProgram>> fisch_snapshots;
		for (auto& snapshot : *pbmem_snapshots) {
			fisch_snapshots.push_back(snapshot.get().m_program_impl);
		}
		m_impl->set(fisch_requests, fisch_snapshots, enforce);
	} else {
		m_impl->set(fisch_requests, std::nullopt, enforce);
	}
}

void ReinitStackEntry::set(
    std::vector<PlaybackProgram>&& pbmem_requests,
    std::optional<std::vector<PlaybackProgram>>&& pbmem_snapshots,
    bool const enforce)
{
	if (!m_impl) {
		throw std::runtime_error("Unexpected access to moved-from object.");
	}

	auto moved_impls = [](std::vector<PlaybackProgram>& programs) {
		std::vector<std::shared_ptr<fisch::vx::PlaybackProgram>> program_ptrs;
		program_ptrs.reserve(programs.size());
		std::transform(
		    programs.begin(), programs.end(), std::back_inserter(program_ptrs),
		    [](PlaybackProgram& program) { return std::move(program.m_program_impl); });
		return program_ptrs;
	};

	if (pbmem_snapshots) {
		m_impl->set(
		    std::move(moved_impls(pbmem_requests)), std::move(moved_impls(*pbmem_snapshots)),
		    enforce);
	} else {
		m_impl->set(std::move(moved_impls(pbmem_requests)), std::nullopt, enforce);
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
