#pragma once
#include "haldls/vx/v3/capmem.h"
#include "haldls/vx/v3/neuron.h"
#include "hate/visibility.h"
#include "lola/vx/genpybind.h"
#include "lola/vx/v3/synapse.h"

namespace lola::vx::v3 GENPYBIND_TAG_LOLA_VX_V3 {

/**
 * Configuration of digital and analog parameters for a single-denmem neuron.
 */
class GENPYBIND(visible) AtomicNeuron
{
public:
	typedef haldls::vx::v3::CapMemCell::Value AnalogValue GENPYBIND(visible);
	typedef haldls::vx::v3::CapMemCell::DisableRefresh AnalogDisableRefresh GENPYBIND(visible);
	typedef haldls::vx::v3::CapMemCell::value_type AnalogValueVariant;

	struct GENPYBIND(visible) SynapticInput
	{
		SynapticInput() SYMBOL_VISIBLE;

		/**
		 * Digital enable of synaptic input.
		 */
		bool enable;

		/** Bias current for the RC circuit controlling the synaptic input time constant.
		 */
		AnalogValueVariant i_bias_tau;

		/**
		 * Bias current for the conductance based mode of the synaptic input.
		 * Scales the conductivity at a given difference between membrane and
		 * reversal potential.
		 */
		AnalogValueVariant i_bias_coba;

		/**
		 * Bias current for the source follower controlling the reference voltage for the
		 * synaptic input's OTA.
		 */
		AnalogValueVariant i_shift_reference;

		/** Bias current of synaptic input OTA for current-based input. */
		AnalogValueVariant i_bias_gm;

		/** Reversal potential of the COBA based synaptic input */
		AnalogValueVariant v_rev_coba;

		/** Enable small capacitance mode of the synaptic input.
		 * This disconnects the capacitor connected to the line in the synapse array,
		 * using only parasitic capacitance of the line. */
		bool enable_small_capacitance;

		/** Enable high resistance mode controlling the time constant. */
		bool enable_high_resistance;

		/** Switch synaptic input from CUBA to COBA mode. */
		bool enable_coba_mode;

		bool operator==(SynapticInput const& other) const SYMBOL_VISIBLE;
		bool operator!=(SynapticInput const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, SynapticInput const& config)
		    SYMBOL_VISIBLE;
	};

	struct GENPYBIND(visible) Leak
	{
		Leak() SYMBOL_VISIBLE;

		/** Leak potential. */
		AnalogValueVariant v_leak;

		/*
		 * Bias current of OTA during leak.
		 * High values result in a shorter membrane time constant.
		 */
		AnalogValueVariant i_bias;

		/** Enable source degeneration of leak OTA. */
		bool enable_degeneration;

		/** Enable division (/8) of leak conductance. */
		bool enable_division;

		/** Enable multiplication (*8) of leak conductance. */
		bool enable_multiplication;

		bool operator==(Leak const& other) const SYMBOL_VISIBLE;
		bool operator!=(Leak const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, Leak const& config) SYMBOL_VISIBLE;
	};

	struct GENPYBIND(visible) Reset
	{
		Reset() SYMBOL_VISIBLE;

		/** Reset potential. */
		AnalogValueVariant v_reset;

		/*
		 * Bias current of OTA during reset.
		 * High values result in higher reset conductance.
		 */
		AnalogValueVariant i_bias;

		/** Enable source degeneration of reset OTA. */
		bool enable_degeneration;

		/** Enable division (/8) of reset conductance. */
		bool enable_division;

		/** Enable multiplication (*8) of reset conductance. */
		bool enable_multiplication;

		bool operator==(Reset const& other) const SYMBOL_VISIBLE;
		bool operator!=(Reset const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, Reset const& config) SYMBOL_VISIBLE;
	};

	struct GENPYBIND(visible) Threshold
	{
		Threshold() SYMBOL_VISIBLE;

		/** Enable spike threshold comparator. */
		bool enable;

		/** Spike Threshold potential. */
		AnalogValueVariant v_threshold;

		bool operator==(Threshold const& other) const SYMBOL_VISIBLE;
		bool operator!=(Threshold const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, Threshold const& config) SYMBOL_VISIBLE;
	};

