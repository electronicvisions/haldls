#pragma once
#include "halco/common/iter_all.h"
#include "halco/common/typed_array.h"
#include "halco/common/typed_heap_array.h"
#include "haldls/cerealization.h"
#include "haldls/vx/common.h"
#include "haldls/vx/traits.h"
#include "haldls/vx/v2/cadc.h"
#include "hate/visibility.h"
#include "lola/vx/cerealization.h"
#include "lola/vx/genpybind.h"
#include <boost/hana/adapt_struct.hpp>

#include <pybind11/numpy.h>

namespace lola::vx::v2 GENPYBIND_TAG_LOLA_VX_V2 {

class GENPYBIND(visible) CADCSampleRow
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::v2::CADCSampleRowOnDLS coordinate_type;

	typedef haldls::vx::v2::CADCSampleQuad::Value Value GENPYBIND(visible);

	typedef halco::common::typed_heap_array<Value, halco::hicann_dls::vx::v2::SynapseOnSynapseRow>
	    _samples_type GENPYBIND(opaque);

	/** Default constructor. */
	CADCSampleRow() SYMBOL_VISIBLE;

	/** Causal sample value array. */
	_samples_type causal;

	/** Acausal sample value array. */
	_samples_type acausal;

	bool operator==(CADCSampleRow const& other) const SYMBOL_VISIBLE;
	bool operator!=(CADCSampleRow const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CADCSampleRow const& row) SYMBOL_VISIBLE;

private:
	friend class haldls::vx::detail::VisitPreorderImpl<CADCSampleRow>;
};


class GENPYBIND(visible) CADCSamples
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::v2::CADCSamplesOnDLS coordinate_type;

	typedef haldls::vx::v2::CADCSampleQuad::Value Value GENPYBIND(visible);

	typedef halco::common::typed_heap_array<
	    halco::common::typed_array<Value, halco::hicann_dls::vx::v2::SynapseOnSynapseRow>,
	    halco::hicann_dls::vx::v2::SynramOnDLS>
	    _samples_type GENPYBIND(opaque);

	/** Default constructor. */
	CADCSamples() SYMBOL_VISIBLE;

	_samples_type causal;
	_samples_type acausal;

	bool operator==(CADCSamples const& other) const SYMBOL_VISIBLE;
	bool operator!=(CADCSamples const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CADCSamples const& row) SYMBOL_VISIBLE;

private:
	friend class haldls::vx::detail::VisitPreorderImpl<CADCSamples>;
};

} // namespace lola::vx

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::v2::CADCSampleRow>
    : public BackendContainerBase<lola::vx::v2::CADCSampleRow, fisch::vx::Omnibus>
{};

template <>
struct BackendContainerTrait<lola::vx::v2::CADCSamples>
    : public BackendContainerBase<lola::vx::v2::CADCSamples, fisch::vx::Omnibus>
{};

template <>
struct VisitPreorderImpl<lola::vx::v2::CADCSampleRow>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    lola::vx::v2::CADCSampleRow::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v2;

		visitor(coord, config);

		// only do something on read
		if constexpr (!std::is_same<ContainerT, lola::vx::v2::CADCSampleRow const>::value) {
			// trigger ADC sampling by reading one quad of causal channels
			v2::CADCSampleQuad quad_config_trigger;
			CADCSampleQuadOnDLS quad_coord_trigger(
			    CADCSampleQuadOnSynram(
			        SynapseQuadOnSynram(
			            SynapseQuadColumnOnDLS(SynapseQuadColumnOnDLS::min),
			            coord.toSynapseRowOnSynram()),
			        CADCChannelType::causal, CADCReadoutType::trigger_read),
			    coord.toSynramOnDLS());
			visit_preorder(quad_config_trigger, quad_coord_trigger, visitor);

			// Use results of triggered read: write into result array
			for (auto const entry : iter_all<EntryOnQuad>()) {
				SynapseOnSynapseRow syn_on_row(entry, SynapseQuadColumnOnDLS(0));
				config.causal[syn_on_row] = quad_config_trigger.get_sample(entry);
			}

			// buffered read of remaining causal channel quads:
			// This reads back the results that were acquired during the last
			// triggered measurement, which was just done above.
			for (size_t i = SynapseQuadColumnOnDLS::min + 1; i <= SynapseQuadColumnOnDLS::max;
			     ++i) {
				auto quad_column = SynapseQuadColumnOnDLS(i);

				CADCSampleQuadOnDLS quad_coord(
				    CADCSampleQuadOnSynram(
				        SynapseQuadOnSynram(quad_column, coord.toSynapseRowOnSynram()),
				        CADCChannelType::causal, CADCReadoutType::buffered),
				    coord.toSynramOnDLS());
				v2::CADCSampleQuad quad_config;
				visit_preorder(quad_config, quad_coord, visitor);
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow syn_on_row(syn, quad_column);
					config.causal[syn_on_row] = quad_config.get_sample(syn);
				}
			}

			// buffered read of acausal channel quads
			for (auto quad_column : iter_all<SynapseQuadColumnOnDLS>()) {
				CADCSampleQuadOnDLS quad_coord(
				    CADCSampleQuadOnSynram(
				        SynapseQuadOnSynram(quad_column, coord.toSynapseRowOnSynram()),
				        CADCChannelType::acausal, CADCReadoutType::buffered),
				    coord.toSynramOnDLS());
				v2::CADCSampleQuad quad_config;
				visit_preorder(quad_config, quad_coord, visitor);
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow syn_on_row(syn, quad_column);
					config.acausal[syn_on_row] = quad_config.get_sample(syn);
				}
			}
		}
	}
};

