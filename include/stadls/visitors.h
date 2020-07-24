#pragma once

#include <algorithm>
#include <array>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace stadls {

/// \brief Extract addresses for reading from hardware for the visited containers.
/// Each container can provide addresses via a `read_addresses` member function or via a
/// `addresses` member function in case the addresses for reading and writing are the same.
/// Containers that do not themselves contain data to be read from the hardware
/// (i.e. containers of containers) can alternatively be tagged via
/// \code
/// typedef std::false_type has_local_data;
/// \endcode
template <typename T>
struct ReadAddressVisitor
{
	T& addresses;

	template <typename CoordinateT, typename ContainerT>
	auto operator()(CoordinateT const& coord, ContainerT const& container)
		-> decltype(container.addresses(coord), void())
	{
		auto const read_addresses = container.addresses(coord);
		addresses.insert(addresses.end(), read_addresses.begin(), read_addresses.end());
	}

	template <typename CoordinateT, typename ContainerT>
	auto operator()(CoordinateT const& coord, ContainerT const& container)
		-> decltype(container.read_addresses(coord), void())
	{
		auto const read_addresses = container.read_addresses(coord);
		addresses.insert(addresses.end(), read_addresses.begin(), read_addresses.end());
	}

	template <typename CoordinateT, typename ContainerT>
	auto operator()(CoordinateT const& coord, ContainerT const& container)
	    -> decltype(container.template addresses<typename T::value_type>(coord), void())
	{
		auto const read_addresses = container.template addresses<typename T::value_type>(coord);
		addresses.insert(addresses.end(), read_addresses.begin(), read_addresses.end());
	}

	template <typename CoordinateT, typename ContainerT>
	auto operator()(CoordinateT const& coord, ContainerT const& container)
	    -> decltype(container.template read_addresses<typename T::value_type>(coord), void())
	{
		auto const read_addresses =
		    container.template read_addresses<typename T::value_type>(coord);
		addresses.insert(addresses.end(), read_addresses.begin(), read_addresses.end());
	}

	template <typename CoordinateT, typename ContainerT>
	auto operator()(CoordinateT const&, ContainerT const&) ->
		typename std::enable_if<!ContainerT::has_local_data::value>::type
	{
		/* do nothing */
	}
};

/// \brief Extract addresses for writing to hardware for the visited containers.
/// Each container can provide addresses via a `write_addresses` member function or via a
/// `addresses` member function in case the addresses for reading and writing are the same.
/// Containers that do not themselves contain data to be written to the hardware
/// (i.e. containers of containers) can alternatively be tagged via
/// \code
/// typedef std::false_type has_local_data;
/// \endcode
template <typename T>
struct WriteAddressVisitor
{
	T& addresses;

	template <typename CoordinateT, typename ContainerT>
	auto operator()(CoordinateT const& coord, ContainerT const& container)
		-> decltype(container.addresses(coord), void())
	{
		auto const write_addresses = container.addresses(coord);
		addresses.insert(addresses.end(), write_addresses.begin(), write_addresses.end());
	}

	template <typename CoordinateT, typename ContainerT>
	auto operator()(CoordinateT const& coord, ContainerT const& container)
		-> decltype(container.write_addresses(coord), void())
	{
		auto const write_addresses = container.write_addresses(coord);
		addresses.insert(addresses.end(), write_addresses.begin(), write_addresses.end());
	}

	template <typename CoordinateT, typename ContainerT>
	auto operator()(CoordinateT const& coord, ContainerT const& container)
	    -> decltype(container.template addresses<typename T::value_type>(coord), void())
	{
		auto const write_addresses = container.template addresses<typename T::value_type>(coord);
		addresses.insert(addresses.end(), write_addresses.begin(), write_addresses.end());
	}

	template <typename CoordinateT, typename ContainerT>
	auto operator()(CoordinateT const& coord, ContainerT const& container)
	    -> decltype(container.template write_addresses<typename T::value_type>(coord), void())
	{
		auto const write_addresses =
		    container.template write_addresses<typename T::value_type>(coord);
		addresses.insert(addresses.end(), write_addresses.begin(), write_addresses.end());
	}

	template <typename CoordinateT, typename ContainerT>
	auto operator()(CoordinateT const&, ContainerT const&) ->
		typename std::enable_if<!ContainerT::has_local_data::value>::type
	{
		/* do nothing */
	}
};

/// \brief Fill the visited containers by decoding the specified configuration data.
/// Each container should implement a `decode` member function that accepts an array of
/// words read from the hardware.  The first argument to the function can optionally be
/// the coordinate of the container, should it be required to correctly decode the data.
/// Containers that do not themselves contain data (i.e. containers of containers) can
/// alternatively be tagged via
/// \code
/// typedef std::false_type has_local_data;
/// \endcode
/// \see ReadAddressVisitor, which is used to extract the addresses to read the
///      configuration data from.
template <typename T>
class DecodeVisitor
{
	T m_data;
	typedef typename T::value_type value_type;
	typename T::const_iterator m_it;

public:
	DecodeVisitor(T data) : m_data(std::move(data)), m_it(m_data.cbegin()) {}