	/**
	 * Multicompartment configuration of connectivity to adjacent neurons.
	 * See https://arxiv.org/abs/1804.01840 III.A for more information about the circuit.
	 */
	struct GENPYBIND(visible) Multicompartment
	{
		Multicompartment() SYMBOL_VISIBLE;

		/** Connect local membrane to soma. */
		bool connect_soma;

		/** Connect soma to soma on the right. */
		bool connect_soma_right;

		/** Connect local membrane to membrane on the right. */
		bool connect_right;

		/** Connect local membrane vertically to membrane on the opposite hemisphere. */
		bool connect_vertical;

		/** Enable inter-compartment conductance. */
		bool enable_conductance;

		/** Enable division (/4) of inter-compartment conductance. */
		bool enable_conductance_division;

		/** Enable multiplication (*4) of inter-compartment conductance. */
		bool enable_conductance_multiplication;

		/** Bias current of inter-compartmental conductance. */
		AnalogValueVariant i_bias_nmda;

		bool operator==(Multicompartment const& other) const SYMBOL_VISIBLE;
		bool operator!=(Multicompartment const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, Multicompartment const& config)
		    SYMBOL_VISIBLE;
	};

	struct GENPYBIND(visible) ConstantCurrent
	{
		ConstantCurrent() SYMBOL_VISIBLE;

		/** Enable application of offset current on membrane. */
		bool enable;

		/** Offset current source/sink from the membrane. */
		AnalogValueVariant i_offset;

		/** Sink/source offset current on membrane. */
		enum class Type
		{
			source,
			sink
		} type;

		bool operator==(ConstantCurrent const& other) const SYMBOL_VISIBLE;
		bool operator!=(ConstantCurrent const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, ConstantCurrent const& config)
		    SYMBOL_VISIBLE;
	};

	struct GENPYBIND(visible) MembraneCapacitance
	{
		typedef haldls::vx::v3::NeuronConfig::MembraneCapacitorSize CapacitorSize
		    GENPYBIND(visible);

		MembraneCapacitance() SYMBOL_VISIBLE;

		/** Select membrane capacitance. The maximum value at 63 is ~2.3pF. */
		CapacitorSize capacitance;

		bool operator==(MembraneCapacitance const& other) const SYMBOL_VISIBLE;
		bool operator!=(MembraneCapacitance const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, MembraneCapacitance const& config)
		    SYMBOL_VISIBLE;
	};

	struct GENPYBIND(visible) Adaptation
	{
		Adaptation() SYMBOL_VISIBLE;

		/** Enable adaptation term. */
		bool enable;

		/**
		 * Enable neuron refractory pulse reaching the adaptation circuitry.
		 * The length of the adaptation pulse is set in the common neuron backend.
		 */
		bool enable_pulse;

		/** Flip the sign of a (sub-threshold adaptation). */
		bool invert_a;

		/** Flip the sign of b (spike-triggered adaptation). */
		bool invert_b;

		/** Adaptation reference potential the adaptation state variable is pulled towards. */
		AnalogValueVariant v_ref;

		/** Bias current controlling the adaptation time constant. */
		AnalogValueVariant i_bias_tau;

		/** Bias current controlling the sub-threshold adaptation strength. */
		AnalogValueVariant i_bias_a;

		/** Bias current controlling the spike-triggered adaptation strength. */
		AnalogValueVariant i_bias_b;

		/** Reference potential for adaptation OTA. */
		AnalogValueVariant v_leak;

		bool operator==(Adaptation const& other) const SYMBOL_VISIBLE;
		bool operator!=(Adaptation const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, Adaptation const& config) SYMBOL_VISIBLE;
	};

	struct GENPYBIND(visible) Exponential
	{
		/** Enable exponential term. */
		bool enable;

		/** Threshold voltage for the exponential term. */
		AnalogValueVariant v_exp;

		/** Bias current controlling the strength of the exponential term. */
		AnalogValueVariant i_bias;

		Exponential() SYMBOL_VISIBLE;

		bool operator==(Exponential const& other) const SYMBOL_VISIBLE;
		bool operator!=(Exponential const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, Exponential const& config) SYMBOL_VISIBLE;
	};

