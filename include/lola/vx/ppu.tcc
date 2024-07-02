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
	    hate::Empty<ContainerT> const& config,
	    lola::vx::ExternalPPUMemoryBlock::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		auto const quad_min = coord.toMin().value() / sizeof(uint32_t);
		auto const quad_max = coord.toMax().value() / sizeof(uint32_t);

		hate::Empty<ExternalPPUMemoryQuad> quad_config;
		for (size_t quad = quad_min; quad <= quad_max; ++quad) {
			visit_preorder(quad_config, ExternalPPUMemoryQuadOnFPGA(quad), visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
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

		auto const byte_min = coord.toMin().value();
		auto const quad_min = coord.toMin().value() / sizeof(uint32_t);
		auto const quad_max = coord.toMax().value() / sizeof(uint32_t);

		ExternalPPUMemoryQuad quad_config;
		for (size_t quad = quad_min; quad <= quad_max; ++quad) {
			quad_config.m_enables.fill(false);
			quad_config.m_quad.fill(ExternalPPUMemoryByte::Value());

			size_t const local_byte_min =
			    (quad == quad_min ? coord.toMin().value() : quad * sizeof(uint32_t));
			size_t const local_byte_max =
			    (quad == quad_max ? coord.toMax().value() : ((quad + 1) * sizeof(uint32_t) - 1));

			for (size_t byte = local_byte_min; byte <= local_byte_max; ++byte) {
				EntryOnQuad const entry(byte % sizeof(uint32_t));
				quad_config.m_quad[entry] = config.m_bytes.at(byte - byte_min).m_value;
				quad_config.m_enables[entry] = true;
			}

			visit_preorder(quad_config, ExternalPPUMemoryQuadOnFPGA(quad), visitor);

			if constexpr (!std::is_same_v<ContainerT, lola::vx::ExternalPPUMemoryBlock const>) {
				for (size_t byte = local_byte_min; byte <= local_byte_max; ++byte) {
					EntryOnQuad const entry(byte % sizeof(uint32_t));
					config.m_bytes.at(byte - byte_min).m_value = quad_config.m_quad[entry];
				}
			}
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
	    hate::Empty<ContainerT> const& config,
	    lola::vx::ExternalPPUMemory::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		hate::Empty<ExternalPPUMemoryQuad> quad_config;
		for (auto const quad : halco::common::iter_all<ExternalPPUMemoryQuadOnFPGA>()) {
			visit_preorder(quad_config, quad, visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<lola::vx::ExternalPPUMemory::coordinate_type> const& coord,
	    VisitorT&& visitor)
	{
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		ExternalPPUMemoryQuad quad_config;
		for (auto const quad : halco::common::iter_all<ExternalPPUMemoryQuadOnFPGA>()) {
			for (auto const entry : halco::common::iter_all<EntryOnQuad>()) {
				quad_config.m_quad[entry] =
				    config
				        .bytes[ExternalPPUMemoryByteOnFPGA(
				            quad.value() * EntryOnQuad::size + entry.value())]
				        .m_value;
			}
			visit_preorder(quad_config, hate::Empty<ExternalPPUMemoryQuadOnFPGA>{}, visitor);
			if constexpr (!std::is_same_v<ContainerT, lola::vx::ExternalPPUMemory const>) {
				for (auto const entry : halco::common::iter_all<EntryOnQuad>()) {
					config
					    .bytes[ExternalPPUMemoryByteOnFPGA(
					        quad.value() * EntryOnQuad::size + entry.value())]
					    .m_value = quad_config.m_quad[entry];
				}
			}
		}
	}

	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    lola::vx::ExternalPPUMemory::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		ExternalPPUMemoryQuad quad_config;
		for (auto const quad : halco::common::iter_all<ExternalPPUMemoryQuadOnFPGA>()) {
			for (auto const entry : halco::common::iter_all<EntryOnQuad>()) {
				quad_config.m_quad[entry] =
				    config
				        .bytes[ExternalPPUMemoryByteOnFPGA(
				            quad.value() * EntryOnQuad::size + entry.value())]
				        .m_value;
			}
			visit_preorder(quad_config, quad, visitor);
			if constexpr (!std::is_same_v<ContainerT, lola::vx::ExternalPPUMemory const>) {
				for (auto const entry : halco::common::iter_all<EntryOnQuad>()) {
					config
					    .bytes[ExternalPPUMemoryByteOnFPGA(
					        quad.value() * EntryOnQuad::size + entry.value())]
					    .m_value = quad_config.m_quad[entry];
				}
			}
		}
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(lola::vx::ExternalPPUMemory, bytes);
BOOST_HANA_ADAPT_STRUCT(lola::vx::PPUProgram::Symbol, type, coordinate);
