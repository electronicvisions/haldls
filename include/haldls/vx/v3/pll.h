#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/pll.h"

namespace haldls::vx::v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using ADPLL GENPYBIND(visible) = haldls::vx::ADPLL;
using PLLSelfTest GENPYBIND(visible) = haldls::vx::PLLSelfTest;
using PLLSelfTestStatus GENPYBIND(visible) = haldls::vx::PLLSelfTestStatus;


/**
 * Container for configuration of the clock outputs of the PLL.
 */
class GENPYBIND(visible) PLLClockOutputBlock
{
public:
	class ClockOutput
	{
	public:
		/** Default construct PLL clock output. */
		ClockOutput() SYMBOL_VISIBLE;

		/**
		 * Get whether output is enabled.
		 * @return Boolean value
		 */
		GENPYBIND(getter_for(enable_output))
		bool get_enable_output() const SYMBOL_VISIBLE;

		/**
		 * Set enable value of output.
		 * @param value Boolean value to set
		 */
		GENPYBIND(setter_for(enable_output))
		void set_enable_output(bool value) SYMBOL_VISIBLE;

		/**
		 * Get whether the output is set to bypass mode. If enabled, the FPGA clock is
		 * passed through to the output.
		 * @return Boolean value
		 */
		GENPYBIND(getter_for(enable_bypass))
		bool get_enable_bypass() const SYMBOL_VISIBLE;

		/**
		 * Set enable value of the bypass mode. If enabled, the FPGA clock is passed through
		 * to the output.
		 * @param value Boolean value to set
		 */
		GENPYBIND(setter_for(enable_bypass))
		void set_enable_bypass(bool value) SYMBOL_VISIBLE;

		/**
		 * Get coordinate of selected ADPLL to route to output.
		 * @return coord Coordinate of ADPLL to select
		 */
		GENPYBIND(getter_for(select_adpll))
		halco::hicann_dls::vx::ADPLLOnDLS get_select_adpll() const SYMBOL_VISIBLE;

		/**
		 * Set select which ADPLL to route to output.
		 * @param coord Coordinate of ADPLL to select
		 */
		GENPYBIND(setter_for(select_adpll))
		void set_select_adpll(halco::hicann_dls::vx::ADPLLOnDLS const& coord) SYMBOL_VISIBLE;

		/**
		 * Get which output of selected ADPLL to route to output.
		 * @return ADPLL output
		 */
		GENPYBIND(getter_for(select_adpll_output))
		ADPLL::Output get_select_adpll_output() const SYMBOL_VISIBLE;

		/**
		 * Set which output of selected ADPLL to route to output.
		 * @param value ADPLL output
		 */
		GENPYBIND(setter_for(select_adpll_output))
		void set_select_adpll_output(ADPLL::Output const value) SYMBOL_VISIBLE;

		bool operator==(ClockOutput const& other) const SYMBOL_VISIBLE;
		bool operator!=(ClockOutput const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, ClockOutput const& config) SYMBOL_VISIBLE;

	private:
		friend struct cereal::access;
		template <typename Archive>
		void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

		bool m_enable_output;
		bool m_enable_bypass;
		halco::hicann_dls::vx::ADPLLOnDLS m_adpll;
		ADPLL::Output m_adpll_output;
	};

	typedef halco::hicann_dls::vx::PLLClockOutputBlockOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	PLLClockOutputBlock() SYMBOL_VISIBLE;

	/**
	 * Get clock output configuration.
	 * @param coord Coordinate of clock output to get
	 * @return Configuration of clock output
	 */
	ClockOutput const& get_clock_output(
	    halco::hicann_dls::vx::PLLClockOutputOnDLS const& coord) const SYMBOL_VISIBLE;

	/**
	 * Set clock output configuration.
	 * @param coord Coordinate of clock output to set
	 * @param config Configuration of clock output to set
	 */
	void set_clock_output(
	    halco::hicann_dls::vx::PLLClockOutputOnDLS const& coord,
	    ClockOutput const& config) SYMBOL_VISIBLE;

	/**
	 * Get SPL1 / Omnibus clock source.
	 * @return Clock source for SPL1 / Omnibus clock.
	 */
	GENPYBIND(getter_for(spl1_source))
	halco::hicann_dls::vx::PLLClockOutputOnDLS const& get_spl1_source() const SYMBOL_VISIBLE;

	/**
	 * Set SPL1 / Omnibus clock source.
	 * @param coord Coordinate of clock source for SPL1 / Omnibus.
	 */
	GENPYBIND(setter_for(spl1_source))
	void set_spl1_source(halco::hicann_dls::vx::PLLClockOutputOnDLS const& coord) SYMBOL_VISIBLE;

	bool operator==(PLLClockOutputBlock const& other) const SYMBOL_VISIBLE;
	bool operator!=(PLLClockOutputBlock const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PLLClockOutputBlock const& config)
	    SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& coord)
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	halco::common::typed_array<ClockOutput, halco::hicann_dls::vx::PLLClockOutputOnDLS> m_output;
	bool m_switch_spl1_to_madc;
};

} // namespace haldls::vx::v3

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<v3::PLLClockOutputBlock>
    : public BackendContainerBase<
          v3::PLLClockOutputBlock,
          fisch::vx::word_access_type::JTAGPLLRegister,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

} // namespace haldls::vx::detail

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::v3::PLLClockOutputBlock)