	struct GENPYBIND(visible) EventRouting
	{
		EventRouting() SYMBOL_VISIBLE;

		typedef haldls::vx::v3::NeuronBackendConfig::AddressOut Address GENPYBIND(visible);

		/**
		 * Analog output (off/normal/strong) settings.
		 * Non-off states enable event output from the analog part.
		 * This setting also affects the bypass circuits.
		 * "Strong" increases the strength of the analog part's pulse.
		 */
		enum class AnalogOutputMode
		{
			off,
			normal,
			strong
		} analog_output;

		/** Enable sending a spike packet out of the digital backend. */
		bool enable_digital;

		/** Enable excitatory bypass circuit. */
		bool enable_bypass_excitatory;

		/** Enable inhibitory bypass circuit. */
		bool enable_bypass_inhibitory;

		/** Lower 8 bit of address sent on event output. */
		Address address;

		/**
		 * Enable usage of external post pulses reaching the synapses exclusively.
		 * If disabled, the internal neuron spikes reach the synapses.
		 */
		bool enable_post_overwrite;

		EventRouting(
		    AnalogOutputMode analog_output,
		    bool enable_digital,
		    bool enable_bypass_excitatory,
		    bool enable_bypass_inhibitory,
		    Address address,
		    bool enable_post_overwrite) SYMBOL_VISIBLE;

		static const EventRouting disabled SYMBOL_VISIBLE;
		static const EventRouting enabled SYMBOL_VISIBLE;
		static const EventRouting bypass_exc SYMBOL_VISIBLE;
		static const EventRouting bypass_inh SYMBOL_VISIBLE;

		bool operator==(EventRouting const& other) const SYMBOL_VISIBLE;
		bool operator!=(EventRouting const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, EventRouting const& config)
		    SYMBOL_VISIBLE;
	};

	struct GENPYBIND(visible) Readout
	{
		typedef haldls::vx::v3::NeuronConfig::ReadoutSource Source GENPYBIND(visible);

		/**
		 * Connect the readout amplifier's output to the shared readout lines.
		 * Do not enable this setting for more than one neuron per line.
		 * This setting does not need to be enabled to reach the CADCs.
		 */
		bool enable_buffered_access;

		/** Enable readout amplifier. */
		bool enable_amplifier;

		/**
		 * Enable direct, unbuffered access.
		 * Connects output of readout mux to shared line different from the buffered readout lines.
		 */
		bool enable_unbuffered_access;

		/**
		 * Source of readout output.
		 * This multiplexer affects both buffered and unbuffered access.
		 *
		 * When choosing one of the synaptic inputs, the voltage is dropped by a source
		 * follower before reaching the multiplexer. The source follower bias current
		 * is derived from the readout amplifier, thus the enable_amplifier flag and
		 * the current i_bias have to be set, even when using unbuffered access only.
		 *
		 * The mux is not capable of disconnecting signals above 1.2 V properly, which
		 * can occur on the membrane as a result of synaptic input.
		 * In order to record low voltages from e.g. the synaptic input, the synaptic
		 * inputs should be disabled.
		 * During normal operation, the source follower in the synaptic input prevents feedback.
		 */
		Source source;

		Readout() SYMBOL_VISIBLE;

		bool operator==(Readout const& other) const SYMBOL_VISIBLE;
		bool operator!=(Readout const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, Readout const& config) SYMBOL_VISIBLE;
	};

	struct GENPYBIND(visible) RefractoryPeriod
	{
		typedef haldls::vx::v3::NeuronBackendConfig::InputClock InputClock GENPYBIND(visible);
		typedef haldls::vx::v3::NeuronBackendConfig::ResetHoldoff ResetHoldoff GENPYBIND(visible);
		typedef haldls::vx::v3::NeuronBackendConfig::RefractoryTime RefractoryTime
		    GENPYBIND(visible);

		/**
		 * Select between the two input clocks specified in the common neuron backend.
		 * A value of 0 refers to the setting called "slow", a value of 1 selects the "fast" clock.
		 */
		InputClock input_clock;

