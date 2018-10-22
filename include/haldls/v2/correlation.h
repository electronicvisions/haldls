#pragma once

#include <ostream>

#include "halco/common/genpybind.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/v2/coordinates.h"

#include "hate/visibility.h"
#include "haldls/v2/common.h"

#include "haldls/cerealization.h"

namespace haldls {
namespace v2 GENPYBIND(tag(haldls_v2)) {

class GENPYBIND(visible) CorrelationConfig
{
public:
	typedef halco::hicann_dls::v2::CorrelationConfigOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) Delay
		: public halco::common::detail::RantWrapper<Delay, uint_fast16_t, 4095, 0>
	{
		constexpr explicit Delay(uintmax_t const val = 0) SYMBOL_VISIBLE : rant_t(val) {}
	};

	CorrelationConfig() SYMBOL_VISIBLE;

	Delay get_sense_delay() const SYMBOL_VISIBLE;
	void set_sense_delay(Delay const& value) SYMBOL_VISIBLE;

	Delay get_reset_delay_1() const SYMBOL_VISIBLE;
	void set_reset_delay_1(Delay const& value) SYMBOL_VISIBLE;

	Delay get_reset_delay_2() const SYMBOL_VISIBLE;
	void set_reset_delay_2(Delay const& value) SYMBOL_VISIBLE;

	bool operator==(CorrelationConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(CorrelationConfig const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 3;
	std::array<hardware_address_type, config_size_in_words> addresses(
		coordinate_type const& unique) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<hardware_word_type, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<hardware_word_type, config_size_in_words> const& data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	Delay m_sense_delay;
	Delay m_reset_delay_1;
	Delay m_reset_delay_2;
};

namespace detail {


/**
 * Base class for a-/causal correlation block
 * Causal and acausal correlation identical besides address space
 */
template <class T>
class SYMBOL_VISIBLE CorrelationBlockBase
{
public:
	typedef halco::hicann_dls::v2::SynapseBlockOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	CorrelationBlockBase() SYMBOL_VISIBLE;

	///  Ranged type for correlation values (0-255)
	struct GENPYBIND(inline_base("*")) Correlation
		: public halco::common::detail::RantWrapper<Correlation, uint_fast16_t, 255, 0>
	{
		constexpr explicit Correlation(uintmax_t const val = 0) SYMBOL_VISIBLE
			: halco::common::detail::RantWrapper<Correlation, uint_fast16_t, 255, 0>::rant_t(val)
		{
		}
	};

	/**
	 * get correlation value of synapse (read only property)
	 *
	 * @param coord SynapseOnSynapsBlock address
	 * @return Correlation
	 */
	Correlation get_correlation(halco::hicann_dls::v2::SynapseOnSynapseBlock const& coord) const
		SYMBOL_VISIBLE;

	bool operator==(CorrelationBlockBase const& other) const SYMBOL_VISIBLE;
	bool operator!=(CorrelationBlockBase const& other) const SYMBOL_VISIBLE;

	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;
	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 1;
	std::array<hardware_address_type, write_config_size_in_words> write_addresses(
		coordinate_type const& coord) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<hardware_address_type, read_config_size_in_words> read_addresses(
		coordinate_type const& coord) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<hardware_word_type, write_config_size_in_words> encode() const SYMBOL_VISIBLE
		GENPYBIND(hidden);
	void decode(std::array<hardware_word_type, read_config_size_in_words> const& data)
		SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	halco::common::typed_array<Correlation, halco::hicann_dls::v2::SynapseOnSynapseBlock>
		m_correlations;
protected:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;
};

} // namespace detail

/**
 *  @brief Container representing a block of causal correlation values (read only).
 *  4 causal correlation values are packed in one 32 bit word, therefore the
 *  smallest independent accessible unit is a block of 4 values. Values can only be read not written
 */
class SYMBOL_VISIBLE GENPYBIND(visible, hide_base("*CorrelationBlockBase*")) CausalCorrelationBlock
	: public detail::CorrelationBlockBase<CausalCorrelationBlock>
{
public:
	CausalCorrelationBlock() SYMBOL_VISIBLE;

	//FIXME: fix Visitor to not require overloaded en/decode function of child classes, see issue #2992
	std::array<hardware_word_type, write_config_size_in_words> encode() const SYMBOL_VISIBLE
		GENPYBIND(hidden);
	void decode(std::array<hardware_word_type, read_config_size_in_words> const& data)
		SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class detail::CorrelationBlockBase<CausalCorrelationBlock>;
	std::array<hardware_address_type, write_config_size_in_words> write_addresses_implementation()
		const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<hardware_address_type, read_config_size_in_words> read_addresses_implementation(
		coordinate_type const& coord) const SYMBOL_VISIBLE GENPYBIND(hidden);

	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;
};

/**
 *  @brief Container representing a block of acausal correlation values (read only).
 *  4 acausal correlation values are packed in one 32 bit word, therefore the
 *  smallest independent accessible unit is a block of 4 values. Values can only be read not written
 */
class SYMBOL_VISIBLE GENPYBIND(visible, hide_base("*CorrelationBlockBase*")) AcausalCorrelationBlock
	: public detail::CorrelationBlockBase<AcausalCorrelationBlock>
{
public:
	AcausalCorrelationBlock() SYMBOL_VISIBLE;

	//FIXME: fix Visitor to not require overloaded en/decode function of child classes, see issue #2992
	std::array<hardware_word_type, write_config_size_in_words> encode() const SYMBOL_VISIBLE
		GENPYBIND(hidden);
	void decode(std::array<hardware_word_type, read_config_size_in_words> const& data)
		SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class detail::CorrelationBlockBase<AcausalCorrelationBlock>;
	std::array<hardware_address_type, write_config_size_in_words> write_addresses_implementation()
		const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<hardware_address_type, read_config_size_in_words> read_addresses_implementation(
		coordinate_type const& coord) const SYMBOL_VISIBLE GENPYBIND(hidden);

	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;
};

template class detail::CorrelationBlockBase<CausalCorrelationBlock>;
template class detail::CorrelationBlockBase<AcausalCorrelationBlock>;

} // namespace v2
} // namespace haldls


namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::v2::CorrelationConfig::Delay)
HALCO_GEOMETRY_HASH_CLASS(haldls::v2::CausalCorrelationBlock::Correlation)
HALCO_GEOMETRY_HASH_CLASS(haldls::v2::AcausalCorrelationBlock::Correlation)

} // namespace std
