#pragma once
#include "haldls/vx/v1/capmem.h"
#include "haldls/vx/v1/neuron.h"
#include "hate/visibility.h"
#include "lola/vx/genpybind.h"

namespace lola::vx::v1 GENPYBIND_TAG_LOLA_VX_V1 {

/**
 * Configuration of digital and analog parameters for a single-denmem neuron.
 */
class GENPYBIND(visible) AtomicNeuron
{
public:
	typedef haldls::vx::v1::CapMemCell::Value AnalogValue GENPYBIND(visible);
	typedef haldls::vx::v1::CapMemCell::DisableRefresh AnalogDisableRefresh GENPYBIND(visible);
	typedef haldls::vx::v1::CapMemCell::value_type AnalogValueVariant;

	struct GENPYBIND(visible) SynapticInput
	{
		SynapticInput() SYMBOL_VISIBLE;

		/**
		 * Digital enable of synaptic input. Disabling this does not suffice for effectively
		 * blocking inputs. The current i_bias_gm needs to be changed as well.
		 */
		bool enable;

		/**
		 * Bias current controlling the synaptic time constant.
		 * High values lead to a shorter synaptic time constant.
		 */
		AnalogValueVariant i_bias_res;

		/**
		 * Bias current of synaptic input OTA for current-based input.
		 * High values lead to stronger synaptic input.
		 */
		AnalogValueVariant i_bias_gm;

		/** Synaptic input OTA reference potential. */
		AnalogValueVariant v_syn;

		bool operator==(SynapticInput const& other) const SYMBOL_VISIBLE;
		bool operator!=(SynapticInput const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, SynapticInput const& config)
		    SYMBOL_VISIBLE;
	};

	struct GENPYBIND(visible) LeakReset
	{
		LeakReset() SYMBOL_VISIBLE;

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

		/**
		 * Configuration unique to the leak operation.
		 */
		Leak leak;

		/**
		 * Configuration unique to the leak operation.
		 */
		Reset reset;

		/**
		 * Bias current of the source follower in the leak/reset circuit.
		 * Both the leak and reset potential are connected via this source follower.
		 *
		 * The voltage is dropped by approximately 0.6 V.
		 * Higher bias currents mean slightly higher voltage drops.
		 */
		AnalogValueVariant i_bias_source_follower;

		bool operator==(LeakReset const& other) const SYMBOL_VISIBLE;
		bool operator!=(LeakReset const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, LeakReset const& config) SYMBOL_VISIBLE;
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

		/** Offset current sink from the membrane. */
		AnalogValueVariant i_offset;

		bool operator==(ConstantCurrent const& other) const SYMBOL_VISIBLE;
		bool operator!=(ConstantCurrent const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, ConstantCurrent const& config)
		    SYMBOL_VISIBLE;
	};

	struct GENPYBIND(visible) MembraneCapacitance
	{
		typedef haldls::vx::v1::NeuronConfig::MembraneCapacitorSize CapacitorSize
		    GENPYBIND(visible);

		MembraneCapacitance() SYMBOL_VISIBLE;

		/** Enable merging of membrane and adaptation capacitances. */
		bool enable_merge;

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

		/**
		 * Enable capacitance.
		 * Spike-triggered adaptation will only be possible with the capacitor connected.
		 */
		bool enable_capacitor;

		/** Flip the sign of a (sub-threshold adaptation). */
		bool invert_a;

		/** Flip the sign of b (spike-triggered adaptation). */
		bool invert_b;

		/**
		 * Bias current for the Adaptation OTA.
		 * High values lead to stronger adaptation currents onto the membrane.
		 */
		AnalogValueVariant i_bias;

		/** Bias current for the amplifier between membrane and the resistor to v_w. */
		AnalogValueVariant i_bias_amp;

		/** Bias current for source degeneration of the adaptation OTA. */
		AnalogValueVariant i_bias_sd;

		/**
		 * Bias current for the resistor connecting adaptation v_w to the membrane (leading to decay
		 * of the adaptation to zero).
		 */
		AnalogValueVariant i_bias_res;