		/**
		 * Release the reset potential before the refractory time ends.
		 * This allows a dead time after the reset where no new spike can be recorded
		 * and the membrane is connected to the leak potential already.
		 * A higher value results in a later release of the reset.
		 *
		 * Note: A more detailed description is given with the ResetHoldoff type.
		 */
		ResetHoldoff reset_holdoff;

		/**
		 * Counter value to control refractory time.
		 * Beginning with a spike, the counter is increased with the selected input clock.
		 * Once the given value is reached, the refractory period ends.
		 */
		RefractoryTime refractory_time;

		/**
		 * Enable gating of synaptic inputs and exponential term during reset periods.
		 */
		bool enable_pause;

		RefractoryPeriod() SYMBOL_VISIBLE;

		bool operator==(RefractoryPeriod const& other) const SYMBOL_VISIBLE;
		bool operator!=(RefractoryPeriod const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, RefractoryPeriod const& config)
		    SYMBOL_VISIBLE;
	};

	struct GENPYBIND(visible) Bayesian
	{
		Bayesian() SYMBOL_VISIBLE;

		/**
		 * Decide whether the bayesian output will be routed to the synapses as
		 * correlation post-pulse.
		 *
		 * If disabled, spikes in the selected master neuron will trigger post-pulses.
		 */
		bool to_post_pulse;

		/**
		 * Switch the logic operation executed by the bayesian module.
		 * The operation affects the spike output and, if enabled, the correlation post-pulse.
		 *
		 * `local` uses only the neuron's own refractory state.
		 * This is the default for non-connected neurons and does not require further configuration
		 * to work.
		 *
		 * `AND` creates an output only if local and remote refractory states are
		 * present.
		 *
		 * `XNOR` creates an output at the falling edge of an XOR gate between the local and
		 * remote refractory states.
		 *
		 * `MULLER_C` executes a Muller C-element operation between the local and
		 * refractory state: The output is activated if both the local and remote neuron are
		 * refractory, but both have to be non-refractory once before the next output can be
		 * generated.
		 */
		enum class Operation
		{
			local,
			AND,
			XNOR,
			MULLER_C
		} operation;

		/* Connect the neuron_fire_connect signal to the opposing neuron at the other hemisphere. */
		bool connect_fire_vertical;

		/* Output the neuron_fire_connect signal to the next neuron at the right. */
		bool connect_fire_to_right;

		/* Receive the neuron_fire_connect signal from the next neuron at the right. */
		bool connect_fire_from_right;

		/* Write the local refractory state to `neuron_fire_connect`. */
		bool enable_master;

		/* Set the local refractory state from `neuron_fire_connect`. */
		bool enable_slave;

		bool operator==(Bayesian const& other) const SYMBOL_VISIBLE;
		bool operator!=(Bayesian const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, Bayesian const& config) SYMBOL_VISIBLE;
	};


	typedef halco::hicann_dls::vx::v3::AtomicNeuronOnDLS coordinate_type;
	typedef std::false_type has_local_data;

	AtomicNeuron() SYMBOL_VISIBLE;

	SynapticInput excitatory_input;
	SynapticInput inhibitory_input;
	Leak leak;
	Reset reset;
	Threshold threshold;
	Multicompartment multicompartment;
	ConstantCurrent constant_current;
	MembraneCapacitance membrane_capacitance;
	Adaptation adaptation;
	Exponential exponential;
	EventRouting event_routing;
	Readout readout;
	RefractoryPeriod refractory_period;
	Bayesian bayesian;

	/** Allow converting to/from a haldls neuron config. */
	explicit operator haldls::vx::v3::NeuronConfig() const SYMBOL_VISIBLE;
	GENPYBIND_MANUAL({
		parent.def("asNeuronConfig", [](GENPYBIND_PARENT_TYPE const& self) {
			return static_cast<haldls::vx::v3::NeuronConfig>(self);
		});
	})

	void set_from(haldls::vx::v3::NeuronConfig const& neuron_config) SYMBOL_VISIBLE;

	/** Allow converting to/from a haldls neuron backend config. */
	explicit operator haldls::vx::v3::NeuronBackendConfig() const SYMBOL_VISIBLE;
	GENPYBIND_MANUAL({
		parent.def("asNeuronBackendConfig", [](GENPYBIND_PARENT_TYPE const& self) {
			return static_cast<haldls::vx::v3::NeuronBackendConfig>(self);
		});
	})

