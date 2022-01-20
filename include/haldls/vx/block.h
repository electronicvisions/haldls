#pragma once
#include <array>
#include <ostream>

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/cerealization.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"
#include "hxcomm/vx/target.h"

namespace cereal {
class access;
} // namespace cereal

namespace haldls::vx GENPYBIND_TAG_HALDLS_VX {

class GENPYBIND(visible) PollingOmnibusBlockConfig
{
public:
	typedef halco::hicann_dls::vx::PollingOmnibusBlockConfigOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::hardware, hxcomm::vx::Target::simulation};

	typedef halco::hicann_dls::vx::OmnibusAddress Address GENPYBIND(visible);
	typedef fisch::vx::word_access_type::Omnibus::Word Value GENPYBIND(visible);

	/** Default constructor. */
	PollingOmnibusBlockConfig() SYMBOL_VISIBLE;

	GENPYBIND(setter_for(address))
	void set_address(Address value) SYMBOL_VISIBLE;
	GENPYBIND(getter_for(address))
	Address get_address() const SYMBOL_VISIBLE;

	GENPYBIND(setter_for(mask))
	void set_mask(Value value) SYMBOL_VISIBLE;
	GENPYBIND(getter_for(mask))
	Value get_mask() const SYMBOL_VISIBLE;

	GENPYBIND(setter_for(target))
	void set_target(Value value) SYMBOL_VISIBLE;
	GENPYBIND(getter_for(target))
	Value get_target() const SYMBOL_VISIBLE;

	bool operator==(PollingOmnibusBlockConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(PollingOmnibusBlockConfig const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PollingOmnibusBlockConfig const& config)
	    SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 3;
	static std::array<halco::hicann_dls::vx::OmnibusAddress, config_size_in_words> addresses(
	    coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::word_access_type::Omnibus, config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<fisch::vx::word_access_type::Omnibus, config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t);

	Address m_address;
	Value m_mask;
	Value m_target;
};

namespace detail {

template <>
struct BackendContainerTrait<PollingOmnibusBlockConfig>
    : public BackendContainerBase<PollingOmnibusBlockConfig, fisch::vx::word_access_type::Omnibus>
{};

} // namespace detail

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(PollingOmnibusBlockConfig)

/**
 * Container for polling block operation on a Omnibus address.
 * Compare given value from address in FPGA register file masked with expected target result.
 */
class GENPYBIND(visible) PollingOmnibusBlock
{
public:
	typedef halco::hicann_dls::vx::PollingOmnibusBlockOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Resolve block if value is true (*address & mask == target) or if value is false (*address &
	 * mask != target).
	 */
	PollingOmnibusBlock(bool enable_expects_equality = true) SYMBOL_VISIBLE;

	/**
	 * Get enable expects equality value.
	 * Resolve block if value is true (*address & mask == target) or if value is false (*address &
	 * mask != target).
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(enable_expects_equality))
	bool get_enable_expects_equality() const SYMBOL_VISIBLE;

	/**
	 * Get enable expects equality value.
	 * Resolve block if value is true (*address & mask == target) or if value is false (*address &
	 * mask != target).
	 * @return Boolean value
	 */
	GENPYBIND(setter_for(enable_expects_equality))
	void set_enable_expects_equality(bool value) SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PollingOmnibusBlock const& config)
	    SYMBOL_VISIBLE;

	bool operator==(PollingOmnibusBlock const& other) const SYMBOL_VISIBLE;
	bool operator!=(PollingOmnibusBlock const& other) const SYMBOL_VISIBLE;

	fisch::vx::word_access_type::PollingOmnibusBlock encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t);

	bool m_enable_expects_equality;
};

namespace detail {

template <>
struct BackendContainerTrait<PollingOmnibusBlock>
    : public BackendContainerBase<
          PollingOmnibusBlock,
          fisch::vx::word_access_type::PollingOmnibusBlock>
{};

} // namespace detail

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(PollingOmnibusBlock)

} // namespace haldls::vx
