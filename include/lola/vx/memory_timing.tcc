#include "halco/common/iter_all.h"
#include "halco/common/typed_array.h"
#include "lola/vx/cerealization.h"
#include <boost/hana/adapt_struct.hpp>

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::MemoryTiming>
    : public BackendContainerBase<
          lola::vx::MemoryTiming,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::MemoryTiming>
{
	template <typename VisitorT>
	static void call(
	    hate::Empty<lola::vx::MemoryTiming> const& config,
	    lola::vx::MemoryTiming::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using namespace lola::vx;
		using namespace halco::common;

		visitor(coord, config);

		for (auto const coord : iter_all<MemoryTiming::CADCOffset::key_type>()) {
			hate::Empty<MemoryTiming::CADCOffset::value_type> config;
			visit_preorder(config, coord, visitor);
		}
		for (auto const coord : iter_all<MemoryTiming::SynapseDriver::key_type>()) {
			hate::Empty<MemoryTiming::SynapseDriver::value_type> config;
			visit_preorder(config, coord, visitor);
		}
		for (auto const coord : iter_all<MemoryTiming::Neuron::key_type>()) {
			hate::Empty<MemoryTiming::Neuron::value_type> config;
			visit_preorder(config, coord, visitor);
		}
		for (auto const coord : iter_all<MemoryTiming::NeuronBackend::key_type>()) {
			hate::Empty<MemoryTiming::NeuronBackend::value_type> config;
			visit_preorder(config, coord, visitor);
		}
		for (auto const coord : iter_all<MemoryTiming::Synram::key_type>()) {
			hate::Empty<MemoryTiming::Synram::value_type> config;
			visit_preorder(config, coord, visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<lola::vx::MemoryTiming::coordinate_type> const& coord,
	    VisitorT&& visitor)
	{
		using namespace lola::vx;
		using namespace halco::common;

		visitor(coord, config);

		for (auto const coord : iter_all<MemoryTiming::CADCOffset::key_type>()) {
			visit_preorder(
			    config.cadc_offset[coord], hate::Empty<MemoryTiming::CADCOffset::key_type>{},
			    visitor);
		}
		for (auto const coord : iter_all<MemoryTiming::SynapseDriver::key_type>()) {
			visit_preorder(
			    config.synapse_driver[coord], hate::Empty<MemoryTiming::SynapseDriver::key_type>{},
			    visitor);
		}
		for (auto const coord : iter_all<MemoryTiming::Neuron::key_type>()) {
			visit_preorder(
			    config.neuron[coord], hate::Empty<MemoryTiming::Neuron::key_type>{}, visitor);
		}
		for (auto const coord : iter_all<MemoryTiming::NeuronBackend::key_type>()) {
			visit_preorder(
			    config.neuron_backend[coord], hate::Empty<MemoryTiming::NeuronBackend::key_type>{},
			    visitor);
		}
		for (auto const coord : iter_all<MemoryTiming::Synram::key_type>()) {
			visit_preorder(
			    config.synram[coord], hate::Empty<MemoryTiming::Synram::key_type>{}, visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    lola::vx::MemoryTiming::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using namespace lola::vx;
		using namespace halco::common;

		visitor(coord, config);

		for (auto const coord : iter_all<MemoryTiming::CADCOffset::key_type>()) {
			visit_preorder(config.cadc_offset[coord], coord, visitor);
		}
		for (auto const coord : iter_all<MemoryTiming::SynapseDriver::key_type>()) {
			visit_preorder(config.synapse_driver[coord], coord, visitor);
		}
		for (auto const coord : iter_all<MemoryTiming::Neuron::key_type>()) {
			visit_preorder(config.neuron[coord], coord, visitor);
		}
		for (auto const coord : iter_all<MemoryTiming::NeuronBackend::key_type>()) {
			visit_preorder(config.neuron_backend[coord], coord, visitor);
		}
		for (auto const coord : iter_all<MemoryTiming::Synram::key_type>()) {
			visit_preorder(config.synram[coord], coord, visitor);
		}
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::MemoryTiming, cadc_offset, synapse_driver, neuron, neuron_backend, synram);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::MemoryTiming)
