#pragma once

#ifndef CHIP_REVISION
#error "Requires CHIP_REVISION"
#endif

#define ID(x) x
#define XSTR(x) #x
#define STR(x) XSTR(x)
// clang-format off
#define CONCAT(w, x, y, z) STR(ID(w)ID(/)ID(x)ID(/)ID(v)ID(y)ID(/)ID(z))
#include CONCAT(haldls,vx,CHIP_REVISION,capmem.h)
#include CONCAT(haldls,vx,CHIP_REVISION,padi.h)
#include CONCAT(haldls,vx,CHIP_REVISION,synapse_driver.h)
// clang-format on
#undef ID
#undef XSTR
#undef STR
#undef CONCAT

#include "halco/common/iter_all.h"
#include "halco/common/typed_array.h"
#include "haldls/cerealization.h"
#include "haldls/vx/common.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"
#include "lola/vx/cerealization.h"
#include "lola/vx/genpybind.h"
#include <boost/hana/adapt_struct.hpp>

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
 * Block on synapse drivers and global configuration of one hemisphere.
 */
class GENPYBIND(visible) SynapseDriverBlock
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::CHIP_REVISION_STR::SynapseDriverBlockOnDLS coordinate_type;

	/** Default constructor. */
	SynapseDriverBlock() = default;

	typedef halco::common::typed_array<
	    haldls::vx::CHIP_REVISION_STR::SynapseDriverConfig,
	    halco::hicann_dls::vx::CHIP_REVISION_STR::SynapseDriverOnSynapseDriverBlock>
	    SynapseDrivers GENPYBIND(opaque(false));

	/**
	 * Block of synapse drivers.
	 */
	SynapseDrivers synapse_drivers;

	/**
	 * Configuration of PADI-busses.
	 */
	haldls::vx::CHIP_REVISION_STR::CommonPADIBusConfig padi_bus;

	/**
	 * Digital STP configuration.
	 */
	haldls::vx::CHIP_REVISION_STR::CommonSTPConfig stp;

	typedef haldls::vx::CHIP_REVISION_STR::CapMemCell::Value AnalogValue GENPYBIND(visible);
	typedef haldls::vx::CHIP_REVISION_STR::CapMemCell::DisableRefresh AnalogDisableRefresh
	    GENPYBIND(visible);
	typedef haldls::vx::CHIP_REVISION_STR::CapMemCell::value_type AnalogValueVariant;
	typedef halco::common::typed_array<
	    haldls::vx::CHIP_REVISION_STR::CapMemCell::value_type,
	    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemBlockOnHemisphere>
	    AnalogValues;

	AnalogValues stp_v_charge_0;
	AnalogValues stp_v_charge_1;
	AnalogValues stp_v_recover_0;
	AnalogValues stp_v_recover_1;
	AnalogValues stp_i_bias_comparator;
	AnalogValues stp_i_ramp;
	AnalogValues stp_i_calib;

	AnalogValueVariant stp_i_bias_readout;
	AnalogValueVariant hagen_i_bias_dac;

	bool operator==(SynapseDriverBlock const& other) const SYMBOL_VISIBLE;
	bool operator!=(SynapseDriverBlock const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SynapseDriverBlock const& config)
	    SYMBOL_VISIBLE;

