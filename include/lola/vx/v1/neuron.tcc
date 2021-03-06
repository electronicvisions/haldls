#include <boost/hana/adapt_struct.hpp>
#include "haldls/cerealization.h"
#include "haldls/vx/common.h"
#include "haldls/vx/neuron.h"
#include "lola/vx/cerealization.h"

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::v1::AtomicNeuron>
    : public BackendContainerBase<
          lola::vx::v1::AtomicNeuron,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v1::AtomicNeuron>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    lola::vx::v1::AtomicNeuron::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v1;

		visitor(coord, config);

		auto capmem_block = coord.toCapMemBlockOnDLS();
		auto capmem_column = coord.toCapMemColumnOnCapMemBlock();
		auto to_capmem_cell = [capmem_block, capmem_column](CapMemRowOnCapMemBlock row) {
			return CapMemCellOnDLS(CapMemCellOnCapMemBlock(capmem_column, row), capmem_block);
		};

		haldls::vx::v1::NeuronConfig neuron_config;

		neuron_config.set_enable_synaptic_input_excitatory(config.excitatory_input.enable);
		neuron_config.set_enable_synaptic_input_inhibitory(config.inhibitory_input.enable);

		neuron_config.set_enable_leak_degeneration(config.leak_reset.leak.enable_degeneration);
		neuron_config.set_enable_leak_division(config.leak_reset.leak.enable_division);
		neuron_config.set_enable_leak_multiplication(config.leak_reset.leak.enable_multiplication);

		neuron_config.set_enable_reset_degeneration(config.leak_reset.reset.enable_degeneration);
		neuron_config.set_enable_reset_division(config.leak_reset.reset.enable_division);
		neuron_config.set_enable_reset_multiplication(
		    config.leak_reset.reset.enable_multiplication);

		neuron_config.set_enable_threshold_comparator(config.threshold.enable);

		neuron_config.set_enable_membrane_offset(config.constant_current.enable);

		neuron_config.set_connect_soma(config.multicompartment.connect_soma);
		neuron_config.set_connect_soma_right(config.multicompartment.connect_soma_right);
		neuron_config.set_connect_membrane_right(config.multicompartment.connect_right);
		neuron_config.set_connect_bottom(config.multicompartment.connect_vertical);
		neuron_config.set_enable_multicomp_conductance(config.multicompartment.enable_conductance);
		neuron_config.set_enable_divide_multicomp_conductance_bias(
		    config.multicompartment.enable_conductance_division);
		neuron_config.set_enable_multiply_multicomp_conductance_bias(
		    config.multicompartment.enable_conductance_multiplication);

		neuron_config.set_enable_capacitor_merge(config.membrane_capacitance.enable_merge);
		neuron_config.set_membrane_capacitor_size(config.membrane_capacitance.capacitance);

		neuron_config.set_enable_adaptation(config.adaptation.enable);
		neuron_config.set_enable_adaptation_capacitor(config.adaptation.enable_capacitor);
		neuron_config.set_invert_adaptation_a(config.adaptation.invert_a);
		neuron_config.set_invert_adaptation_b(config.adaptation.invert_b);

		neuron_config.set_enable_exponential(config.exponential.enable);
		neuron_config.set_exponential_term_strength(config.exponential.strength);

		neuron_config.set_enable_fire(config.event_routing.enable_analog);
		neuron_config.set_enable_bypass_excitatory(config.event_routing.enable_bypass_excitatory);
		neuron_config.set_enable_bypass_inhibitory(config.event_routing.enable_bypass_inhibitory);

		neuron_config.set_enable_adaptation_readout(
		    config.readout.source == haldls::vx::v1::NeuronConfig::ReadoutSource::adaptation);
		neuron_config.set_enable_unbuffered_access(config.readout.enable_unbuffered_access);
		neuron_config.set_enable_readout_amplifier(config.readout.enable_amplifier);
		neuron_config.set_readout_source(config.readout.source);
		neuron_config.set_enable_readout(config.readout.enable_buffered_access);

		visit_preorder(neuron_config, coord.toNeuronConfigOnDLS(), visitor);

		haldls::vx::v1::CapMemCell cell_i_bias_syn_exc_gm(config.excitatory_input.i_bias_gm);
		haldls::vx::v1::CapMemCell cell_i_bias_syn_exc_res(config.excitatory_input.i_bias_res);
		haldls::vx::v1::CapMemCell cell_v_syn_exc(config.excitatory_input.v_syn);
		haldls::vx::v1::CapMemCell cell_i_bias_syn_inh_gm(config.inhibitory_input.i_bias_gm);
		haldls::vx::v1::CapMemCell cell_i_bias_syn_inh_res(config.inhibitory_input.i_bias_res);
		haldls::vx::v1::CapMemCell cell_v_syn_inh(config.inhibitory_input.v_syn);
		haldls::vx::v1::CapMemCell cell_v_leak(config.leak_reset.leak.v_leak);
		haldls::vx::v1::CapMemCell cell_i_bias_leak(config.leak_reset.leak.i_bias);
		haldls::vx::v1::CapMemCell cell_v_reset(config.leak_reset.reset.v_reset);
		haldls::vx::v1::CapMemCell cell_i_bias_reset(config.leak_reset.reset.i_bias);
		haldls::vx::v1::CapMemCell cell_v_threshold(config.threshold.v_threshold);
		haldls::vx::v1::CapMemCell cell_i_offset(config.constant_current.i_offset);
		haldls::vx::v1::CapMemCell cell_v_leak_adapt(config.adaptation.v_leak);
		haldls::vx::v1::CapMemCell cell_i_bias_adapt_amp(config.adaptation.i_bias_amp);
		haldls::vx::v1::CapMemCell cell_i_bias_adapt_sd(config.adaptation.i_bias_sd);
		haldls::vx::v1::CapMemCell cell_i_bias_adapt_res(config.adaptation.i_bias_res);
		haldls::vx::v1::CapMemCell cell_i_bias_adapt(config.adaptation.i_bias);
		haldls::vx::v1::CapMemCell cell_i_adapt_w(config.adaptation.i_w);
		haldls::vx::v1::CapMemCell cell_i_bias_nmda(config.multicompartment.i_bias_nmda);
		haldls::vx::v1::CapMemCell cell_i_bias_readout(config.readout.i_bias);
		haldls::vx::v1::CapMemCell cell_i_bias_source_follower(
		    config.leak_reset.i_bias_source_follower);

		visit_preorder(
		    cell_i_bias_syn_exc_gm, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_syn_exc_gm),
		    visitor);
		visit_preorder(
		    cell_i_bias_syn_exc_res, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_syn_exc_res),
		    visitor);
		visit_preorder(cell_v_syn_exc, to_capmem_cell(CapMemRowOnCapMemBlock::v_syn_exc), visitor);
		visit_preorder(
		    cell_i_bias_syn_inh_gm, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_syn_inh_gm),
		    visitor);
		visit_preorder(
		    cell_i_bias_syn_inh_res, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_syn_inh_res),
		    visitor);
		visit_preorder(cell_v_syn_inh, to_capmem_cell(CapMemRowOnCapMemBlock::v_syn_inh), visitor);
		visit_preorder(cell_v_leak, to_capmem_cell(CapMemRowOnCapMemBlock::v_leak), visitor);
		visit_preorder(
		    cell_i_bias_leak, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_leak), visitor);
		visit_preorder(cell_v_reset, to_capmem_cell(CapMemRowOnCapMemBlock::v_reset), visitor);
		visit_preorder(
		    cell_i_bias_reset, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_reset), visitor);
		visit_preorder(
		    cell_v_threshold, to_capmem_cell(CapMemRowOnCapMemBlock::v_threshold), visitor);
		visit_preorder(cell_i_offset, to_capmem_cell(CapMemRowOnCapMemBlock::i_offset), visitor);
		visit_preorder(
		    cell_v_leak_adapt, to_capmem_cell(CapMemRowOnCapMemBlock::v_leak_adapt), visitor);
		visit_preorder(
		    cell_i_bias_adapt_amp, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_adapt_amp),
		    visitor);
		visit_preorder(
		    cell_i_bias_adapt_sd, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_adapt_sd), visitor);
		visit_preorder(
		    cell_i_bias_adapt_res, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_adapt_res),
		    visitor);
		visit_preorder(
		    cell_i_bias_adapt, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_adapt), visitor);
		visit_preorder(cell_i_adapt_w, to_capmem_cell(CapMemRowOnCapMemBlock::i_adapt_w), visitor);
		visit_preorder(
		    cell_i_bias_nmda, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_nmda), visitor);
		visit_preorder(
		    cell_i_bias_readout, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_readout), visitor);
		visit_preorder(
		    cell_i_bias_source_follower,
		    to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_source_follower), visitor);

		haldls::vx::v1::NeuronBackendConfig neuron_backend_config;
		neuron_backend_config.set_enable_spike_out(config.event_routing.enable_digital);
		neuron_backend_config.set_enable_bayesian_extension(config.bayesian.enable);
		neuron_backend_config.set_enable_bayesian_0(config.bayesian.enable_0);
		neuron_backend_config.set_enable_bayesian_1(config.bayesian.enable_1);
		neuron_backend_config.set_enable_neuron_master(config.bayesian.enable_master);
		neuron_backend_config.set_enable_neuron_slave(config.bayesian.enable_slave);
		neuron_backend_config.set_connect_fire_bottom(config.bayesian.connect_fire_vertical);
		neuron_backend_config.set_connect_fire_to_right(config.bayesian.connect_fire_to_right);
		neuron_backend_config.set_connect_fire_from_right(config.bayesian.connect_fire_from_right);
		neuron_backend_config.set_address_out(config.event_routing.address);
		neuron_backend_config.set_reset_holdoff(config.refractory_period.reset_holdoff);
		neuron_backend_config.set_refractory_time(config.refractory_period.refractory_time);
		neuron_backend_config.set_select_input_clock(config.refractory_period.input_clock);
		neuron_backend_config.set_enable_adaptation_pulse(config.adaptation.enable_pulse);
		neuron_backend_config.set_post_overwrite(config.event_routing.enable_post_overwrite);

		visit_preorder(neuron_backend_config, coord.toNeuronBackendConfigOnDLS(), visitor);

		if constexpr (!std::is_same<ContainerT, lola::vx::v1::AtomicNeuron const>::value) {
			config.excitatory_input.enable = neuron_config.get_enable_synaptic_input_excitatory();
			config.inhibitory_input.enable = neuron_config.get_enable_synaptic_input_inhibitory();

			config.leak_reset.leak.enable_degeneration =
			    neuron_config.get_enable_leak_degeneration();
			config.leak_reset.leak.enable_division = neuron_config.get_enable_leak_division();
			config.leak_reset.leak.enable_multiplication =
			    neuron_config.get_enable_leak_multiplication();

			config.leak_reset.reset.enable_degeneration =
			    neuron_config.get_enable_reset_degeneration();
			config.leak_reset.reset.enable_division = neuron_config.get_enable_reset_division();
			config.leak_reset.reset.enable_multiplication =
			    neuron_config.get_enable_reset_multiplication();

			config.threshold.enable = neuron_config.get_enable_threshold_comparator();

			config.constant_current.enable = neuron_config.get_enable_membrane_offset();

			config.multicompartment.connect_soma = neuron_config.get_connect_soma();
			config.multicompartment.connect_soma_right = neuron_config.get_connect_soma_right();
			config.multicompartment.connect_right = neuron_config.get_connect_membrane_right();
			config.multicompartment.connect_vertical = neuron_config.get_connect_bottom();
			config.multicompartment.enable_conductance =
			    neuron_config.get_enable_multicomp_conductance();
			config.multicompartment.enable_conductance_division =
			    neuron_config.get_enable_divide_multicomp_conductance_bias();
			config.multicompartment.enable_conductance_multiplication =
			    neuron_config.get_enable_multiply_multicomp_conductance_bias();

			config.membrane_capacitance.enable_merge = neuron_config.get_enable_capacitor_merge();
			config.membrane_capacitance.capacitance = neuron_config.get_membrane_capacitor_size();

			config.adaptation.enable = neuron_config.get_enable_adaptation();
			config.adaptation.enable_capacitor = neuron_config.get_enable_adaptation_capacitor();
			config.adaptation.invert_a = neuron_config.get_invert_adaptation_a();
			config.adaptation.invert_b = neuron_config.get_invert_adaptation_b();
			config.adaptation.enable_pulse = neuron_backend_config.get_enable_adaptation_pulse();

			config.refractory_period.reset_holdoff = neuron_backend_config.get_reset_holdoff();
			config.refractory_period.refractory_time = neuron_backend_config.get_refractory_time();
			config.refractory_period.input_clock = neuron_backend_config.get_select_input_clock();

			config.exponential.enable = neuron_config.get_enable_exponential();
			config.exponential.strength = neuron_config.get_exponential_term_strength();

			config.readout.enable_unbuffered_access = neuron_config.get_enable_unbuffered_access();
			config.readout.enable_amplifier = neuron_config.get_enable_readout_amplifier();
			config.readout.source = neuron_config.get_readout_source();
			config.readout.enable_buffered_access = neuron_config.get_enable_readout();
			if ((config.readout.source == haldls::vx::v1::NeuronConfig::ReadoutSource::adaptation) ^
			    neuron_config.get_enable_adaptation_readout()) {
				throw std::runtime_error("lola::vx::v1::AtomicNeuron can't represent a state of "
				                         "(readout-mux==adaptation ^ enable_adaptation)");
			}

			config.event_routing.enable_analog = neuron_config.get_enable_fire();
			config.event_routing.enable_digital = neuron_backend_config.get_enable_spike_out();
			config.event_routing.enable_bypass_excitatory =
			    neuron_config.get_enable_bypass_excitatory();
			config.event_routing.enable_bypass_inhibitory =
			    neuron_config.get_enable_bypass_inhibitory();
			config.event_routing.enable_post_overwrite = neuron_backend_config.get_post_overwrite();

			config.event_routing.address = neuron_backend_config.get_address_out();
			config.bayesian.enable_master = neuron_backend_config.get_enable_neuron_master();
			config.bayesian.enable_slave = neuron_backend_config.get_enable_neuron_slave();
			config.bayesian.connect_fire_vertical = neuron_backend_config.get_connect_fire_bottom();
			config.bayesian.connect_fire_to_right =
			    neuron_backend_config.get_connect_fire_to_right();
			config.bayesian.connect_fire_from_right =
			    neuron_backend_config.get_connect_fire_from_right();
			config.bayesian.enable = neuron_backend_config.get_enable_bayesian_extension();
			config.bayesian.enable_0 = neuron_backend_config.get_enable_bayesian_0();
			config.bayesian.enable_1 = neuron_backend_config.get_enable_bayesian_1();

			config.excitatory_input.i_bias_gm = cell_i_bias_syn_exc_gm.get_value();
			config.excitatory_input.i_bias_res = cell_i_bias_syn_exc_res.get_value();
			config.excitatory_input.v_syn = cell_v_syn_exc.get_value();
			config.inhibitory_input.i_bias_gm = cell_i_bias_syn_inh_gm.get_value();
			config.inhibitory_input.i_bias_res = cell_i_bias_syn_inh_res.get_value();
			config.inhibitory_input.v_syn = cell_v_syn_inh.get_value();
			config.leak_reset.leak.v_leak = cell_v_leak.get_value();
			config.leak_reset.leak.i_bias = cell_i_bias_leak.get_value();
			config.leak_reset.reset.v_reset = cell_v_reset.get_value();
			config.leak_reset.reset.i_bias = cell_i_bias_reset.get_value();
			config.threshold.v_threshold = cell_v_threshold.get_value();
			config.constant_current.i_offset = cell_i_offset.get_value();
			config.adaptation.v_leak = cell_v_leak_adapt.get_value();
			config.adaptation.i_bias_amp = cell_i_bias_adapt_amp.get_value();
			config.adaptation.i_bias_sd = cell_i_bias_adapt_sd.get_value();
			config.adaptation.i_bias_res = cell_i_bias_adapt_res.get_value();
			config.adaptation.i_bias = cell_i_bias_adapt.get_value();
			config.adaptation.i_w = cell_i_adapt_w.get_value();
			config.multicompartment.i_bias_nmda = cell_i_bias_nmda.get_value();
			config.readout.i_bias = cell_i_bias_readout.get_value();
			config.leak_reset.i_bias_source_follower = cell_i_bias_source_follower.get_value();
		}
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v1::AtomicNeuron::SynapticInput, enable, i_bias_res, i_bias_gm, v_syn);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v1::AtomicNeuron::LeakReset::Leak,
    v_leak,
    i_bias,
    enable_degeneration,
    enable_division,
    enable_multiplication);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v1::AtomicNeuron::LeakReset::Reset,
    v_reset,
    i_bias,
    enable_degeneration,
    enable_division,
    enable_multiplication);

