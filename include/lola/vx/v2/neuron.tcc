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
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v2::AtomicNeuron>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
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

		haldls::vx::v2::NeuronConfig neuron_config;

		neuron_config.set_enable_synaptic_input_excitatory(config.excitatory_input.enable);
		neuron_config.set_enable_synaptic_input_inhibitory(config.inhibitory_input.enable);

		neuron_config.set_enable_synaptic_input_excitatory_small_capacitor(
		    config.excitatory_input.enable_small_capacitor);
		neuron_config.set_enable_synaptic_input_inhibitory_small_capacitor(
		    config.inhibitory_input.enable_small_capacitor);

		neuron_config.set_enable_synaptic_input_excitatory_high_resistance(
		    config.excitatory_input.enable_high_resistance);
		neuron_config.set_enable_synaptic_input_inhibitory_high_resistance(
		    config.inhibitory_input.enable_high_resistance);

		neuron_config.set_enable_leak_degeneration(config.leak.enable_degeneration);
		neuron_config.set_enable_leak_division(config.leak.enable_division);
		neuron_config.set_enable_leak_multiplication(config.leak.enable_multiplication);

		neuron_config.set_enable_reset_degeneration(config.reset.enable_degeneration);
		neuron_config.set_enable_reset_division(config.reset.enable_division);
		neuron_config.set_enable_reset_multiplication(config.reset.enable_multiplication);

		neuron_config.set_enable_threshold_comparator(config.threshold.enable);

		neuron_config.set_enable_membrane_offset(config.constant_current.enable);
		neuron_config.set_invert_membrane_offset(config.constant_current.type
			== lola::vx::v2::AtomicNeuron::ConstantCurrent::Type::sink);

		neuron_config.set_connect_soma(config.multicompartment.connect_soma);
		neuron_config.set_connect_soma_right(config.multicompartment.connect_soma_right);
		neuron_config.set_connect_membrane_right(config.multicompartment.connect_right);
		neuron_config.set_connect_bottom(config.multicompartment.connect_vertical);
		neuron_config.set_enable_multicomp_conductance(config.multicompartment.enable_conductance);
		neuron_config.set_enable_divide_multicomp_conductance_bias(
		    config.multicompartment.enable_conductance_division);
		neuron_config.set_enable_multiply_multicomp_conductance_bias(
		    config.multicompartment.enable_conductance_multiplication);

		neuron_config.set_membrane_capacitor_size(config.membrane_capacitance.capacitance);

		neuron_config.set_enable_adaptation(config.adaptation.enable);
		neuron_config.set_invert_adaptation_a(config.adaptation.invert_a);
		neuron_config.set_invert_adaptation_b(config.adaptation.invert_b);

		neuron_config.set_enable_exponential(config.exponential.enable);

		if (config.event_routing.analog_output != lola::vx::v2::AtomicNeuron::EventRouting::AnalogOutputMode::off) {
			neuron_config.set_enable_fire(true);
			if (config.event_routing.analog_output == lola::vx::v2::AtomicNeuron::EventRouting::AnalogOutputMode::strong) {
				neuron_config.set_enable_strong_fire(true);
			}
		}
		neuron_config.set_enable_bypass_excitatory(config.event_routing.enable_bypass_excitatory);
		neuron_config.set_enable_bypass_inhibitory(config.event_routing.enable_bypass_inhibitory);

		neuron_config.set_enable_adaptation_readout(
		    config.readout.source == haldls::vx::v2::NeuronConfig::ReadoutSource::adaptation);
		neuron_config.set_enable_unbuffered_access(config.readout.enable_unbuffered_access);
		neuron_config.set_enable_readout_amplifier(config.readout.enable_amplifier);
		neuron_config.set_readout_source(config.readout.source);
		neuron_config.set_enable_readout(config.readout.enable_buffered_access);

		neuron_config.set_enable_pause(config.refractory_period.enable_pause);

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
		visit_preorder(
		    cell_i_bias_nmda, to_capmem_cell(CapMemRowOnCapMemBlock::i_bias_nmda), visitor);

		haldls::vx::v2::NeuronBackendConfig neuron_backend_config;
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

		if constexpr (!std::is_same<ContainerT, lola::vx::v2::AtomicNeuron const>::value) {
			config.excitatory_input.enable = neuron_config.get_enable_synaptic_input_excitatory();
			config.inhibitory_input.enable = neuron_config.get_enable_synaptic_input_inhibitory();

			config.excitatory_input.enable_small_capacitor =
			    neuron_config.get_enable_synaptic_input_excitatory_small_capacitor();
			config.inhibitory_input.enable_small_capacitor =
			    neuron_config.get_enable_synaptic_input_inhibitory_small_capacitor();

			config.excitatory_input.enable_high_resistance =
			    neuron_config.get_enable_synaptic_input_excitatory_high_resistance();
			config.inhibitory_input.enable_high_resistance =
			    neuron_config.get_enable_synaptic_input_inhibitory_high_resistance();

			config.leak.enable_degeneration = neuron_config.get_enable_leak_degeneration();
			config.leak.enable_division = neuron_config.get_enable_leak_division();
			config.leak.enable_multiplication = neuron_config.get_enable_leak_multiplication();

			config.reset.enable_degeneration = neuron_config.get_enable_reset_degeneration();
			config.reset.enable_division = neuron_config.get_enable_reset_division();
			config.reset.enable_multiplication = neuron_config.get_enable_reset_multiplication();

			config.threshold.enable = neuron_config.get_enable_threshold_comparator();

			config.constant_current.enable = neuron_config.get_enable_membrane_offset();
			config.constant_current.type =
				neuron_config.get_invert_membrane_offset() ?
				lola::vx::v2::AtomicNeuron::ConstantCurrent::Type::sink :
				lola::vx::v2::AtomicNeuron::ConstantCurrent::Type::source;

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

			config.membrane_capacitance.capacitance = neuron_config.get_membrane_capacitor_size();

			config.adaptation.enable = neuron_config.get_enable_adaptation();
			config.adaptation.invert_a = neuron_config.get_invert_adaptation_a();
			config.adaptation.invert_b = neuron_config.get_invert_adaptation_b();
			config.adaptation.enable_pulse = neuron_backend_config.get_enable_adaptation_pulse();

			config.refractory_period.reset_holdoff = neuron_backend_config.get_reset_holdoff();
			config.refractory_period.refractory_time = neuron_backend_config.get_refractory_time();
			config.refractory_period.input_clock = neuron_backend_config.get_select_input_clock();
			config.refractory_period.enable_pause = neuron_config.get_enable_pause();

			config.exponential.enable = neuron_config.get_enable_exponential();

			config.readout.enable_unbuffered_access = neuron_config.get_enable_unbuffered_access();
			config.readout.enable_amplifier = neuron_config.get_enable_readout_amplifier();
			config.readout.source = neuron_config.get_readout_source();
			config.readout.enable_buffered_access = neuron_config.get_enable_readout();
			if ((config.readout.source == haldls::vx::v2::NeuronConfig::ReadoutSource::adaptation) ^
			    neuron_config.get_enable_adaptation_readout()) {
				throw std::runtime_error("lola::vx::v2::AtomicNeuron can't represent a state of "
				                         "(readout-mux==adaptation ^ enable_adaptation)");
			}

			if (neuron_config.get_enable_fire()) {
				if (!neuron_config.get_enable_strong_fire()) {
					config.event_routing.analog_output = lola::vx::v2::AtomicNeuron::EventRouting::AnalogOutputMode::strong;
				} else {
					config.event_routing.analog_output = lola::vx::v2::AtomicNeuron::EventRouting::AnalogOutputMode::normal;
				}
			} else {
				config.event_routing.analog_output = lola::vx::v2::AtomicNeuron::EventRouting::AnalogOutputMode::off;
			}

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
			config.multicompartment.i_bias_nmda = cell_i_bias_nmda.get_value();
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
