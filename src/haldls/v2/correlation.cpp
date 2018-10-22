#include "haldls/v2/correlation.h"

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
void CorrelationConfig::cerealize(Archive& ar) {
	ar(CEREAL_NVP(m_sense_delay));
	ar(CEREAL_NVP(m_reset_delay_1));
	ar(CEREAL_NVP(m_reset_delay_2));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CorrelationConfig)

namespace detail {

template <class T>
CorrelationBlockBase<T>::CorrelationBlockBase() : m_correlations()
{
}

template <class T>
typename CorrelationBlockBase<T>::Correlation CorrelationBlockBase<T>::get_correlation(
    halco::hicann_dls::v2::SynapseOnSynapseBlock const& coord) const
{
	return m_correlations.at(coord);
}

template <class T>
bool CorrelationBlockBase<T>::operator==(CorrelationBlockBase<T> const& other) const
{
	return m_correlations == other.m_correlations;
}

template <class T>
bool CorrelationBlockBase<T>::operator!=(CorrelationBlockBase<T> const& other) const
{
	return !(*this == other);
}

template <class T>
std::array<hardware_address_type, CorrelationBlockBase<T>::write_config_size_in_words>
CorrelationBlockBase<T>::write_addresses(CorrelationBlockBase<T>::coordinate_type const& /*block*/) const
{
	return static_cast<const T*>(this)->write_addresses_implementation();
}

template <class T>
std::array<hardware_address_type, CorrelationBlockBase<T>::read_config_size_in_words>
CorrelationBlockBase<T>::read_addresses(CorrelationBlockBase<T>::coordinate_type const& block) const
{
	return static_cast<const T*>(this)->read_addresses_implementation(block);
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
};

} // anonymous namespace

template <class T>
std::array<hardware_word_type, CorrelationBlockBase<T>::write_config_size_in_words>
CorrelationBlockBase<T>::encode() const
{
	return {{}};
}

template <class T>
void CorrelationBlockBase<T>::decode(
    std::array<hardware_word_type, CorrelationBlockBase<T>::read_config_size_in_words> const& data)
{
	using namespace halco::hicann_dls::v2;
	CorrelationBlockBitfield<T> bitfield(data);

	m_correlations.at(SynapseOnSynapseBlock(0)) =
	    CorrelationBlockBase<T>::Correlation(bitfield.u.m.correlation_value_0);
	m_correlations.at(SynapseOnSynapseBlock(1)) =
	    CorrelationBlockBase<T>::Correlation(bitfield.u.m.correlation_value_1);
	m_correlations.at(SynapseOnSynapseBlock(2)) =
	    CorrelationBlockBase<T>::Correlation(bitfield.u.m.correlation_value_2);
	m_correlations.at(SynapseOnSynapseBlock(3)) =
	    CorrelationBlockBase<T>::Correlation(bitfield.u.m.correlation_value_3);
}

template <class T>
template <class Archive>
void CorrelationBlockBase<T>::cerealize(Archive& ar) {
	ar(CEREAL_NVP(m_correlations));
}

} // namespace detail

CausalCorrelationBlock::CausalCorrelationBlock()
    : detail::CorrelationBlockBase<CausalCorrelationBlock>()
{
}

std::array<hardware_address_type, CausalCorrelationBlock::write_config_size_in_words>
CausalCorrelationBlock::write_addresses_implementation() const
{
	return {{}};
}

std::array<hardware_address_type, CausalCorrelationBlock::read_config_size_in_words>
CausalCorrelationBlock::read_addresses_implementation(
    CausalCorrelationBlock::coordinate_type const& block) const
{
	using namespace halco::hicann_dls::v2;
	hardware_address_type const base_address = 0x00018000;
	// correlation space is bloated by one probably because of legacy code
	hardware_address_type const address_offset =
	    (block.y() * NeuronOnDLS::size * (read_config_size_in_words + 1) /
	     SynapseOnSynapseBlock::size) +
	    block.x();
	return {{base_address + address_offset}};
}

std::array<hardware_word_type, CausalCorrelationBlock::write_config_size_in_words>
CausalCorrelationBlock::encode() const
{
	return detail::CorrelationBlockBase<CausalCorrelationBlock>::encode();
}

void CausalCorrelationBlock::decode(
    std::array<hardware_word_type, CausalCorrelationBlock::read_config_size_in_words> const& data)
{
	return detail::CorrelationBlockBase<CausalCorrelationBlock>::decode(data);
}

template <class Archive>
void CausalCorrelationBlock::cerealize(Archive& ar)
{
	CorrelationBlockBase<CausalCorrelationBlock>::cerealize<Archive>(ar);
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(detail::CorrelationBlockBase<CausalCorrelationBlock>)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CausalCorrelationBlock)

AcausalCorrelationBlock::AcausalCorrelationBlock()
    : detail::CorrelationBlockBase<AcausalCorrelationBlock>()
{
}

std::array<hardware_address_type, AcausalCorrelationBlock::write_config_size_in_words>
AcausalCorrelationBlock::write_addresses_implementation() const
{
	return {{}};
}

std::array<hardware_address_type, AcausalCorrelationBlock::read_config_size_in_words>
AcausalCorrelationBlock::read_addresses_implementation(
    AcausalCorrelationBlock::coordinate_type const& block) const
{
	using namespace halco::hicann_dls::v2;
	hardware_address_type const base_address = 0x0001C000;
	// correlation space is bloated by one probably because of legacy code
	hardware_address_type const address_offset =
	    (block.y() * NeuronOnDLS::size * (read_config_size_in_words + 1) /
	     SynapseOnSynapseBlock::size) +
	    block.x();
	return {{base_address + address_offset}};
}

std::array<hardware_word_type, AcausalCorrelationBlock::write_config_size_in_words>
AcausalCorrelationBlock::encode() const
{
	return detail::CorrelationBlockBase<AcausalCorrelationBlock>::encode();
}

void AcausalCorrelationBlock::decode(
    std::array<hardware_word_type, AcausalCorrelationBlock::read_config_size_in_words> const& data)
{
	return detail::CorrelationBlockBase<AcausalCorrelationBlock>::decode(data);
}

template <class Archive>
void AcausalCorrelationBlock::cerealize(Archive& ar)
{
	CorrelationBlockBase<AcausalCorrelationBlock>::cerealize<Archive>(ar);
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(detail::CorrelationBlockBase<AcausalCorrelationBlock>)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(AcausalCorrelationBlock)

} // namespace v2
} // namespace haldls