		/** Strength of the spike-triggered adaptation. */
		AnalogValueVariant i_w;

		/** Reference potential for adaptation OTA. */
		AnalogValueVariant v_leak;

		bool operator==(Adaptation const& other) const SYMBOL_VISIBLE;
		bool operator!=(Adaptation const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, Adaptation const& config) SYMBOL_VISIBLE;
	};

	struct GENPYBIND(visible) Exponential
	{
		typedef haldls::vx::v1::NeuronConfig::ExponentialTermStrength Strength GENPYBIND(visible);

		/** Enable exponential term. */
		bool enable;

		/** Strength of exponential term. */
		Strength strength;

		Exponential() SYMBOL_VISIBLE;

		bool operator==(Exponential const& other) const SYMBOL_VISIBLE;
		bool operator!=(Exponential const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, Exponential const& config) SYMBOL_VISIBLE;
	};

	struct GENPYBIND(visible) EventRouting
	{
		EventRouting() SYMBOL_VISIBLE;

		typedef haldls::vx::v1::NeuronBackendConfig::AddressOut Address GENPYBIND(visible);

		EventRouting(
		    bool enable_analog,
		    bool enable_digital,
		    bool enable_bypass_excitatory,
		    bool enable_bypass_inhibitory,
		    Address address,
		    bool enable_post_overwrite) SYMBOL_VISIBLE;

		/**
		 * Enable event output from the analog part.
		 * This setting also affects the bypass circuits.
		 */
		bool enable_analog;

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
		typedef haldls::vx::v1::NeuronConfig::ReadoutSource Source GENPYBIND(visible);

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

		/**
		 * Bias current for the final readout buffer at the Neuron.
		 * This buffer is after the mux that selects readout from e.g. membrane or adaptation
		 * capacitor. The bias current has to be set high when using this neuron's readout amplifier
		 * for any input, including the connections to the CADC in the ColumnCurrentSwitches or the
		 * common neuron readout line selected with enable_readout in the NeuronConfig.
		 */
		AnalogValueVariant i_bias;

		Readout() SYMBOL_VISIBLE;

		bool operator==(Readout const& other) const SYMBOL_VISIBLE;
		bool operator!=(Readout const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, Readout const& config) SYMBOL_VISIBLE;
	};

	struct GENPYBIND(visible) RefractoryPeriod
	{
		typedef haldls::vx::v1::NeuronBackendConfig::InputClock InputClock GENPYBIND(visible);
		typedef haldls::vx::v1::NeuronBackendConfig::ResetHoldoff ResetHoldoff GENPYBIND(visible);
		typedef haldls::vx::v1::NeuronBackendConfig::RefractoryTime RefractoryTime
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
		 * A value of 15 disables this feature, indicating the reset and refractory periods are
		 * equal and the neuron can spike again immediately.
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

		bool enable;
		bool connect_fire_vertical;
		bool connect_fire_to_right;
		bool connect_fire_from_right;

		/**
		 * Enable local spikes to be effective.
		 * If not using Bayesian extensions, this setting needs to be enabled for spikes
		 * to reach synapses and the outputs.
		 */
		bool enable_master;
		bool enable_slave;
		bool enable_0;
		bool enable_1;

		bool operator==(Bayesian const& other) const SYMBOL_VISIBLE;
		bool operator!=(Bayesian const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, Bayesian const& config) SYMBOL_VISIBLE;
	};


	typedef halco::hicann_dls::vx::v1::AtomicNeuronOnDLS coordinate_type;
	typedef std::false_type has_local_data;

	AtomicNeuron() SYMBOL_VISIBLE;

	SynapticInput excitatory_input;
	SynapticInput inhibitory_input;
	LeakReset leak_reset;
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

	bool operator==(AtomicNeuron const& other) const SYMBOL_VISIBLE;
	bool operator!=(AtomicNeuron const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, AtomicNeuron const& config) SYMBOL_VISIBLE;

private:
	friend class haldls::vx::detail::VisitPreorderImpl<AtomicNeuron>;
};

} // namespace lola::vx::v1

#include "lola/vx/v1/neuron.tcc"
