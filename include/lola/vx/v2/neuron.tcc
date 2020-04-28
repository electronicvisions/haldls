#include "haldls/cerealization.h"
#include "haldls/vx/common.h"
#include "haldls/vx/neuron.h"
#include "lola/vx/cerealization.h"
#include <boost/hana/adapt_struct.hpp>

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::v2::AtomicNeuron>
    : public BackendContainerBase<
          lola::vx::v2::AtomicNeuron,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v2::AtomicNeuron>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> const& config,
	    lola::vx::v2::AtomicNeuron::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v2;

		visitor(coord, config);

		auto capmem_block = coord.toCapMemBlockOnDLS();
		auto capmem_column = coord.toCapMemColumnOnCapMemBlock();
		auto to_capmem_cell = [capmem_block, capmem_column](CapMemRowOnCapMemBlock row) {
			return CapMemCellOnDLS(CapMemCellOnCapMemBlock(capmem_column, row), capmem_block);
		};

		hate::Empty<haldls::vx::v2::NeuronConfig> neuron_config;
		visit_preorder(neuron_config, coord.toNeuronConfigOnDLS(), visitor);

		hate::Empty<haldls::vx::v2::CapMemCell> cell_i_bias_synin_exc_tau;
		hate::Empty<haldls::vx::v2::CapMemCell> cell_i_bias_synin_exc_drop;
		hate::Empty<haldls::vx::v2::CapMemCell> cell_i_bias_synin_exc_shift;
		hate::Empty<haldls::vx::v2::CapMemCell> cell_i_bias_synin_exc_gm;
		hate::Empty<haldls::vx::v2::CapMemCell> cell_i_bias_synin_inh_tau;
		hate::Empty<haldls::vx::v2::CapMemCell> cell_i_bias_synin_inh_drop;
		hate::Empty<haldls::vx::v2::CapMemCell> cell_i_bias_synin_inh_shift;
		hate::Empty<haldls::vx::v2::CapMemCell> cell_i_bias_synin_inh_gm;
		hate::Empty<haldls::vx::v2::CapMemCell> cell_v_leak;
		hate::Empty<haldls::vx::v2::CapMemCell> cell_i_bias_leak;
		hate::Empty<haldls::vx::v2::CapMemCell> cell_v_reset;
		hate::Empty<haldls::vx::v2::CapMemCell> cell_i_bias_reset;
		hate::Empty<haldls::vx::v2::CapMemCell> cell_v_threshold;
		hate::Empty<haldls::vx::v2::CapMemCell> cell_i_offset;
		hate::Empty<haldls::vx::v2::CapMemCell> cell_v_ref_adapt;
		hate::Empty<haldls::vx::v2::CapMemCell> cell_v_leak_adapt;
		hate::Empty<haldls::vx::v2::CapMemCell> cell_i_bias_adapt_tau;
		hate::Empty<haldls::vx::v2::CapMemCell> cell_i_bias_adapt_a;
		hate::Empty<haldls::vx::v2::CapMemCell> cell_i_bias_adapt_b;
		hate::Empty<haldls::vx::v2::CapMemCell> cell_v_exp;
		hate::Empty<haldls::vx::v2::CapMemCell> cell_i_bias_exp;
		hate::Empty<haldls::vx::v2::CapMemCell> cell_i_bias_nmda;

		visit_preorder(
		    cell_i_bias_synin_exc_tau, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_synin_exc_tau),
		    visitor);
		visit_preorder(
		    cell_i_bias_synin_exc_drop,
		    to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_synin_exc_drop), visitor);
		visit_preorder(
		    cell_i_bias_synin_exc_shift,
		    to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_synin_exc_shift), visitor);
		visit_preorder(
		    cell_i_bias_synin_exc_gm, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_synin_exc_gm),
		    visitor);
		visit_preorder(
		    cell_i_bias_synin_inh_tau, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_synin_inh_tau),
		    visitor);
		visit_preorder(
		    cell_i_bias_synin_inh_drop,
		    to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_synin_inh_drop), visitor);
		visit_preorder(
		    cell_i_bias_synin_inh_shift,
		    to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_synin_inh_shift), visitor);
		visit_preorder(
		    cell_i_bias_synin_inh_gm, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_synin_inh_gm),
		    visitor);
		visit_preorder(cell_v_leak, to_capmem_cell(CapMemRowOnCapMemBlock::v_leak), visitor);
		visit_preorder(
		    cell_i_bias_leak, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_leak), visitor);
		visit_preorder(cell_v_reset, to_capmem_cell(CapMemRowOnCapMemBlock::v_reset), visitor);
		visit_preorder(
		    cell_i_bias_reset, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_reset), visitor);
		visit_preorder(
		    cell_v_threshold, to_capmem_cell(CapMemRowOnCapMemBlock::v_threshold), visitor);
		visit_preorder(
		    cell_i_offset, to_capmem_cell(CapMemRowOnCapMemBlock::i_mem_offset), visitor);
		visit_preorder(
		    cell_v_leak_adapt, to_capmem_cell(CapMemRowOnCapMemBlock::v_adapt_leak), visitor);
		visit_preorder(
		    cell_i_bias_adapt_tau, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_adapt_tau),
		    visitor);
		visit_preorder(
		    cell_i_bias_adapt_a, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_adapt_a), visitor);
		visit_preorder(
		    cell_i_bias_adapt_b, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_adapt_b), visitor);
		visit_preorder(
		    cell_v_ref_adapt, to_capmem_cell(CapMemRowOnCapMemBlock::v_adapt_ref), visitor);
		visit_preorder(cell_v_exp, to_capmem_cell(CapMemRowOnCapMemBlock::v_exp), visitor);
		visit_preorder(
		    cell_i_bias_exp, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_exp), visitor);
		visit_preorder(
		    cell_i_bias_nmda, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_nmda), visitor);

		hate::Empty<haldls::vx::v2::NeuronBackendConfig> neuron_backend_config;
		visit_preorder(neuron_backend_config, coord.toNeuronBackendConfigOnDLS(), visitor);
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<lola::vx::v2::AtomicNeuron::coordinate_type> const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v2;

		visitor(coord, config);

		auto neuron_config = static_cast<haldls::vx::v2::NeuronConfig>(config);
		visit_preorder(neuron_config, hate::Empty<NeuronConfigOnDLS>{}, visitor);

		haldls::vx::v2::CapMemCell cell_i_bias_synin_exc_tau(config.excitatory_input.i_bias_tau);
		haldls::vx::v2::CapMemCell cell_i_bias_synin_exc_drop(config.excitatory_input.i_drop_input);
		haldls::vx::v2::CapMemCell cell_i_bias_synin_exc_shift(
		    config.excitatory_input.i_shift_reference);
		haldls::vx::v2::CapMemCell cell_i_bias_synin_exc_gm(config.excitatory_input.i_bias_gm);
		haldls::vx::v2::CapMemCell cell_i_bias_synin_inh_tau(config.inhibitory_input.i_bias_tau);
		haldls::vx::v2::CapMemCell cell_i_bias_synin_inh_drop(config.inhibitory_input.i_drop_input);
		haldls::vx::v2::CapMemCell cell_i_bias_synin_inh_shift(
		    config.inhibitory_input.i_shift_reference);
		haldls::vx::v2::CapMemCell cell_i_bias_synin_inh_gm(config.inhibitory_input.i_bias_gm);
		haldls::vx::v2::CapMemCell cell_v_leak(config.leak.v_leak);
		haldls::vx::v2::CapMemCell cell_i_bias_leak(config.leak.i_bias);
		haldls::vx::v2::CapMemCell cell_v_reset(config.reset.v_reset);
		haldls::vx::v2::CapMemCell cell_i_bias_reset(config.reset.i_bias);
		haldls::vx::v2::CapMemCell cell_v_threshold(config.threshold.v_threshold);
		haldls::vx::v2::CapMemCell cell_i_offset(config.constant_current.i_offset);
		haldls::vx::v2::CapMemCell cell_v_ref_adapt(config.adaptation.v_ref);
		haldls::vx::v2::CapMemCell cell_v_leak_adapt(config.adaptation.v_leak);
		haldls::vx::v2::CapMemCell cell_i_bias_adapt_tau(config.adaptation.i_bias_tau);
		haldls::vx::v2::CapMemCell cell_i_bias_adapt_a(config.adaptation.i_bias_a);
		haldls::vx::v2::CapMemCell cell_i_bias_adapt_b(config.adaptation.i_bias_b);
		haldls::vx::v2::CapMemCell cell_v_exp(config.exponential.v_exp);
		haldls::vx::v2::CapMemCell cell_i_bias_exp(config.exponential.i_bias);
		haldls::vx::v2::CapMemCell cell_i_bias_nmda(config.multicompartment.i_bias_nmda);

		visit_preorder(cell_i_bias_synin_exc_tau, hate::Empty<CapMemCellOnDLS>{}, visitor);
		visit_preorder(cell_i_bias_synin_exc_drop, hate::Empty<CapMemCellOnDLS>{}, visitor);
		visit_preorder(cell_i_bias_synin_exc_shift, hate::Empty<CapMemCellOnDLS>{}, visitor);
		visit_preorder(cell_i_bias_synin_exc_gm, hate::Empty<CapMemCellOnDLS>{}, visitor);
		visit_preorder(cell_i_bias_synin_inh_tau, hate::Empty<CapMemCellOnDLS>{}, visitor);
		visit_preorder(cell_i_bias_synin_inh_drop, hate::Empty<CapMemCellOnDLS>{}, visitor);
		visit_preorder(cell_i_bias_synin_inh_shift, hate::Empty<CapMemCellOnDLS>{}, visitor);
		visit_preorder(cell_i_bias_synin_inh_gm, hate::Empty<CapMemCellOnDLS>{}, visitor);
		visit_preorder(cell_v_leak, hate::Empty<CapMemCellOnDLS>{}, visitor);
		visit_preorder(cell_i_bias_leak, hate::Empty<CapMemCellOnDLS>{}, visitor);
		visit_preorder(cell_v_reset, hate::Empty<CapMemCellOnDLS>{}, visitor);
		visit_preorder(cell_i_bias_reset, hate::Empty<CapMemCellOnDLS>{}, visitor);
		visit_preorder(cell_v_threshold, hate::Empty<CapMemCellOnDLS>{}, visitor);
		visit_preorder(cell_i_offset, hate::Empty<CapMemCellOnDLS>{}, visitor);
		visit_preorder(cell_v_leak_adapt, hate::Empty<CapMemCellOnDLS>{}, visitor);
		visit_preorder(cell_i_bias_adapt_tau, hate::Empty<CapMemCellOnDLS>{}, visitor);
		visit_preorder(cell_i_bias_adapt_a, hate::Empty<CapMemCellOnDLS>{}, visitor);
		visit_preorder(cell_i_bias_adapt_b, hate::Empty<CapMemCellOnDLS>{}, visitor);
		visit_preorder(cell_v_ref_adapt, hate::Empty<CapMemCellOnDLS>{}, visitor);
		visit_preorder(cell_v_exp, hate::Empty<CapMemCellOnDLS>{}, visitor);
		visit_preorder(cell_i_bias_exp, hate::Empty<CapMemCellOnDLS>{}, visitor);
		visit_preorder(cell_i_bias_nmda, hate::Empty<CapMemCellOnDLS>{}, visitor);

		auto neuron_backend_config = static_cast<haldls::vx::v2::NeuronBackendConfig>(config);
		visit_preorder(neuron_backend_config, hate::Empty<NeuronBackendConfigOnDLS>{}, visitor);

		if constexpr (!std::is_same<ContainerT, lola::vx::v2::AtomicNeuron const>::value) {
			config.set_from(neuron_config);

			config.set_from(neuron_backend_config);

			config.excitatory_input.i_bias_gm = cell_i_bias_synin_exc_gm.get_value();
			config.excitatory_input.i_bias_tau = cell_i_bias_synin_exc_tau.get_value();
			config.excitatory_input.i_drop_input = cell_i_bias_synin_exc_drop.get_value();
			config.excitatory_input.i_shift_reference = cell_i_bias_synin_exc_shift.get_value();
			config.inhibitory_input.i_bias_gm = cell_i_bias_synin_inh_gm.get_value();
			config.inhibitory_input.i_bias_tau = cell_i_bias_synin_inh_tau.get_value();
			config.inhibitory_input.i_drop_input = cell_i_bias_synin_inh_drop.get_value();
			config.inhibitory_input.i_shift_reference = cell_i_bias_synin_inh_shift.get_value();
			config.leak.v_leak = cell_v_leak.get_value();
			config.leak.i_bias = cell_i_bias_leak.get_value();
			config.reset.v_reset = cell_v_reset.get_value();
			config.reset.i_bias = cell_i_bias_reset.get_value();
			config.threshold.v_threshold = cell_v_threshold.get_value();
			config.constant_current.i_offset = cell_i_offset.get_value();
			config.adaptation.v_leak = cell_v_leak_adapt.get_value();
			config.adaptation.v_ref = cell_v_ref_adapt.get_value();
			config.adaptation.i_bias_tau = cell_i_bias_adapt_tau.get_value();
			config.adaptation.i_bias_a = cell_i_bias_adapt_a.get_value();
			config.adaptation.i_bias_b = cell_i_bias_adapt_b.get_value();
			config.exponential.v_exp = cell_v_exp.get_value();
			config.exponential.i_bias = cell_i_bias_exp.get_value();
			config.multicompartment.i_bias_nmda = cell_i_bias_nmda.get_value();
		}
	}

	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    lola::vx::v2::AtomicNeuron::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v2;

		visitor(coord, config);

		auto capmem_block = coord.toCapMemBlockOnDLS();
		auto capmem_column = coord.toCapMemColumnOnCapMemBlock();
		auto to_capmem_cell = [capmem_block, capmem_column](CapMemRowOnCapMemBlock row) {
			return CapMemCellOnDLS(CapMemCellOnCapMemBlock(capmem_column, row), capmem_block);
		};

		auto neuron_config = static_cast<haldls::vx::v2::NeuronConfig>(config);
		visit_preorder(neuron_config, coord.toNeuronConfigOnDLS(), visitor);

		haldls::vx::v2::CapMemCell cell_i_bias_synin_exc_tau(config.excitatory_input.i_bias_tau);
		haldls::vx::v2::CapMemCell cell_i_bias_synin_exc_drop(config.excitatory_input.i_drop_input);
		haldls::vx::v2::CapMemCell cell_i_bias_synin_exc_shift(
		    config.excitatory_input.i_shift_reference);
		haldls::vx::v2::CapMemCell cell_i_bias_synin_exc_gm(config.excitatory_input.i_bias_gm);
		haldls::vx::v2::CapMemCell cell_i_bias_synin_inh_tau(config.inhibitory_input.i_bias_tau);
		haldls::vx::v2::CapMemCell cell_i_bias_synin_inh_drop(config.inhibitory_input.i_drop_input);
		haldls::vx::v2::CapMemCell cell_i_bias_synin_inh_shift(
		    config.inhibitory_input.i_shift_reference);
		haldls::vx::v2::CapMemCell cell_i_bias_synin_inh_gm(config.inhibitory_input.i_bias_gm);
		haldls::vx::v2::CapMemCell cell_v_leak(config.leak.v_leak);
		haldls::vx::v2::CapMemCell cell_i_bias_leak(config.leak.i_bias);
		haldls::vx::v2::CapMemCell cell_v_reset(config.reset.v_reset);
		haldls::vx::v2::CapMemCell cell_i_bias_reset(config.reset.i_bias);
		haldls::vx::v2::CapMemCell cell_v_threshold(config.threshold.v_threshold);
		haldls::vx::v2::CapMemCell cell_i_offset(config.constant_current.i_offset);
		haldls::vx::v2::CapMemCell cell_v_ref_adapt(config.adaptation.v_ref);
		haldls::vx::v2::CapMemCell cell_v_leak_adapt(config.adaptation.v_leak);
		haldls::vx::v2::CapMemCell cell_i_bias_adapt_tau(config.adaptation.i_bias_tau);
		haldls::vx::v2::CapMemCell cell_i_bias_adapt_a(config.adaptation.i_bias_a);
		haldls::vx::v2::CapMemCell cell_i_bias_adapt_b(config.adaptation.i_bias_b);
		haldls::vx::v2::CapMemCell cell_v_exp(config.exponential.v_exp);
		haldls::vx::v2::CapMemCell cell_i_bias_exp(config.exponential.i_bias);
		haldls::vx::v2::CapMemCell cell_i_bias_nmda(config.multicompartment.i_bias_nmda);

		visit_preorder(
		    cell_i_bias_synin_exc_tau, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_synin_exc_tau),
		    visitor);
		visit_preorder(
		    cell_i_bias_synin_exc_drop,
		    to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_synin_exc_drop), visitor);
		visit_preorder(
		    cell_i_bias_synin_exc_shift,
		    to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_synin_exc_shift), visitor);
		visit_preorder(
		    cell_i_bias_synin_exc_gm, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_synin_exc_gm),
		    visitor);
		visit_preorder(
		    cell_i_bias_synin_inh_tau, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_synin_inh_tau),
		    visitor);
		visit_preorder(
		    cell_i_bias_synin_inh_drop,
		    to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_synin_inh_drop), visitor);
		visit_preorder(
		    cell_i_bias_synin_inh_shift,
		    to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_synin_inh_shift), visitor);
		visit_preorder(
		    cell_i_bias_synin_inh_gm, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_synin_inh_gm),
		    visitor);
		visit_preorder(cell_v_leak, to_capmem_cell(CapMemRowOnCapMemBlock::v_leak), visitor);
		visit_preorder(
		    cell_i_bias_leak, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_leak), visitor);
		visit_preorder(cell_v_reset, to_capmem_cell(CapMemRowOnCapMemBlock::v_reset), visitor);
		visit_preorder(
		    cell_i_bias_reset, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_reset), visitor);
		visit_preorder(
		    cell_v_threshold, to_capmem_cell(CapMemRowOnCapMemBlock::v_threshold), visitor);
		visit_preorder(
		    cell_i_offset, to_capmem_cell(CapMemRowOnCapMemBlock::i_mem_offset), visitor);
		visit_preorder(
		    cell_v_leak_adapt, to_capmem_cell(CapMemRowOnCapMemBlock::v_adapt_leak), visitor);
		visit_preorder(
		    cell_i_bias_adapt_tau, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_adapt_tau),
		    visitor);
		visit_preorder(
		    cell_i_bias_adapt_a, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_adapt_a), visitor);
		visit_preorder(
		    cell_i_bias_adapt_b, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_adapt_b), visitor);
		visit_preorder(
		    cell_v_ref_adapt, to_capmem_cell(CapMemRowOnCapMemBlock::v_adapt_ref), visitor);
		visit_preorder(cell_v_exp, to_capmem_cell(CapMemRowOnCapMemBlock::v_exp), visitor);
		visit_preorder(
		    cell_i_bias_exp, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_exp), visitor);
		visit_preorder(
		    cell_i_bias_nmda, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_nmda), visitor);

		auto neuron_backend_config = static_cast<haldls::vx::v2::NeuronBackendConfig>(config);
		visit_preorder(neuron_backend_config, coord.toNeuronBackendConfigOnDLS(), visitor);

		if constexpr (!std::is_same<ContainerT, lola::vx::v2::AtomicNeuron const>::value) {
			config.set_from(neuron_config);

			config.set_from(neuron_backend_config);

			config.excitatory_input.i_bias_gm = cell_i_bias_synin_exc_gm.get_value();
			config.excitatory_input.i_bias_tau = cell_i_bias_synin_exc_tau.get_value();
			config.excitatory_input.i_drop_input = cell_i_bias_synin_exc_drop.get_value();
			config.excitatory_input.i_shift_reference = cell_i_bias_synin_exc_shift.get_value();
			config.inhibitory_input.i_bias_gm = cell_i_bias_synin_inh_gm.get_value();
			config.inhibitory_input.i_bias_tau = cell_i_bias_synin_inh_tau.get_value();
			config.inhibitory_input.i_drop_input = cell_i_bias_synin_inh_drop.get_value();
			config.inhibitory_input.i_shift_reference = cell_i_bias_synin_inh_shift.get_value();
			config.leak.v_leak = cell_v_leak.get_value();
			config.leak.i_bias = cell_i_bias_leak.get_value();
			config.reset.v_reset = cell_v_reset.get_value();
			config.reset.i_bias = cell_i_bias_reset.get_value();
			config.threshold.v_threshold = cell_v_threshold.get_value();
			config.constant_current.i_offset = cell_i_offset.get_value();
			config.adaptation.v_leak = cell_v_leak_adapt.get_value();
			config.adaptation.v_ref = cell_v_ref_adapt.get_value();
			config.adaptation.i_bias_tau = cell_i_bias_adapt_tau.get_value();
			config.adaptation.i_bias_a = cell_i_bias_adapt_a.get_value();
			config.adaptation.i_bias_b = cell_i_bias_adapt_b.get_value();
			config.exponential.v_exp = cell_v_exp.get_value();
			config.exponential.i_bias = cell_i_bias_exp.get_value();
			config.multicompartment.i_bias_nmda = cell_i_bias_nmda.get_value();
		}
	}
};