BOOST_HANA_ADAPT_STRUCT(lola::vx::v1::AtomicNeuron::LeakReset, leak, reset, i_bias_source_follower);

BOOST_HANA_ADAPT_STRUCT(lola::vx::v1::AtomicNeuron::Threshold, enable, v_threshold);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v1::AtomicNeuron::Multicompartment,
    connect_soma,
    connect_soma_right,
    connect_right,
    connect_vertical,
    enable_conductance,
    enable_conductance_division,
    enable_conductance_multiplication,
    i_bias_nmda);

BOOST_HANA_ADAPT_STRUCT(lola::vx::v1::AtomicNeuron::ConstantCurrent, enable, i_offset);

BOOST_HANA_ADAPT_STRUCT(lola::vx::v1::AtomicNeuron::MembraneCapacitance, enable_merge, capacitance);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v1::AtomicNeuron::Adaptation,
    enable,
    enable_pulse,
    enable_capacitor,
    invert_a,
    invert_b,
    i_bias,
    i_bias_amp,
    i_bias_sd,
    i_bias_res,
    i_w,
    v_leak);

BOOST_HANA_ADAPT_STRUCT(lola::vx::v1::AtomicNeuron::Exponential, enable, strength);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v1::AtomicNeuron::Readout,
    enable_buffered_access,
    enable_amplifier,
    enable_unbuffered_access,
    source,
    i_bias);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v1::AtomicNeuron::EventRouting,
    enable_analog,
    enable_digital,
    enable_bypass_excitatory,
    enable_bypass_inhibitory,
    address,
    enable_post_overwrite);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v1::AtomicNeuron::RefractoryPeriod, input_clock, reset_holdoff, refractory_time);
BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v1::AtomicNeuron::Bayesian,
    enable,
    connect_fire_vertical,
    connect_fire_to_right,
    connect_fire_from_right,
    enable_master,
    enable_slave,
    enable_0,
    enable_1);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v1::AtomicNeuron,
    excitatory_input,
    inhibitory_input,
    leak_reset,
    threshold,
    multicompartment,
    constant_current,
    membrane_capacitance,
    adaptation,
    exponential,
    event_routing,
    readout);

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::SynapticInput)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::LeakReset::Leak)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::LeakReset::Reset)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::LeakReset)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::Threshold)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::Multicompartment)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::ConstantCurrent)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::MembraneCapacitance)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::Adaptation)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::Exponential)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::Readout)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::EventRouting)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::RefractoryPeriod)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::Bayesian)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron)
