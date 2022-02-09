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
	template <typename ContainerT, typename VisitorT>
	static void call(
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