template <>
struct VisitPreorderImpl<lola::vx::v2::CADCSamples>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    lola::vx::v2::CADCSamples::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v2;

		visitor(coord, config);

		// only do something on read
		if constexpr (!std::is_same<ContainerT, lola::vx::v2::CADCSamples const>::value) {
			auto const trigger = [&](auto& values, auto const& loc) {
				v2::CADCSampleQuad quad_config_trigger;
				CADCSampleQuadOnDLS quad_coord_trigger_top(
				    CADCSampleQuadOnSynram(
				        SynapseQuadOnSynram(
				            SynapseQuadColumnOnDLS(SynapseQuadColumnOnDLS::min),
				            SynapseRowOnSynram()),
				        CADCChannelType::causal, CADCReadoutType::trigger_read),
				    loc);
				visit_preorder(quad_config_trigger, quad_coord_trigger_top, visitor);

				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow syn_on_row(
					    syn, SynapseQuadColumnOnDLS(SynapseQuadColumnOnDLS::min));
					values[loc][syn_on_row] = quad_config_trigger.get_sample(syn);
				}
			};

			// trigger ADC sampling in top synram by reading one quad of causal channels
			// immediately trigger ADC sampling in bottom synram as well
			trigger(config.causal, SynramOnDLS::top);
			trigger(config.causal, SynramOnDLS::bottom);

			auto const buffered = [&](auto& values, auto const& type, auto const& loc) {
				for (auto quad_column : iter_all<SynapseQuadColumnOnDLS>()) {
					// Skip causal quad 0 (handled above as triggered read)
					if (type == CADCChannelType::causal &&
					    quad_column == SynapseQuadColumnOnDLS::min)
						continue;

					CADCSampleQuadOnDLS quad_coord(
					    CADCSampleQuadOnSynram(
					        SynapseQuadOnSynram(quad_column, SynapseRowOnSynram()), type,
					        CADCReadoutType::buffered),
					    loc);
					v2::CADCSampleQuad quad_config;
					visit_preorder(quad_config, quad_coord, visitor);
					for (auto const syn : iter_all<EntryOnQuad>()) {
						SynapseOnSynapseRow syn_on_row(syn, quad_column);
						values[loc][syn_on_row] = quad_config.get_sample(syn);
					}
				}
			};

			// buffered reads of remaining quads
			buffered(config.causal, CADCChannelType::causal, SynramOnDLS::top);
			buffered(config.acausal, CADCChannelType::acausal, SynramOnDLS::top);
			buffered(config.causal, CADCChannelType::causal, SynramOnDLS::bottom);
			buffered(config.acausal, CADCChannelType::acausal, SynramOnDLS::bottom);
		}
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(lola::vx::v2::CADCSampleRow, causal, acausal);
BOOST_HANA_ADAPT_STRUCT(lola::vx::v2::CADCSamples, causal, acausal);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::CADCSampleRow)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::CADCSamples)