	template <typename CoordinateT, typename ContainerT>
	auto operator()(CoordinateT const& coord, ContainerT& container)
		-> decltype(&ContainerT::decode, void())
	{
		decode(coord, container, &ContainerT::decode);
	}

	template <typename CoordinateT, typename ContainerT>
	auto operator()(CoordinateT const& coord, ContainerT& container)
	    -> decltype(&ContainerT::template decode<value_type>, void())
	{
		decode(coord, container, &ContainerT::template decode<value_type>);
	}

	template <typename CoordinateT, typename ContainerT>
	auto operator()(CoordinateT const&, ContainerT const&) ->
		typename std::enable_if<!ContainerT::has_local_data::value>::type
	{
		/* do nothing */
	}

private:
	template <typename CoordinateT, typename ContainerT, typename DecodeContainerT, size_t N>
	void decode(
	    CoordinateT const& coord,
	    ContainerT& container,
	    void (DecodeContainerT::*decode)(CoordinateT const&, std::array<value_type, N> const&))
	{
		(container.*decode)(coord, slice<N>());
	}

	template <typename CoordinateT, typename ContainerT, typename DecodeContainerT, size_t N>
	void decode(
	    CoordinateT const&,
	    ContainerT& container,
	    void (DecodeContainerT::*decode)(std::array<value_type, N> const&))
	{
		(container.*decode)(slice<N>());
	}

	template <size_t N>
	auto slice() -> std::array<value_type, N>
	{
		if (N > remaining())
			throw std::runtime_error("end of buffer during decoding");

		std::array<value_type, N> buf;

		auto prev_it = m_it;
		std::advance(m_it, N);
		std::copy(prev_it, m_it, buf.begin());

		return buf;
	}

	size_t remaining() const { return std::distance(m_it, m_data.cend()); }
};

/// \brief Extract hardware configuration data for the visited containers.
/// Each container should implement an `encode` member function that returns an array of
/// words to be written to the hardware.  The first argument to the function can
/// optionally be the coordinate of the container, should it be required to correctly
/// encode the data.
/// Containers that do not themselves contain data (i.e. containers of containers) can
/// alternatively be tagged via
/// \code
/// typedef std::false_type has_local_data;
/// \endcode
/// \see WriteAddressVisitor, which is used to extract the addresses to write the
///      configuration data to.
template <typename T>
class EncodeVisitor
{
	T& m_data;
	typedef typename T::value_type value_type;

public:
	EncodeVisitor(T& data) : m_data(data) {}

	template <typename CoordinateT, typename ContainerT>
	auto operator()(CoordinateT const& coord, ContainerT const& container)
		-> decltype(&ContainerT::encode, void())
	{
		encode(coord, container, &ContainerT::encode);
	}

	template <typename CoordinateT, typename ContainerT>
	auto operator()(CoordinateT const& coord, ContainerT const& container)
	    -> decltype(&ContainerT::template encode<value_type>, void())
	{
		encode(coord, container, &ContainerT::template encode<value_type>);
	}

	template <typename CoordinateT, typename ContainerT>
	auto operator()(CoordinateT const&, ContainerT const&) ->
		typename std::enable_if<!ContainerT::has_local_data::value>::type
	{
		/* do nothing */
	}

private:
	template <typename CoordinateT, typename ContainerT, size_t N>
	void encode(
		CoordinateT const& coord,
		ContainerT const& container,
		std::array<value_type, N> (ContainerT::*encode)(CoordinateT const&) const)
	{
		auto const words = (container.*encode)(coord);
		m_data.insert(m_data.end(), words.begin(), words.end());
	}

	template <typename CoordinateT, typename ContainerT, size_t N>
	void encode(
		CoordinateT const&,
		ContainerT const& container,
		std::array<value_type, N> (ContainerT::*encode)() const)
	{
		auto const words = (container.*encode)();
		m_data.insert(m_data.end(), words.begin(), words.end());
	}

	template <typename CoordinateT, typename ContainerT>
	void encode(
	    CoordinateT const& coord,
	    ContainerT const& container,
	    decltype(container.template encode<value_type>(coord)) (ContainerT::*encode)(
	        CoordinateT const&) const)
	{
		auto const words = (container.*encode)(coord);
		m_data.insert(m_data.end(), words.begin(), words.end());
	}

	template <typename CoordinateT, typename ContainerT>
	void encode(
	    CoordinateT const&,
	    ContainerT const& container,
	    decltype(container.template encode<value_type>()) (ContainerT::*encode)() const)
	{
		auto const words = (container.*encode)();
		m_data.insert(m_data.end(), words.begin(), words.end());
	}
};

} // namespace stadls
