#include "stadls/vx/dumper.h"


namespace stadls::vx::detail {

template <typename DoneType>
void Dumper<DoneType>::block_until(
    haldls::vx::BlockUntil::Coordinate const& coord, haldls::vx::BlockUntil const& condition)
{
	m_dumpit.values.push_back(std::pair{coord.clone(), condition.clone_block_until()});
}

template <typename DoneType>
void Dumper<DoneType>::write(
    haldls::vx::Container::Coordinate const& coord, haldls::vx::Container const& config)
{
	m_dumpit.values.push_back(std::pair{coord.clone(), config.clone_container()});
}

template <typename DoneType>
ContainerTicket Dumper<DoneType>::read(haldls::vx::Container::Coordinate const&)
{
	throw std::runtime_error("Dumper does not support reads.");
}

template <typename DoneType>
typename Dumper<DoneType>::done_type Dumper<DoneType>::done()
{
	decltype(m_dumpit) ret;
	ret = std::move(m_dumpit);
	return ret;
}

template <typename DoneType>
void Dumper<DoneType>::merge_back(Dumper& other)
{
	for (auto&& value : other.m_dumpit.values) {
		m_dumpit.values.emplace_back(std::move(value));
	}
	other.m_dumpit.values.clear();
}

template <typename DoneType>
void Dumper<DoneType>::merge_front(Dumper& other)
{
	Dumper::done_type tmp;
	std::swap(tmp, other.m_dumpit);
	for (auto const& value : tmp.values) {
		if (!value.first || !value.second) {
			throw std::logic_error("Unexpected access to moved-from object.");
		}
		m_dumpit.values.push_back(std::pair{value.first->clone(), value.second->clone_encodable()});
	}
	std::swap(m_dumpit, tmp);
}

template <typename DoneType>
void Dumper<DoneType>::copy_back(Dumper const& other)
{
	for (auto const& value : other.m_dumpit.values) {
		if (!value.first || !value.second) {
			throw std::logic_error("Unexpected access to moved-from object.");
		}
		m_dumpit.values.push_back(std::pair{value.first->clone(), value.second->clone_encodable()});
	}
}

template <typename DoneType>
bool Dumper<DoneType>::empty() const
{
	return m_dumpit.values.empty();
}

template <typename DoneType>
std::ostream& operator<<(std::ostream& os, Dumper<DoneType> const& builder)
{
	for (auto const& value : builder.m_dumpit.values) {
		if (!value.first || !value.second) {
			throw std::logic_error("Unexpected access to moved-from object.");
		}
		os << *std::get<0>(value) << " " << *std::get<1>(value) << "\n";
	}
	return os;
}

} // namespace stadls::vx
