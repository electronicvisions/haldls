#include "halco/common/iter_all.h"

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::ExternalPPUMemoryBlock>
    : public BackendContainerBase<
          lola::vx::ExternalPPUMemoryBlock,
          fisch::vx::word_access_type::Omnibus>
{};

template <>
struct VisitPreorderImpl<lola::vx::ExternalPPUMemoryBlock>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    lola::vx::ExternalPPUMemoryBlock::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using namespace halco::hicann_dls::vx;

		if (coord.toExternalPPUMemoryBlockSize() != config.size()) {
			std::stringstream ss;
			ss << "container size(" << config.size() << ") and " << coord << " size do not match.";
			throw std::runtime_error(ss.str());
		}

		visitor(coord, config);

		for (size_t counter = 0; counter < config.size(); counter++) {
			auto byte_coord = ExternalPPUMemoryByteOnFPGA(coord.toMin() + counter);
			visit_preorder(config.m_bytes.at(counter), byte_coord, visitor);
		}
	}
};

template <>
inline lola::vx::ExternalPPUMemoryBlock coordinate_to_container(
    lola::vx::ExternalPPUMemoryBlock::coordinate_type const& coord)
{
	return lola::vx::ExternalPPUMemoryBlock(coord.toExternalPPUMemoryBlockSize());
}


template <>
struct BackendContainerTrait<lola::vx::ExternalPPUMemory>
    : public BackendContainerBase<lola::vx::ExternalPPUMemory, fisch::vx::word_access_type::Omnibus>
{};

template <>
struct VisitPreorderImpl<lola::vx::ExternalPPUMemory>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    lola::vx::ExternalPPUMemory::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const byte_coord : halco::common::iter_all<ExternalPPUMemoryByteOnFPGA>()) {
			visit_preorder(config.bytes[byte_coord], byte_coord, visitor);
		}
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(lola::vx::PPUProgram::Symbol, type, coordinate);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE(lola::vx::ExternalPPUMemoryBlock)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE(lola::vx::ExternalPPUMemory)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::PPUProgram::Symbol)
