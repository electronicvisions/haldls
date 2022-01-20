#include "haldls/cerealization.h"
#include "haldls/vx/common.h"
#include "haldls/vx/neuron.h"
#include "lola/vx/cerealization.h"
#include <boost/hana/adapt_struct.hpp>

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::v3::AtomicNeuron>
    : public BackendContainerBase<
          lola::vx::v3::AtomicNeuron,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v3::AtomicNeuron>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    lola::vx::v3::AtomicNeuron::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v3;

		visitor(coord, config);

		auto capmem_block = coord.toCapMemBlockOnDLS();
		auto capmem_column = coord.toCapMemColumnOnCapMemBlock();
		auto to_capmem_cell = [capmem_block, capmem_column](CapMemRowOnCapMemBlock row) {
			return CapMemCellOnDLS(CapMemCellOnCapMemBlock(capmem_column, row), capmem_block);
		};

		auto neuron_config = static_cast<haldls::vx::v3::NeuronConfig>(config);
		visit_preorder(neuron_config, coord.toNeuronConfigOnDLS(), visitor);

		haldls::vx::v3::CapMemCell cell_i_bias_synin_exc_tau(config.excitatory_input.i_bias_tau);
		haldls::vx::v3::CapMemCell cell_i_bias_synin_exc_drop(config.excitatory_input.i_drop_input);
		haldls::vx::v3::CapMemCell cell_i_bias_synin_exc_shift(
		    config.excitatory_input.i_shift_reference);
		haldls::vx::v3::CapMemCell cell_i_bias_synin_exc_gm(config.excitatory_input.i_bias_gm);
		haldls::vx::v3::CapMemCell cell_i_bias_synin_inh_tau(config.inhibitory_input.i_bias_tau);
		haldls::vx::v3::CapMemCell cell_i_bias_synin_inh_drop(config.inhibitory_input.i_drop_input);
		haldls::vx::v3::CapMemCell cell_i_bias_synin_inh_shift(
		    config.inhibitory_input.i_shift_reference);
		haldls::vx::v3::CapMemCell cell_i_bias_synin_inh_gm(config.inhibitory_input.i_bias_gm);
		haldls::vx::v3::CapMemCell cell_v_leak(config.leak.v_leak);
		haldls::vx::v3::CapMemCell cell_i_bias_leak(config.leak.i_bias);
		haldls::vx::v3::CapMemCell cell_v_reset(config.reset.v_reset);
		haldls::vx::v3::CapMemCell cell_i_bias_reset(config.reset.i_bias);
		haldls::vx::v3::CapMemCell cell_v_threshold(config.threshold.v_threshold);
		haldls::vx::v3::CapMemCell cell_i_offset(config.constant_current.i_offset);
		haldls::vx::v3::CapMemCell cell_v_ref_adapt(config.adaptation.v_ref);
		haldls::vx::v3::CapMemCell cell_v_leak_adapt(config.adaptation.v_leak);
		haldls::vx::v3::CapMemCell cell_i_bias_adapt_tau(config.adaptation.i_bias_tau);
		haldls::vx::v3::CapMemCell cell_i_bias_adapt_a(config.adaptation.i_bias_a);
		haldls::vx::v3::CapMemCell cell_i_bias_adapt_b(config.adaptation.i_bias_b);
		haldls::vx::v3::CapMemCell cell_v_exp(config.exponential.v_exp);
		haldls::vx::v3::CapMemCell cell_i_bias_exp(config.exponential.i_bias);
		haldls::vx::v3::CapMemCell cell_i_bias_nmda(config.multicompartment.i_bias_nmda);

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

		auto neuron_backend_config = static_cast<haldls::vx::v3::NeuronBackendConfig>(config);
		visit_preorder(neuron_backend_config, coord.toNeuronBackendConfigOnDLS(), visitor);

		if constexpr (!std::is_same<ContainerT, lola::vx::v3::AtomicNeuron const>::value) {
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

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v3::AtomicNeuron::SynapticInput,
    enable,
    i_bias_tau,
    i_drop_input,
    i_shift_reference,
    i_bias_gm,
    enable_small_capacitor,
    enable_high_resistance);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v3::AtomicNeuron::Leak,
    v_leak,
    i_bias,
    enable_degeneration,
    enable_division,
    enable_multiplication);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v3::AtomicNeuron::Reset,
    v_reset,
    i_bias,
    enable_degeneration,
    enable_division,
    enable_multiplication);

BOOST_HANA_ADAPT_STRUCT(lola::vx::v3::AtomicNeuron::Threshold, enable, v_threshold);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v3::AtomicNeuron::Multicompartment,
    connect_soma,
    connect_soma_right,
    connect_right,
    connect_vertical,
    enable_conductance,
    enable_conductance_division,
    enable_conductance_multiplication,
    i_bias_nmda);

BOOST_HANA_ADAPT_STRUCT(lola::vx::v3::AtomicNeuron::ConstantCurrent, enable, i_offset, type);

BOOST_HANA_ADAPT_STRUCT(lola::vx::v3::AtomicNeuron::MembraneCapacitance, capacitance);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v3::AtomicNeuron::Adaptation,
    enable,
    enable_pulse,
    invert_a,
    invert_b,
    v_ref,
    i_bias_tau,
    i_bias_a,
    i_bias_b,
    v_leak);

BOOST_HANA_ADAPT_STRUCT(lola::vx::v3::AtomicNeuron::Exponential, enable, v_exp, i_bias);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v3::AtomicNeuron::Readout,
    enable_buffered_access,
    enable_amplifier,
    enable_unbuffered_access,
    source);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v3::AtomicNeuron::EventRouting,
    analog_output,
    enable_digital,
    enable_bypass_excitatory,
    enable_bypass_inhibitory,
    address,
    enable_post_overwrite);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v3::AtomicNeuron::RefractoryPeriod,
    input_clock,
    reset_holdoff,
    refractory_time,
    enable_pause);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v3::AtomicNeuron::Bayesian,
    enable,
    connect_fire_vertical,
    connect_fire_to_right,
    connect_fire_from_right,
    enable_master,
    enable_slave,
    enable_0,
    enable_1);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v3::AtomicNeuron,
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

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::AtomicNeuron::SynapticInput)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::AtomicNeuron::Leak)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::AtomicNeuron::Reset)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::AtomicNeuron::Threshold)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::AtomicNeuron::Multicompartment)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::AtomicNeuron::ConstantCurrent)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::AtomicNeuron::MembraneCapacitance)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::AtomicNeuron::Adaptation)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::AtomicNeuron::Exponential)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::AtomicNeuron::Readout)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::AtomicNeuron::EventRouting)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::AtomicNeuron::RefractoryPeriod)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::AtomicNeuron::Bayesian)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::AtomicNeuron)
