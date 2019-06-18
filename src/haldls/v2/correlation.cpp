#include "haldls/v2/correlation.h"

#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/cerealization.h"

namespace haldls {
namespace v2 {

CorrelationConfig::CorrelationConfig() : m_sense_delay(0), m_reset_delay_1(0), m_reset_delay_2(0) {}

CorrelationConfig::Delay CorrelationConfig::get_sense_delay() const
{
	return m_sense_delay;
}

void CorrelationConfig::set_sense_delay(CorrelationConfig::Delay const& value)
{
	m_sense_delay = value;
}

CorrelationConfig::Delay CorrelationConfig::get_reset_delay_1() const
{
	return m_reset_delay_1;
}

void CorrelationConfig::set_reset_delay_1(CorrelationConfig::Delay const& value)
{
	m_reset_delay_1 = value;
}

CorrelationConfig::Delay CorrelationConfig::get_reset_delay_2() const
{
	return m_reset_delay_2;
}

void CorrelationConfig::set_reset_delay_2(CorrelationConfig::Delay const& value)
{
	m_reset_delay_2 = value;
}

bool CorrelationConfig::operator==(CorrelationConfig const& other) const
{
	return get_sense_delay() == other.get_sense_delay() &&
		   get_reset_delay_1() == other.get_reset_delay_1() &&
		   get_reset_delay_2() == other.get_reset_delay_2();
}

bool CorrelationConfig::operator!=(CorrelationConfig const& other) const
{
	return !(*this == other);
}

std::array<hardware_address_type, CorrelationConfig::config_size_in_words>
CorrelationConfig::addresses(coordinate_type const& /*unique*/) const
{
	return {{0x0c000000, 0x0C000001, 0x0C000002}};
}

std::array<hardware_word_type, CorrelationConfig::config_size_in_words> CorrelationConfig::encode()
	const
{
	return {{static_cast<hardware_word_type>(m_sense_delay),
	         static_cast<hardware_word_type>(m_reset_delay_1),
	         static_cast<hardware_word_type>(m_reset_delay_2)}};
}

void CorrelationConfig::decode(std::array<hardware_word_type, config_size_in_words> const& data) {
	m_sense_delay = Delay(data[0]);
	m_reset_delay_1 = Delay(data[1]);
	m_reset_delay_2 = Delay(data[2]);
}

template <class Archive>
void CorrelationConfig::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_sense_delay));
	ar(CEREAL_NVP(m_reset_delay_1));
	ar(CEREAL_NVP(m_reset_delay_2));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CorrelationConfig)

namespace detail {

template <class T>
CorrelationBlockBase<T>::CorrelationBlockBase()
{
}

namespace {

template <class T>
struct CorrelationBlockBitfield
{
	union
	{
		std::array<hardware_word_type, CorrelationBlockBase<T>::read_config_size_in_words> raw;
		// clang-format off
		struct __attribute__((packed)) {
			hardware_word_type correlation_value_3         : 8;
			hardware_word_type correlation_value_2         : 8;
			hardware_word_type correlation_value_1         : 8;
			hardware_word_type correlation_value_0         : 8;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	CorrelationBlockBitfield() { u.raw = {{0}}; }
	CorrelationBlockBitfield(
	    std::array<hardware_word_type, CorrelationBlockBase<T>::read_config_size_in_words> const& data)
	{
		u.raw = data;
	}

