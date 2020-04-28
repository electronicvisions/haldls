#pragma once

#include <algorithm>
#include <array>
#include <iterator>
#include <stdexcept>
#include <utility>

#include "haldls/has_local_data.h"
#include "haldls/is_leaf_node.h"
#include "hate/empty.h"

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
	void operator()(CoordinateT const& coord, ContainerT&)
	{
		static_assert(
		    !hate::is_empty_v<CoordinateT>,
		    "ReadAddressVisitor needs non-empty coordinate instance");
		if constexpr (
		    haldls::HasLocalData<hate::remove_empty_t<ContainerT>>::value ||
		    haldls::IsLeafNode<hate::remove_empty_t<ContainerT>>::value) {
			impl<CoordinateT, hate::remove_empty_t<std::remove_cv_t<ContainerT>>>(coord);
		}
	}

private:
	template <typename CoordinateT, typename ContainerT>
	auto impl(CoordinateT const& coord) -> decltype(ContainerT::addresses(coord), void())
	{
		auto const read_addresses = ContainerT::addresses(coord);
		addresses.insert(addresses.end(), read_addresses.begin(), read_addresses.end());
	}

	template <typename CoordinateT, typename ContainerT>
	auto impl(CoordinateT const& coord) -> decltype(ContainerT::read_addresses(coord), void())
	{
		auto const read_addresses = ContainerT::read_addresses(coord);
		addresses.insert(addresses.end(), read_addresses.begin(), read_addresses.end());
	}

	template <typename CoordinateT, typename ContainerT>
	auto impl(CoordinateT const& coord)
	    -> decltype(ContainerT::template addresses<typename T::value_type>(coord), void())
	{
		auto const read_addresses = ContainerT::template addresses<typename T::value_type>(coord);
		addresses.insert(addresses.end(), read_addresses.begin(), read_addresses.end());
	}

	template <typename CoordinateT, typename ContainerT>
	auto impl(CoordinateT const& coord)
	    -> decltype(ContainerT::template read_addresses<typename T::value_type>(coord), void())
	{
		auto const read_addresses =
		    ContainerT::template read_addresses<typename T::value_type>(coord);
		addresses.insert(addresses.end(), read_addresses.begin(), read_addresses.end());
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
	void operator()(CoordinateT const& coord, ContainerT&)
	{
		static_assert(
		    !hate::is_empty_v<CoordinateT>,
		    "WriteAddressVisitor needs non-empty coordinate instance");
		if constexpr (
		    haldls::HasLocalData<hate::remove_empty_t<ContainerT>>::value ||
		    haldls::IsLeafNode<hate::remove_empty_t<ContainerT>>::value) {
			impl<CoordinateT, hate::remove_empty_t<std::remove_cv_t<ContainerT>>>(coord);
		}
	}

private:
	template <typename CoordinateT, typename ContainerT>
	auto impl(CoordinateT const& coord) -> decltype(ContainerT::addresses(coord), void())
	{
		auto const write_addresses = ContainerT::addresses(coord);
		addresses.insert(addresses.end(), write_addresses.begin(), write_addresses.end());
	}

	template <typename CoordinateT, typename ContainerT>
	auto impl(CoordinateT const& coord) -> decltype(ContainerT::write_addresses(coord), void())
	{
		auto const write_addresses = ContainerT::write_addresses(coord);
		addresses.insert(addresses.end(), write_addresses.begin(), write_addresses.end());
	}

	template <typename CoordinateT, typename ContainerT>
	auto impl(CoordinateT const& coord)
	    -> decltype(ContainerT::template addresses<typename T::value_type>(coord), void())
	{
		auto const write_addresses = ContainerT::template addresses<typename T::value_type>(coord);
		addresses.insert(addresses.end(), write_addresses.begin(), write_addresses.end());
	}

	template <typename CoordinateT, typename ContainerT>
	auto impl(CoordinateT const& coord)
	    -> decltype(ContainerT::template write_addresses<typename T::value_type>(coord), void())
	{
		auto const write_addresses =
		    ContainerT::template write_addresses<typename T::value_type>(coord);
		addresses.insert(addresses.end(), write_addresses.begin(), write_addresses.end());
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
	void operator()(CoordinateT const& coordinate, ContainerT& config)
	{
		static_assert(
		    !hate::is_empty_v<ContainerT>, "DecodeVisitor needs non-empty container instance");
		impl(coordinate, config);
	}

private:
	template <typename CoordinateT, typename ContainerT>
	auto impl(CoordinateT const& coordinate, ContainerT& container)
	    -> decltype(&ContainerT::decode, void())
	{
		decode(coordinate, container, &ContainerT::decode);
	}

	template <typename CoordinateT, typename ContainerT>
	auto impl(CoordinateT const& coordinate, ContainerT& container)
	    -> decltype(&ContainerT::template decode<value_type>, void())
	{
		decode(coordinate, container, &ContainerT::template decode<value_type>);
	}

	template <typename CoordinateT, typename ContainerT>
	auto impl(CoordinateT const&, ContainerT const&) ->
	    typename std::enable_if<!ContainerT::has_local_data::value>::type
	{
		/* do nothing */
	}

	template <typename CoordinateT, typename ContainerT, typename DecodeContainerT, size_t N>
	void decode(
	    CoordinateT const&,
	    ContainerT& container,
	    void (DecodeContainerT::*decode)(std::array<value_type, N> const&))
	{
		(container.*decode)(slice<N>());
	}

	template <typename CoordinateT, typename ContainerT, typename DecodeContainerT, size_t N>
	void decode(
	    CoordinateT const& coordinate,
	    ContainerT& container,
	    void (DecodeContainerT::*decode)(CoordinateT const&, std::array<value_type, N> const&))
	{
		(container.*decode)(coordinate, slice<N>());
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
	void operator()(CoordinateT const& coordinate, ContainerT& config)
	{
		static_assert(
		    !hate::is_empty_v<ContainerT>, "EncodeVisitor needs non-empty container instance");
		impl(coordinate, config);
	}

private:
	template <typename CoordinateT, typename ContainerT>
	auto impl(CoordinateT const& coordinate, ContainerT const& container)
	    -> decltype(&ContainerT::encode, void())
	{
		encode(coordinate, container, &ContainerT::encode);
	}

	template <typename CoordinateT, typename ContainerT>
	auto impl(CoordinateT const& coordinate, ContainerT const& container)
	    -> decltype(&ContainerT::template encode<value_type>, void())
	{
		encode(coordinate, container, &ContainerT::template encode<value_type>);
	}

	template <typename CoordinateT, typename ContainerT>
	auto impl(CoordinateT const&, ContainerT const&) ->
	    typename std::enable_if<!ContainerT::has_local_data::value>::type
	{
		/* do nothing */
	}

	template <typename CoordinateT, typename ContainerT, typename EncodeContainerT, size_t N>
	void encode(
	    CoordinateT const&,
	    ContainerT const& container,
	    std::array<value_type, N> (EncodeContainerT::*encode)() const)
	{
		auto const words = (container.*encode)();
		m_data.insert(m_data.end(), words.begin(), words.end());
	}

	template <typename CoordinateT, typename ContainerT, typename EncodeContainerT, size_t N>
	void encode(
	    CoordinateT const& coordinate,
	    ContainerT const& container,
	    std::array<value_type, N> (EncodeContainerT::*encode)(CoordinateT const&) const)
	{
		auto const words = (container.*encode)(coordinate);
		m_data.insert(m_data.end(), words.begin(), words.end());
	}
};

} // namespace stadls
