#pragma once
#include "haldls/vx/block_until.h"
#include "haldls/vx/container.h"
#include "hate/visibility.h"
#include "stadls/vx/container_ticket.h"

namespace stadls::vx::detail {
template <typename>
class Dumper;
} // namespace stadls::vx::detail

namespace cereal {

template <typename Archive, typename DoneType>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, stadls::vx::detail::Dumper<DoneType>& value, std::uint32_t const version);

} // namespace cereal

namespace stadls::vx::detail {

template <typename T>
std::ostream& operator<<(std::ostream& os, Dumper<T> const& builder) SYMBOL_VISIBLE;

/** Dumper implements an interface derived from fisch::vx::PlaybackProgramBuilder.
 * However, it does work on haldls container/halco coordinate pairs instead of
 * fisch containers. Reads are not supported as there is currently no application;
 * writes are collected into a std container holding coordinate/container
 * pairs. The std container is returned by done().
 */
template <typename DoneType>
class Dumper
{
public:
	typedef DoneType done_type;

	/** Default constructor. */
	Dumper() = default;

	/** Destructor. */
	~Dumper() = default;

	/** Move constructor. */
	Dumper(Dumper&&) = default;

	/** Move assignment operator. */
	Dumper& operator=(Dumper&&) = default;

	/** Copy constructor forbidden. */
	Dumper(Dumper const&) = delete;

	/** Copy assignment forbidden. */
	Dumper& operator=(Dumper const&) = delete;

	/**
	 * Add instruction to block execution until specified condition is satisfied.
	 * @param coord Coordinate for which to block
	 * @param condition Condition to block execution for until satisfaction
	 */
	void block_until(
	    haldls::vx::BlockUntil::Coordinate const& coord, haldls::vx::BlockUntil const& condition);

	/**
	 * Add write instruction for container.
	 * @param coord Container coordinate
	 * @param config Container data
	 */
	void write(haldls::vx::Container::Coordinate const& coord, haldls::vx::Container const& config);

	/** We do not support read and throw at runtime. */
	ContainerTicket read(haldls::vx::Container::Coordinate const& coord);

	/**
	 * Return ordered list of write coordinate-container pairs.
	 * @return Ordered list of writes of coordinate-container pairs.
	 */
	done_type done();

	/**
	 * Print coordinate/container pairs added until now.
	 * @return Altered ostream
	 */
	template <typename T>
	friend std::ostream& operator<<(std::ostream& os, Dumper<T> const& builder);

	/**
	 * Merge other Dumper to the end of this instance.
	 * The moved-from builder is emptied during the process.
	 * @param other Builder to move to this instance at the back
	 */
	void merge_back(Dumper& other);

	/**
	 * Merge other PlaybackProgramBuilder to the beginning of this builder instance.
	 * The moved-from builder is emptied during the process.
	 * @param other Builder to move to this instance at the front
	 */
	void merge_front(Dumper& other);

	/**
	 * Copy other Dumper to the end of this instance.
	 * The copied-from builder is untouched during the process.
	 * @throws std::runtime_error On other builder not being write only
	 * @param other Builder to copy to this instance at the back
	 */
	void copy_back(Dumper const& other);

	/**
	 * Get whether builder is empty, i.e. no instructions are embodied.
	 * @return Boolean value
	 */
	bool empty() const;

private:
	template <class Archive, typename T>
	friend void ::cereal::serialize(Archive& ar, Dumper<T>& value, std::uint32_t const version);

	done_type m_dumpit;
};

} // namespace stadls::vx::detail
