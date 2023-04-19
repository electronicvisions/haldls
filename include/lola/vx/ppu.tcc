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

		bool handled_min = false;
		size_t offset = 0;
		if (coord.toMin().value() % sizeof(uint32_t) != 0) {
			hate::Empty<ExternalPPUMemoryQuad> quad_config;
			size_t const max = coord.toMin().value() - (coord.toMin().value() % sizeof(uint32_t)) +
			                   sizeof(uint32_t) - 1;
			visit_preorder(quad_config, ExternalPPUMemoryQuadOnFPGA(quad_min), visitor);
			handled_min = true;
			offset = max - coord.toMin().value() + 1;
		}
		bool const handling_max =
		    (quad_min == quad_max) ||
		    (coord.toMax().value() % sizeof(uint32_t) != sizeof(uint32_t) - 1);
		{
			hate::Empty<ExternalPPUMemoryQuad> quad_config;
			auto const max = quad_max - handling_max;
			assert(
			    (max - (quad_min + handled_min) + 1) * sizeof(uint32_t) <=
			    coord.toExternalPPUMemoryBlockSize().value());
			for (auto quad = quad_min + handled_min; quad <= max; quad++) {
				visit_preorder(quad_config, ExternalPPUMemoryQuadOnFPGA(quad), visitor);
			}
			offset += (max - quad_min) * sizeof(uint32_t);
		}
		if (handling_max) {
			hate::Empty<ExternalPPUMemoryQuad> quad_config;
			visit_preorder(quad_config, ExternalPPUMemoryQuadOnFPGA(quad_max), visitor);
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

		auto const quad_min = coord.toMin().value() / sizeof(uint32_t);
		auto const quad_max = coord.toMax().value() / sizeof(uint32_t);

		bool handled_min = false;
		size_t offset = 0;
		if (coord.toMin().value() % sizeof(uint32_t) != 0) {
			ExternalPPUMemoryQuad quad_config;
			quad_config.m_enables.fill(false);
			size_t const max = coord.toMin().value() - (coord.toMin().value() % sizeof(uint32_t)) +
			                   sizeof(uint32_t) - 1;
			{
				size_t i = 0;
				for (auto byte = coord.toMin().value(); byte <= max; ++byte) {
					auto const entry = EntryOnQuad(byte % sizeof(uint32_t));
					quad_config.m_quad[entry] = config.m_bytes.at(i).m_value;
					quad_config.m_enables[entry] = true;
					++i;
				}
			}
			visit_preorder(quad_config, ExternalPPUMemoryQuadOnFPGA(quad_min), visitor);
			if constexpr (!std::is_same_v<ContainerT, lola::vx::ExternalPPUMemoryBlock const>) {
				size_t i = 0;
				for (auto byte = coord.toMin().value(); byte <= max; ++byte) {
					auto const entry = EntryOnQuad(byte % sizeof(uint32_t));
					config.m_bytes.at(i).m_value = quad_config.m_quad[entry];
					i++;
				}
			}
			handled_min = true;
			offset = max - coord.toMin().value() + 1;
		}
		bool const handling_max =
		    (quad_min == quad_max) ||
		    (coord.toMax().value() % sizeof(uint32_t) != sizeof(uint32_t) - 1);
		{
			ExternalPPUMemoryQuad quad_config;
			auto const max = quad_max - handling_max;
			assert(
			    (max - (quad_min + handled_min) + 1) * sizeof(uint32_t) <= config.m_bytes.size());
			for (auto quad = quad_min + handled_min; quad <= max; quad++) {
				auto const local_offset =
				    (quad - (quad_min + handled_min)) * sizeof(uint32_t) + offset;
				for (auto const entry : halco::common::iter_all<EntryOnQuad>()) {
					quad_config.m_quad[entry] =
					    config.m_bytes[local_offset + entry.value()].m_value;
				}
				visit_preorder(quad_config, ExternalPPUMemoryQuadOnFPGA(quad), visitor);
				if constexpr (!std::is_same_v<ContainerT, lola::vx::ExternalPPUMemoryBlock const>) {
					for (auto const entry : halco::common::iter_all<EntryOnQuad>()) {
						config.m_bytes[local_offset + entry.value()].m_value =
						    quad_config.m_quad[entry];
					}
				}
			}
			offset += (max - quad_min) * sizeof(uint32_t);
		}
		if (handling_max) {
			ExternalPPUMemoryQuad quad_config;
			quad_config.m_enables.fill(false);
			size_t const min = coord.toMax().value() - (coord.toMax().value() % sizeof(uint32_t));
			{
				size_t i = offset;
				for (auto byte = min; byte <= coord.toMax().value(); ++byte) {
					auto const entry = EntryOnQuad(byte % sizeof(uint32_t));
					quad_config.m_quad[entry] = config.m_bytes.at(i).m_value;
					quad_config.m_enables[entry] = true;
					++i;
				}
			}
			visit_preorder(quad_config, ExternalPPUMemoryQuadOnFPGA(quad_max), visitor);
			if constexpr (!std::is_same_v<ContainerT, lola::vx::ExternalPPUMemoryBlock const>) {
				size_t i = offset;
				for (auto byte = min; byte <= coord.toMax().value(); ++byte) {
					auto const entry = EntryOnQuad(byte % sizeof(uint32_t));
					config.m_bytes.at(i).m_value = quad_config.m_quad[entry];
					i++;
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