	void fill_block(typename T::block_type& block)
	{
		block.at(typename T::single_coordinate_type(0)) =
		    typename T::Correlation(u.m.correlation_value_0);
		block.at(typename T::single_coordinate_type(1)) =
		    typename T::Correlation(u.m.correlation_value_1);
		block.at(typename T::single_coordinate_type(2)) =
		    typename T::Correlation(u.m.correlation_value_2);
		block.at(typename T::single_coordinate_type(3)) =
		    typename T::Correlation(u.m.correlation_value_3);
	}
};

} // anonymous namespace

} // namespace detail

CausalCorrelationBlock::CausalCorrelationBlock() :
    detail::CorrelationBlockBase<CausalCorrelationBlock>(),
    m_correlations()
{
}

typename CausalCorrelationBlock::Correlation CausalCorrelationBlock::get_correlation(
    single_coordinate_type const& coord) const
{
	return m_correlations.at(coord);
}

bool CausalCorrelationBlock::operator==(CausalCorrelationBlock const& other) const
{
	return m_correlations == other.m_correlations;
}

bool CausalCorrelationBlock::operator!=(CausalCorrelationBlock const& other) const
{
	return !(*this == other);
}

std::array<hardware_address_type, CausalCorrelationBlock::write_config_size_in_words>
CausalCorrelationBlock::write_addresses(coordinate_type const& /*coord*/) const
{
	return {{}};
}

std::array<hardware_address_type, CausalCorrelationBlock::read_config_size_in_words>
CausalCorrelationBlock::read_addresses(coordinate_type const& block) const
{
	using namespace halco::hicann_dls::v2;
	hardware_address_type const base_address = 0x00018000;
	// correlation space is bloated by one probably because of legacy code
	hardware_address_type const address_offset =
	    (block.y() * NeuronOnDLS::size * (read_config_size_in_words + 1) /
	     single_coordinate_type::size) +
	    block.x();
	return {{base_address + address_offset}};
}

std::array<hardware_word_type, CausalCorrelationBlock::write_config_size_in_words>
CausalCorrelationBlock::encode() const
{
	return {{}};
}

void CausalCorrelationBlock::decode(
    std::array<hardware_word_type, CausalCorrelationBlock::read_config_size_in_words> const& data)
{
	detail::CorrelationBlockBitfield<CausalCorrelationBlock> bitfield(data);
	bitfield.fill_block(m_correlations);
}

template <class Archive>
void CausalCorrelationBlock::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_correlations));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CausalCorrelationBlock)

AcausalCorrelationBlock::AcausalCorrelationBlock() :
    detail::CorrelationBlockBase<AcausalCorrelationBlock>(),
    m_correlations()
{}

typename AcausalCorrelationBlock::Correlation AcausalCorrelationBlock::get_correlation(
    single_coordinate_type const& coord) const
{
	return m_correlations.at(coord);
}

bool AcausalCorrelationBlock::operator==(AcausalCorrelationBlock const& other) const
{
	return m_correlations == other.m_correlations;
}

bool AcausalCorrelationBlock::operator!=(AcausalCorrelationBlock const& other) const
{
	return !(*this == other);
}

std::array<hardware_address_type, AcausalCorrelationBlock::write_config_size_in_words>
AcausalCorrelationBlock::write_addresses(coordinate_type const& /*coord*/) const
{
	return {{}};
}

std::array<hardware_address_type, AcausalCorrelationBlock::read_config_size_in_words>
AcausalCorrelationBlock::read_addresses(coordinate_type const& block) const
{
	using namespace halco::hicann_dls::v2;
	hardware_address_type const base_address = 0x0001C000;
	// correlation space is bloated by one probably because of legacy code
	hardware_address_type const address_offset =
	    (block.y() * NeuronOnDLS::size * (read_config_size_in_words + 1) /
	     single_coordinate_type::size) +
	    block.x();
	return {{base_address + address_offset}};
}

std::array<hardware_word_type, AcausalCorrelationBlock::write_config_size_in_words>
AcausalCorrelationBlock::encode() const
{
	return {{}};
}

void AcausalCorrelationBlock::decode(
    std::array<hardware_word_type, AcausalCorrelationBlock::read_config_size_in_words> const& data)
{
	detail::CorrelationBlockBitfield<AcausalCorrelationBlock> bitfield(data);
	bitfield.fill_block(m_correlations);
}

template <class Archive>
void AcausalCorrelationBlock::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_correlations));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(AcausalCorrelationBlock)

} // namespace v2
} // namespace haldls
