#pragma once
#include <array>
#include <iosfwd>

#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/chip.h"
#include "halco/hicann-dls/vx/current_dac.h"
#include "halco/hicann-dls/vx/synapse.h"
#include "haldls/vx/container.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"
#include <cereal/macros.hpp>

namespace haldls::vx {

struct CurrentDAC;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CurrentDAC& value, std::uint32_t const version);

} // namespace cereal

namespace fisch::vx {

class OmnibusChipOverJTAG;
class Omnibus;

} // namespace fisch::vx

namespace haldls::vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Configuration of the current DAC of the readout chain.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) CurrentDAC
    : public ContainerBase<CurrentDAC>
{
public:
	typedef halco::hicann_dls::vx::CurrentDACOnDLS coordinate_type;

	typedef std::true_type is_leaf_node;

	/** Decide whether current is sourced or sinked. */
	enum class Sign : uint_fast8_t
	{
		source = 0b00,
		sink = 0b01
	};

	typedef halco::common::typed_array<bool, halco::hicann_dls::vx::HemisphereOnDLS> HemisphereType
	    GENPYBIND(opaque(false));

	typedef halco::common::typed_array<bool, halco::hicann_dls::vx::SynapticInputOnNeuron>
	    SynapseTargetType GENPYBIND(opaque(false));

	/** Value of current DAC. */
	struct GENPYBIND(inline_base("*")) Current
	    : public halco::common::detail::BaseType<Current, uint8_t>
	{
		constexpr explicit Current(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    base_t(val)
		{}
	};

	/** Default constructor with all inputs disabled. */
	CurrentDAC() SYMBOL_VISIBLE;

	/** Enable current source. */
	GENPYBIND(getter_for(enable_current))
	bool get_enable_current() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_current))
	void set_enable_current(bool value) SYMBOL_VISIBLE;

	/** Value of current DAC. */
	GENPYBIND(getter_for(current))
	Current get_current() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(current))
	void set_current(Current value) SYMBOL_VISIBLE;

	/** Select sign of current. */
	GENPYBIND(getter_for(sign))
	Sign get_sign() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(sign))
	void set_sign(Sign value) SYMBOL_VISIBLE;

	/** Enable mux connection to neuron array current stimulus of corresponding hemisphere. */
	GENPYBIND(getter_for(connect_neuron))
	HemisphereType get_connect_neuron() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(connect_neuron))
	void set_connect_neuron(HemisphereType value) SYMBOL_VISIBLE;

	/** Enable mux connection to synapse. */
	GENPYBIND(getter_for(connect_synapse))
	SynapseTargetType get_connect_synapse() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(connect_synapse))
	void set_connect_synapse(SynapseTargetType value) SYMBOL_VISIBLE;

	bool operator==(CurrentDAC const& other) const SYMBOL_VISIBLE;
	bool operator!=(CurrentDAC const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CurrentDAC const& config) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;

	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& coord)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);

	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& words) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(Archive& ar, CurrentDAC& value, std::uint32_t const version)
	    SYMBOL_VISIBLE;

	Current m_current;
	bool m_enable_current;
	Sign m_sign;
	HemisphereType m_connect_neuron;
	SynapseTargetType m_connect_synapse;
};

std::ostream& operator<<(std::ostream&, CurrentDAC::Sign const&) SYMBOL_VISIBLE;


namespace detail {

template <>
struct BackendContainerTrait<CurrentDAC>
    : public BackendContainerBase<
          CurrentDAC,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

} // namespace detail

} // namespace haldls::vx