	void set_from(haldls::vx::v3::NeuronBackendConfig const& neuron_backend_config) SYMBOL_VISIBLE;

	bool operator==(AtomicNeuron const& other) const SYMBOL_VISIBLE;
	bool operator!=(AtomicNeuron const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, AtomicNeuron const& config) SYMBOL_VISIBLE;

private:
	friend struct haldls::vx::detail::VisitPreorderImpl<AtomicNeuron>;
};


/**
 * Neuron block of all neurons on one chip including switches to synapses and global parameters.
 */
class GENPYBIND(visible) NeuronBlock
{
public:
	typedef haldls::vx::v3::CapMemCell::Value AnalogValue GENPYBIND(visible);
	typedef haldls::vx::v3::CapMemCell::DisableRefresh AnalogDisableRefresh GENPYBIND(visible);
	typedef haldls::vx::v3::CapMemCell::value_type AnalogValueVariant;

	typedef halco::hicann_dls::vx::v3::NeuronBlockOnDLS coordinate_type;
	typedef std::false_type has_local_data;

	NeuronBlock() = default;

	typedef halco::common::
	    typed_heap_array<AtomicNeuron, halco::hicann_dls::vx::v3::AtomicNeuronOnDLS>
	        AtomicNeurons GENPYBIND(opaque(false));
	/**
	 * Array of all neuron circuits.
	 */
	AtomicNeurons atomic_neurons{};

	typedef halco::common::typed_array<
	    haldls::vx::v3::CommonNeuronBackendConfig,
	    halco::hicann_dls::vx::v3::CommonNeuronBackendConfigOnDLS>
	    Backends GENPYBIND(opaque(false));

	/**
	 * Block-wise digital backend configuration.
	 */
	Backends backends{};

	typedef halco::common::typed_array<
	    lola::vx::v3::ColumnCurrentRow,
	    halco::hicann_dls::vx::v3::ColumnCurrentRowOnDLS>
	    CurrentRows GENPYBIND(opaque(false));

	/**
	 * Switch-rows to synapse arrays.
	 */
	CurrentRows current_rows{};

	typedef halco::common::
	    typed_array<AnalogValueVariant, halco::hicann_dls::vx::v3::CapMemBlockOnDLS>
	        AnalogValues;
	/**
	 * Cascode bias potential for various OTAs within the neuron.
	 */
	AnalogValues v_bias_casc_n{
	    AnalogValue{250}, AnalogValue{250}, AnalogValue{250}, AnalogValue{250}};

	/**
	 * Bias current for the neuron's readout amplifier.
	 */
	AnalogValues i_bias_readout_amp{
	    AnalogValue{110}, AnalogValue{110}, AnalogValue{110}, AnalogValue{110}};

	/**
	 * Bias current for the leak/reset input voltage drop source follower.
	 */
	AnalogValues i_bias_leak_source_follower{
	    AnalogValue{100}, AnalogValue{100}, AnalogValue{100}, AnalogValue{100}};

	/**
	 * Bias current for the threshold comparator.
	 */
	AnalogValues i_bias_threshold_comparator{
	    AnalogValue{200}, AnalogValue{200}, AnalogValue{200}, AnalogValue{200}};

	/**
	 * Bias current for the source follower controlling the drop of the
	 * voltage on the synaptic line before the OTA's input.
	 */
	AnalogValues i_bias_synin_drop{
	    AnalogValue{300}, AnalogValue{300}, AnalogValue{300}, AnalogValue{300}};

	bool operator==(NeuronBlock const& other) const SYMBOL_VISIBLE;
	bool operator!=(NeuronBlock const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, NeuronBlock const& config) SYMBOL_VISIBLE;