template <>
struct BackendContainerTrait<lola::vx::v2::NeuronBlock>
    : public BackendContainerBase<
          lola::vx::v2::NeuronBlock,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v2::NeuronBlock>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> const& config,
	    lola::vx::v2::NeuronBlock::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v2;

		visitor(coord, config);

		for (auto const an : iter_all<AtomicNeuronOnDLS>()) {
			hate::Empty<lola::vx::v2::AtomicNeuron> atomic_neuron;
			visit_preorder(atomic_neuron, an, visitor);
		}

		for (auto const block : iter_all<CommonNeuronBackendConfigOnDLS>()) {
			hate::Empty<haldls::vx::v2::CommonNeuronBackendConfig> common_neuron_backend_config;
			visit_preorder(common_neuron_backend_config, block, visitor);
		}

		for (auto const row : iter_all<ColumnCurrentRowOnDLS>()) {
			hate::Empty<lola::vx::v2::ColumnCurrentRow> column_current_row;
			visit_preorder(column_current_row, row, visitor);
		}

		for (auto const block : iter_all<CapMemBlockOnDLS>()) {
			hate::Empty<haldls::vx::v2::CapMemCell> v_bias_casc_n;
			hate::Empty<haldls::vx::v2::CapMemCell> i_bias_readout_amp;
			hate::Empty<haldls::vx::v2::CapMemCell> i_bias_leak_source_follower;
			hate::Empty<haldls::vx::v2::CapMemCell> i_bias_threshold_comparator;
			visit_preorder(
			    v_bias_casc_n,
			    CapMemCellOnDLS(CapMemCellOnCapMemBlock::neuron_v_bias_casc_n, block), visitor);
			visit_preorder(
			    i_bias_readout_amp,
			    CapMemCellOnDLS(CapMemCellOnCapMemBlock::neuron_i_bias_readout_amp, block),
			    visitor);
			visit_preorder(
			    i_bias_leak_source_follower,
			    CapMemCellOnDLS(CapMemCellOnCapMemBlock::neuron_i_bias_leak_source_follower, block),
			    visitor);
			visit_preorder(
			    i_bias_threshold_comparator,
			    CapMemCellOnDLS(CapMemCellOnCapMemBlock::neuron_i_bias_spike_comparator, block),
			    visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<lola::vx::v2::NeuronBlock::coordinate_type> const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v2;

		visitor(coord, config);

		for (auto const an : iter_all<AtomicNeuronOnDLS>()) {
			visit_preorder(config.atomic_neurons[an], hate::Empty<AtomicNeuronOnDLS>{}, visitor);
		}

		for (auto const block : iter_all<CommonNeuronBackendConfigOnDLS>()) {
			visit_preorder(
			    config.backends[block], hate::Empty<CommonNeuronBackendConfigOnDLS>{}, visitor);
		}

		for (auto const row : iter_all<ColumnCurrentRowOnDLS>()) {
			visit_preorder(config.current_rows[row], hate::Empty<ColumnCurrentRowOnDLS>{}, visitor);
		}

		for (auto const block : iter_all<CapMemBlockOnDLS>()) {
			haldls::vx::v2::CapMemCell v_bias_casc_n(config.v_bias_casc_n[block]);
			haldls::vx::v2::CapMemCell i_bias_readout_amp(config.i_bias_readout_amp[block]);
			haldls::vx::v2::CapMemCell i_bias_leak_source_follower(
			    config.i_bias_leak_source_follower[block]);
			haldls::vx::v2::CapMemCell i_bias_threshold_comparator(
			    config.i_bias_threshold_comparator[block]);
			visit_preorder(v_bias_casc_n, hate::Empty<CapMemCellOnDLS>{}, visitor);
			visit_preorder(i_bias_readout_amp, hate::Empty<CapMemCellOnDLS>{}, visitor);
			visit_preorder(i_bias_leak_source_follower, hate::Empty<CapMemCellOnDLS>{}, visitor);
			visit_preorder(i_bias_threshold_comparator, hate::Empty<CapMemCellOnDLS>{}, visitor);
			if constexpr (!std::is_same<ContainerT, lola::vx::v2::NeuronBlock const>::value) {
				config.v_bias_casc_n[block] = v_bias_casc_n.get_value();
				config.i_bias_readout_amp[block] = i_bias_readout_amp.get_value();
				config.i_bias_leak_source_follower[block] = i_bias_leak_source_follower.get_value();
				config.i_bias_threshold_comparator[block] = i_bias_threshold_comparator.get_value();
			}
		}
	}

	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    lola::vx::v2::NeuronBlock::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v2;

		visitor(coord, config);

		for (auto const an : iter_all<AtomicNeuronOnDLS>()) {
			visit_preorder(config.atomic_neurons[an], an, visitor);
		}

		for (auto const block : iter_all<CommonNeuronBackendConfigOnDLS>()) {
			visit_preorder(config.backends[block], block, visitor);
		}

		for (auto const row : iter_all<ColumnCurrentRowOnDLS>()) {
			visit_preorder(config.current_rows[row], row, visitor);
		}

		for (auto const block : iter_all<CapMemBlockOnDLS>()) {
			haldls::vx::v2::CapMemCell v_bias_casc_n(config.v_bias_casc_n[block]);
			haldls::vx::v2::CapMemCell i_bias_readout_amp(config.i_bias_readout_amp[block]);
			haldls::vx::v2::CapMemCell i_bias_leak_source_follower(
			    config.i_bias_leak_source_follower[block]);
			haldls::vx::v2::CapMemCell i_bias_threshold_comparator(
			    config.i_bias_threshold_comparator[block]);
			visit_preorder(
			    v_bias_casc_n,
			    CapMemCellOnDLS(CapMemCellOnCapMemBlock::neuron_v_bias_casc_n, block), visitor);
			visit_preorder(
			    i_bias_readout_amp,
			    CapMemCellOnDLS(CapMemCellOnCapMemBlock::neuron_i_bias_readout_amp, block),
			    visitor);
			visit_preorder(
			    i_bias_leak_source_follower,
			    CapMemCellOnDLS(CapMemCellOnCapMemBlock::neuron_i_bias_leak_source_follower, block),
			    visitor);
			visit_preorder(
			    i_bias_threshold_comparator,
			    CapMemCellOnDLS(CapMemCellOnCapMemBlock::neuron_i_bias_spike_comparator, block),
			    visitor);
			if constexpr (!std::is_same<ContainerT, lola::vx::v2::NeuronBlock const>::value) {
				config.v_bias_casc_n[block] = v_bias_casc_n.get_value();
				config.i_bias_readout_amp[block] = i_bias_readout_amp.get_value();
				config.i_bias_leak_source_follower[block] = i_bias_leak_source_follower.get_value();
				config.i_bias_threshold_comparator[block] = i_bias_threshold_comparator.get_value();
			}
		}
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v2::AtomicNeuron::SynapticInput,
    enable,
    i_bias_tau,
    i_drop_input,
    i_shift_reference,
    i_bias_gm,
    enable_small_capacitor,
    enable_high_resistance);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v2::AtomicNeuron::Leak,
    v_leak,
    i_bias,
    enable_degeneration,
    enable_division,
    enable_multiplication);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v2::AtomicNeuron::Reset,
    v_reset,
    i_bias,
    enable_degeneration,
    enable_division,
    enable_multiplication);