private:
	friend class haldls::vx::detail::VisitPreorderImpl<SynapseDriverBlock>;
};

} // namespace lola::vx::vY

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::CHIP_REVISION_STR::SynapseDriverBlock>
    : public BackendContainerBase<
          lola::vx::CHIP_REVISION_STR::SynapseDriverBlock,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::CHIP_REVISION_STR::SynapseDriverBlock>
{
	typedef lola::vx::CHIP_REVISION_STR::SynapseDriverBlock container_type;

	template <typename VisitorT>
	static void call(
	    hate::Empty<container_type> const& config,
	    lola::vx::CHIP_REVISION_STR::SynapseDriverBlock::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::CHIP_REVISION_STR;

		visitor(coord, config);

		for (auto const on_block : iter_all<SynapseDriverOnSynapseDriverBlock>()) {
			hate::Empty<haldls::vx::CHIP_REVISION_STR::SynapseDriverConfig> synapse_driver_config;
			visit_preorder(synapse_driver_config, SynapseDriverOnDLS(on_block, coord), visitor);
		}

		hate::Empty<haldls::vx::CHIP_REVISION_STR::CommonPADIBusConfig> common_padi_bus_config;
		visit_preorder(common_padi_bus_config, coord.toCommonPADIBusConfigOnDLS(), visitor);
		hate::Empty<haldls::vx::CHIP_REVISION_STR::CommonSTPConfig> common_stp_config;
		visit_preorder(common_stp_config, coord.toCommonSTPConfigOnDLS(), visitor);

		for (auto block : iter_all<CapMemBlockOnHemisphere>()) {
			CapMemBlockOnDLS block_on_dls(
			    CapMemBlockOnDLS(block.toEnum() + CapMemBlockOnHemisphere::size * coord.toEnum()));

			hate::Empty<haldls::vx::CHIP_REVISION_STR::CapMemCell> stp_v_charge_0;
			visit_preorder(
			    stp_v_charge_0,
			    CapMemCellOnDLS(CapMemCellOnCapMemBlock::stp_v_charge_0, block_on_dls), visitor);

			hate::Empty<haldls::vx::CHIP_REVISION_STR::CapMemCell> stp_v_charge_1;
			visit_preorder(
			    stp_v_charge_1,
			    CapMemCellOnDLS(CapMemCellOnCapMemBlock::stp_v_charge_1, block_on_dls), visitor);

			hate::Empty<haldls::vx::CHIP_REVISION_STR::CapMemCell> stp_v_recover_0;
			visit_preorder(
			    stp_v_recover_0,
			    CapMemCellOnDLS(CapMemCellOnCapMemBlock::stp_v_recover_0, block_on_dls), visitor);

			hate::Empty<haldls::vx::CHIP_REVISION_STR::CapMemCell> stp_v_recover_1;
			visit_preorder(
			    stp_v_recover_1,
			    CapMemCellOnDLS(CapMemCellOnCapMemBlock::stp_v_recover_1, block_on_dls), visitor);

			hate::Empty<haldls::vx::CHIP_REVISION_STR::CapMemCell> stp_i_bias_comparator;
			visit_preorder(
			    stp_i_bias_comparator,
			    CapMemCellOnDLS(CapMemCellOnCapMemBlock::stp_i_bias_comparator, block_on_dls),
			    visitor);

			hate::Empty<haldls::vx::CHIP_REVISION_STR::CapMemCell> stp_i_ramp;
			visit_preorder(
			    stp_i_ramp, CapMemCellOnDLS(CapMemCellOnCapMemBlock::stp_i_ramp, block_on_dls),
			    visitor);

			hate::Empty<haldls::vx::CHIP_REVISION_STR::CapMemCell> stp_i_calib;
			visit_preorder(
			    stp_i_calib, CapMemCellOnDLS(CapMemCellOnCapMemBlock::stp_i_calib, block_on_dls),
			    visitor);
		}

		hate::Empty<haldls::vx::CHIP_REVISION_STR::CapMemCell> stp_i_bias_readout;
		visit_preorder(
		    stp_i_bias_readout,
		    ((coord == SynapseDriverBlockOnDLS::top) ? CapMemCellOnDLS::stp_ibias_readout_top
		                                             : CapMemCellOnDLS::stp_ibias_readout_bottom),
		    visitor);

		hate::Empty<haldls::vx::CHIP_REVISION_STR::CapMemCell> hagen_i_bias_dac;
		visit_preorder(
		    hagen_i_bias_dac,
		    ((coord == SynapseDriverBlockOnDLS::top) ? CapMemCellOnDLS::hagen_ibias_dac_top
		                                             : CapMemCellOnDLS::hagen_ibias_dac_bottom),
		    visitor);
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<lola::vx::CHIP_REVISION_STR::SynapseDriverBlock::coordinate_type> const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::CHIP_REVISION_STR;

		visitor(coord, config);

		for (auto const on_block : iter_all<SynapseDriverOnSynapseDriverBlock>()) {
			visit_preorder(
			    config.synapse_drivers[on_block], hate::Empty<SynapseDriverOnDLS>{}, visitor);
		}

		visit_preorder(config.padi_bus, hate::Empty<CommonPADIBusConfigOnDLS>{}, visitor);
		visit_preorder(config.stp, hate::Empty<CommonSTPConfigOnDLS>{}, visitor);

		for (auto block : iter_all<CapMemBlockOnHemisphere>()) {
			haldls::vx::CHIP_REVISION_STR::CapMemCell stp_v_charge_0(config.stp_v_charge_0[block]);
			visit_preorder(stp_v_charge_0, hate::Empty<CapMemCellOnDLS>{}, visitor);
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				config.stp_v_charge_0[block] = stp_v_charge_0.get_value();
			}

			haldls::vx::CHIP_REVISION_STR::CapMemCell stp_v_charge_1(config.stp_v_charge_1[block]);
			visit_preorder(stp_v_charge_1, hate::Empty<CapMemCellOnDLS>{}, visitor);
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				config.stp_v_charge_1[block] = stp_v_charge_1.get_value();
			}

			haldls::vx::CHIP_REVISION_STR::CapMemCell stp_v_recover_0(
			    config.stp_v_recover_0[block]);
			visit_preorder(stp_v_recover_0, hate::Empty<CapMemCellOnDLS>{}, visitor);
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				config.stp_v_recover_0[block] = stp_v_recover_0.get_value();
			}

			haldls::vx::CHIP_REVISION_STR::CapMemCell stp_v_recover_1(
			    config.stp_v_recover_1[block]);
			visit_preorder(stp_v_recover_1, hate::Empty<CapMemCellOnDLS>{}, visitor);
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				config.stp_v_recover_1[block] = stp_v_recover_1.get_value();
			}

			haldls::vx::CHIP_REVISION_STR::CapMemCell stp_i_bias_comparator(
			    config.stp_i_bias_comparator[block]);
			visit_preorder(stp_i_bias_comparator, hate::Empty<CapMemCellOnDLS>{}, visitor);
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				config.stp_i_bias_comparator[block] = stp_i_bias_comparator.get_value();
			}

			haldls::vx::CHIP_REVISION_STR::CapMemCell stp_i_ramp(config.stp_i_ramp[block]);
			visit_preorder(stp_i_ramp, hate::Empty<CapMemCellOnDLS>{}, visitor);
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				config.stp_i_ramp[block] = stp_i_ramp.get_value();
			}

			haldls::vx::CHIP_REVISION_STR::CapMemCell stp_i_calib(config.stp_i_calib[block]);
			visit_preorder(stp_i_calib, hate::Empty<CapMemCellOnDLS>{}, visitor);
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				config.stp_i_calib[block] = stp_i_calib.get_value();
			}
		}

		haldls::vx::CHIP_REVISION_STR::CapMemCell stp_i_bias_readout(config.stp_i_bias_readout);
		visit_preorder(stp_i_bias_readout, hate::Empty<CapMemCellOnDLS>{}, visitor);
		if constexpr (!std::is_same<ContainerT, container_type const>::value) {
			config.stp_i_bias_readout = stp_i_bias_readout.get_value();
		}

		haldls::vx::CHIP_REVISION_STR::CapMemCell hagen_i_bias_dac(config.hagen_i_bias_dac);
		visit_preorder(hagen_i_bias_dac, hate::Empty<CapMemCellOnDLS>{}, visitor);
		if constexpr (!std::is_same<ContainerT, container_type const>::value) {
			config.hagen_i_bias_dac = hagen_i_bias_dac.get_value();
		}
	}

	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    lola::vx::CHIP_REVISION_STR::SynapseDriverBlock::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::CHIP_REVISION_STR;

		visitor(coord, config);

		for (auto const on_block : iter_all<SynapseDriverOnSynapseDriverBlock>()) {
			visit_preorder(
			    config.synapse_drivers[on_block], SynapseDriverOnDLS(on_block, coord), visitor);
		}

		visit_preorder(config.padi_bus, coord.toCommonPADIBusConfigOnDLS(), visitor);
		visit_preorder(config.stp, coord.toCommonSTPConfigOnDLS(), visitor);

		for (auto block : iter_all<CapMemBlockOnHemisphere>()) {
			CapMemBlockOnDLS block_on_dls(
			    CapMemBlockOnDLS(block.toEnum() + CapMemBlockOnHemisphere::size * coord.toEnum()));

			haldls::vx::CHIP_REVISION_STR::CapMemCell stp_v_charge_0(config.stp_v_charge_0[block]);
			visit_preorder(
			    stp_v_charge_0,
			    CapMemCellOnDLS(CapMemCellOnCapMemBlock::stp_v_charge_0, block_on_dls), visitor);
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				config.stp_v_charge_0[block] = stp_v_charge_0.get_value();
			}

			haldls::vx::CHIP_REVISION_STR::CapMemCell stp_v_charge_1(config.stp_v_charge_1[block]);
			visit_preorder(
			    stp_v_charge_1,
			    CapMemCellOnDLS(CapMemCellOnCapMemBlock::stp_v_charge_1, block_on_dls), visitor);
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				config.stp_v_charge_1[block] = stp_v_charge_1.get_value();
			}

			haldls::vx::CHIP_REVISION_STR::CapMemCell stp_v_recover_0(
			    config.stp_v_recover_0[block]);
			visit_preorder(
			    stp_v_recover_0,
			    CapMemCellOnDLS(CapMemCellOnCapMemBlock::stp_v_recover_0, block_on_dls), visitor);
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				config.stp_v_recover_0[block] = stp_v_recover_0.get_value();
			}

			haldls::vx::CHIP_REVISION_STR::CapMemCell stp_v_recover_1(
			    config.stp_v_recover_1[block]);
			visit_preorder(
			    stp_v_recover_1,
			    CapMemCellOnDLS(CapMemCellOnCapMemBlock::stp_v_recover_1, block_on_dls), visitor);
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				config.stp_v_recover_1[block] = stp_v_recover_1.get_value();
			}

			haldls::vx::CHIP_REVISION_STR::CapMemCell stp_i_bias_comparator(
			    config.stp_i_bias_comparator[block]);
			visit_preorder(
			    stp_i_bias_comparator,
			    CapMemCellOnDLS(CapMemCellOnCapMemBlock::stp_i_bias_comparator, block_on_dls),
			    visitor);
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				config.stp_i_bias_comparator[block] = stp_i_bias_comparator.get_value();
			}

			haldls::vx::CHIP_REVISION_STR::CapMemCell stp_i_ramp(config.stp_i_ramp[block]);
			visit_preorder(
			    stp_i_ramp, CapMemCellOnDLS(CapMemCellOnCapMemBlock::stp_i_ramp, block_on_dls),
			    visitor);
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				config.stp_i_ramp[block] = stp_i_ramp.get_value();
			}

			haldls::vx::CHIP_REVISION_STR::CapMemCell stp_i_calib(config.stp_i_calib[block]);
			visit_preorder(
			    stp_i_calib, CapMemCellOnDLS(CapMemCellOnCapMemBlock::stp_i_calib, block_on_dls),
			    visitor);
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				config.stp_i_calib[block] = stp_i_calib.get_value();
			}
		}

		haldls::vx::CHIP_REVISION_STR::CapMemCell stp_i_bias_readout(config.stp_i_bias_readout);
		visit_preorder(
		    stp_i_bias_readout,
		    ((coord == SynapseDriverBlockOnDLS::top) ? CapMemCellOnDLS::stp_ibias_readout_top
		                                             : CapMemCellOnDLS::stp_ibias_readout_bottom),
		    visitor);
		if constexpr (!std::is_same<ContainerT, container_type const>::value) {
			config.stp_i_bias_readout = stp_i_bias_readout.get_value();
		}

		haldls::vx::CHIP_REVISION_STR::CapMemCell hagen_i_bias_dac(config.hagen_i_bias_dac);
		visit_preorder(
		    hagen_i_bias_dac,
		    ((coord == SynapseDriverBlockOnDLS::top) ? CapMemCellOnDLS::hagen_ibias_dac_top
		                                             : CapMemCellOnDLS::hagen_ibias_dac_bottom),
		    visitor);
		if constexpr (!std::is_same<ContainerT, container_type const>::value) {
			config.hagen_i_bias_dac = hagen_i_bias_dac.get_value();
		}
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::CHIP_REVISION_STR::SynapseDriverBlock,
    synapse_drivers,
    padi_bus,
    stp,
    stp_v_charge_0,
    stp_v_charge_1,
    stp_v_recover_0,
    stp_v_recover_1,
    stp_i_bias_comparator,
    stp_i_ramp,
    stp_i_calib,
    stp_i_bias_readout,
    hagen_i_bias_dac);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::CHIP_REVISION_STR::SynapseDriverBlock)
