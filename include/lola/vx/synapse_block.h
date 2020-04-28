#pragma once

#ifndef CHIP_REVISION
#error "Requires CHIP_REVISION"
#endif

#include "halco/common/iter_all.h"
#include "halco/common/typed_array.h"
#include "halco/common/typed_heap_array.h"
#include "haldls/cerealization.h"
#include "haldls/vx/common.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"
#include "lola/vx/cerealization.h"
#include "lola/vx/genpybind.h"
#include <boost/hana/adapt_struct.hpp>

#define ID(x) x
#define XSTR(x) #x
#define STR(x) XSTR(x)
// clang-format off
#define CONCAT(w, x, y, z) STR(ID(w)ID(/)ID(x)ID(/)ID(v)ID(y)ID(/)ID(z))
#include CONCAT(haldls,vx,CHIP_REVISION,capmem.h)
#include CONCAT(lola,vx,CHIP_REVISION,synapse.h)
// clang-format on
#undef ID
#undef XSTR
#undef STR
#undef CONCAT

#if CHIP_REVISION == 2
#define GENPYBIND_TAG_LOLA_VX_VY GENPYBIND_TAG_LOLA_VX_V2
#define CHIP_REVISION_STR v2
#elif CHIP_REVISION == 3
#define GENPYBIND_TAG_LOLA_VX_VY GENPYBIND_TAG_LOLA_VX_V3
#define CHIP_REVISION_STR v3
#else
#error "Unknown CHIP_REVISION"
#endif

namespace lola::vx::CHIP_REVISION_STR GENPYBIND_TAG_LOLA_VX_VY {

/**
 * Synapses of one hemisphere in conjunction with hemisphere-global configuration.
 */
struct GENPYBIND(visible) SynapseBlock
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::CHIP_REVISION_STR::SynapseBlockOnDLS coordinate_type;

	typedef haldls::vx::CHIP_REVISION_STR::CapMemCell::Value AnalogValue GENPYBIND(visible);
	typedef haldls::vx::CHIP_REVISION_STR::CapMemCell::DisableRefresh AnalogDisableRefresh
	    GENPYBIND(visible);
	typedef haldls::vx::CHIP_REVISION_STR::CapMemCell::value_type AnalogValueVariant;
	typedef SynapseMatrix Matrix GENPYBIND(visible);

	/** Default constructor. */
	SynapseBlock() = default;

	/** Synapses. */
	Matrix matrix;

	typedef halco::common::typed_array<
	    haldls::vx::CHIP_REVISION_STR::CapMemCell::value_type,
	    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemBlockOnHemisphere>
	    AnalogValues;

	/** DAC bias altering the synaptic strength. */
	AnalogValues i_bias_dac{AnalogValue{1022}, AnalogValue{1022}};

	bool operator==(SynapseBlock const& other) const SYMBOL_VISIBLE;
	bool operator!=(SynapseBlock const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SynapseBlock const& config) SYMBOL_VISIBLE;

private:
	friend struct haldls::vx::detail::VisitPreorderImpl<SynapseBlock>;
};

} // namespace lola::vx::vY

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::CHIP_REVISION_STR::SynapseBlock>
    : public BackendContainerBase<
          lola::vx::CHIP_REVISION_STR::SynapseBlock,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::CHIP_REVISION_STR::SynapseBlock>
{
	typedef lola::vx::CHIP_REVISION_STR::SynapseBlock container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> const& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::CHIP_REVISION_STR;

		visitor(coord, config);

		hate::Empty<lola::vx::CHIP_REVISION_STR::SynapseMatrix> synapse_matrix;
		visit_preorder(synapse_matrix, coord.toSynramOnDLS(), visitor);

		for (auto block : iter_all<CapMemBlockOnHemisphere>()) {
			CapMemBlockOnDLS block_on_dls(
			    CapMemBlockOnDLS(block.toEnum() + CapMemBlockOnHemisphere::size * coord.toEnum()));
			hate::Empty<haldls::vx::CHIP_REVISION_STR::CapMemCell> i_bias_dac;
			visit_preorder(
			    i_bias_dac, CapMemCellOnDLS(CapMemCellOnCapMemBlock::syn_i_bias_dac, block_on_dls),
			    visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<typename container_type::coordinate_type> const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::CHIP_REVISION_STR;

		visitor(coord, config);

		visit_preorder(config.matrix, hate::Empty<SynramOnDLS>{}, visitor);

		for (auto block : iter_all<CapMemBlockOnHemisphere>()) {
			haldls::vx::CHIP_REVISION_STR::CapMemCell i_bias_dac(config.i_bias_dac[block]);
			visit_preorder(i_bias_dac, hate::Empty<CapMemCellOnDLS>{}, visitor);
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				config.i_bias_dac[block] = i_bias_dac.get_value();
			}
		}
	}

	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::CHIP_REVISION_STR;

		visitor(coord, config);

		visit_preorder(config.matrix, coord.toSynramOnDLS(), visitor);

		for (auto block : iter_all<CapMemBlockOnHemisphere>()) {
			CapMemBlockOnDLS block_on_dls(
			    CapMemBlockOnDLS(block.toEnum() + CapMemBlockOnHemisphere::size * coord.toEnum()));
			haldls::vx::CHIP_REVISION_STR::CapMemCell i_bias_dac(config.i_bias_dac[block]);
			visit_preorder(
			    i_bias_dac, CapMemCellOnDLS(CapMemCellOnCapMemBlock::syn_i_bias_dac, block_on_dls),
			    visitor);
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				config.i_bias_dac[block] = i_bias_dac.get_value();
			}
		}
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(lola::vx::CHIP_REVISION_STR::SynapseBlock, matrix, i_bias_dac);

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::CHIP_REVISION_STR::SynapseBlock)