BOOST_HANA_ADAPT_STRUCT(lola::vx::v2::AtomicNeuron::Threshold, enable, v_threshold);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v2::AtomicNeuron::Multicompartment,
    connect_soma,
    connect_soma_right,
    connect_right,
    connect_vertical,
    enable_conductance,
    enable_conductance_division,
    enable_conductance_multiplication,
    i_bias_nmda);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v2::AtomicNeuron::ConstantCurrent, enable, i_offset, type);

BOOST_HANA_ADAPT_STRUCT(lola::vx::v2::AtomicNeuron::MembraneCapacitance, capacitance);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v2::AtomicNeuron::Adaptation,
    enable,
    enable_pulse,
    invert_a,
    invert_b,
    v_ref,
    i_bias_tau,
    i_bias_a,
    i_bias_b,
    v_leak);

BOOST_HANA_ADAPT_STRUCT(lola::vx::v2::AtomicNeuron::Exponential, enable, v_exp, i_bias);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v2::AtomicNeuron::Readout,
    enable_buffered_access,
    enable_amplifier,
    enable_unbuffered_access,
    source);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v2::AtomicNeuron::EventRouting,
    analog_output,
    enable_digital,
    enable_bypass_excitatory,
    enable_bypass_inhibitory,
    address,
    enable_post_overwrite);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v2::AtomicNeuron::RefractoryPeriod,
    input_clock,
    reset_holdoff,
    refractory_time,
    enable_pause);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v2::AtomicNeuron::Bayesian,
    enable,
    connect_fire_vertical,
    connect_fire_to_right,
    connect_fire_from_right,
    enable_master,
    enable_slave,
    enable_0,
    enable_1);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v2::AtomicNeuron,
    excitatory_input,
    inhibitory_input,
    leak,
    reset,
    threshold,
    multicompartment,
    constant_current,
    membrane_capacitance,
    adaptation,
    exponential,
    event_routing,
    readout,
    refractory_period,
    bayesian);


BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v2::NeuronBlock,
    atomic_neurons,
    backends,
    current_rows,
    v_bias_casc_n,
    i_bias_readout_amp,
    i_bias_leak_source_follower,
    i_bias_threshold_comparator);


EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::SynapticInput)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::Leak)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::Reset)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::Threshold)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::Multicompartment)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::ConstantCurrent)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::MembraneCapacitance)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::Adaptation)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::Exponential)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::Readout)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::EventRouting)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::RefractoryPeriod)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::Bayesian)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::NeuronBlock)