	// TODO (Issue 3622): Manual wrapping is necessary due to genpybind forgetting full
	// qualification
	GENPYBIND_MANUAL({
		auto av = parent->py::template class_<::lola::vx::v3::NeuronBlock::AnalogValues>(
		    parent, "AnalogValues");
		av.def(
		    "fill", &::lola::vx::v3::NeuronBlock::AnalogValues::fill, "", parent->py::arg("val"));
		{
			typedef ::lola::vx::v3::NeuronBlock::AnalogValues::reference (
			    ::lola::vx::v3::NeuronBlock::AnalogValues::*genpybind_at_type)(
			    const ::lola::vx::v3::NeuronBlock::AnalogValues::key_type&);
			av.def(
			    "at", (genpybind_at_type) & ::lola::vx::v3::NeuronBlock::AnalogValues::at, "",
			    parent->py::arg("key"));
		}
		{
			typedef ::lola::vx::v3::NeuronBlock::AnalogValues::const_reference (
			    ::lola::vx::v3::NeuronBlock::AnalogValues::*genpybind_at_type)(
			    const ::lola::vx::v3::NeuronBlock::AnalogValues::key_type&) const;
			av.def(
			    "at", (genpybind_at_type) & ::lola::vx::v3::NeuronBlock::AnalogValues::at, "",
			    parent->py::arg("key"));
		}
		{
			typedef ::lola::vx::v3::NeuronBlock::AnalogValues::reference (
			    ::lola::vx::v3::NeuronBlock::AnalogValues::*genpybind_front_type)();
			av.def(
			    "front", (genpybind_front_type) & ::lola::vx::v3::NeuronBlock::AnalogValues::front,
			    "");
		}
		{
			typedef ::lola::vx::v3::NeuronBlock::AnalogValues::const_reference (
			    ::lola::vx::v3::NeuronBlock::AnalogValues::*genpybind_front_type)() const;
			av.def(
			    "front", (genpybind_front_type) & ::lola::vx::v3::NeuronBlock::AnalogValues::front,
			    "");
		}
		{
			typedef ::lola::vx::v3::NeuronBlock::AnalogValues::reference (
			    ::lola::vx::v3::NeuronBlock::AnalogValues::*genpybind_back_type)();
			av.def(
			    "back", (genpybind_back_type) & ::lola::vx::v3::NeuronBlock::AnalogValues::back,
			    "");
		}
		{
			typedef ::lola::vx::v3::NeuronBlock::AnalogValues::const_reference (
			    ::lola::vx::v3::NeuronBlock::AnalogValues::*genpybind_back_type)() const;
			av.def(
			    "back", (genpybind_back_type) & ::lola::vx::v3::NeuronBlock::AnalogValues::back,
			    "");
		}
		av.def(
		    "__getitem__", &::lola::vx::v3::NeuronBlock::AnalogValues::get, "",
		    parent->py::arg("key"), parent->py::return_value_policy::reference);
		av.def(
		    "__setitem__", &::lola::vx::v3::NeuronBlock::AnalogValues::set, "",
		    parent->py::arg("key"), parent->py::arg("value"));
		av.def(
		    "__iter__",
		    [av](::lola::vx::v3::NeuronBlock::AnalogValues& self) {
			    return pybind11::make_iterator(self);
		    },
		    parent->py::template keep_alive<0, 1>());

		av.def("to_numpy", [](::lola::vx::v3::NeuronBlock::AnalogValues const& self) {
			return ::halco::common::detail::to_numpy(self);
		});
		av.def(
		    "from_numpy",
		    [](::lola::vx::v3::NeuronBlock::AnalogValues& self, pybind11::array const& array) {
			    ::halco::common::detail::from_numpy(self, array);
		    });
		av.def(parent->py::template init<const ::lola::vx::v3::NeuronBlock::AnalogValues&>(), "");
		av.def(parent->py::template init<>(), "");
		av.def_property_readonly(
		    "size", parent->py::cpp_function(&::lola::vx::v3::NeuronBlock::AnalogValues::size));
		av.def_property_readonly(
		    "max_size",
		    parent->py::cpp_function(&::lola::vx::v3::NeuronBlock::AnalogValues::max_size));
		av.def_property_readonly(
		    "empty", parent->py::cpp_function(&::lola::vx::v3::NeuronBlock::AnalogValues::empty));
	})
private:
	friend struct haldls::vx::detail::VisitPreorderImpl<NeuronBlock>;
};

} // namespace lola::vx::v3

#include "lola/vx/v3/neuron.tcc"
