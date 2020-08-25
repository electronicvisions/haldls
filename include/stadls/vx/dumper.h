#pragma once
#include <memory>
#include <utility> // std::pair
#include <variant>
#include <vector>

#include "halco/hicann-dls/vx/barrier.h"
#include "haldls/vx/barrier.h"
#include "haldls/vx/container.h"
#include "hate/visibility.h"
#include "lola/vx/container.h"
#include "stadls/vx/genpybind.h"
#include "stadls/vx/playback_program.h"

// clang-format off
#if defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)
#include "haldls/cerealization.h"
#include "haldls/vx/pickle.h"
namespace py = pybind11;
#endif

namespace stadls::vx GENPYBIND_TAG_STADLS_VX {

struct GENPYBIND(visible) DumperDone
{
	/**
	 * Variant over all coordinate/container pairs
	 */
	typedef std::variant<
#define PLAYBACK_CONTAINER(Name, Type) std::pair<typename Type::coordinate_type, Type>,
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/container.def"
	    std::pair<typename haldls::vx::Timer::coordinate_type, haldls::vx::Timer::Value>,
	    std::pair<halco::hicann_dls::vx::BarrierOnFPGA, haldls::vx::Barrier> >
	    coco_type;
	typedef std::vector<coco_type> values_type;

	DumperDone() = default;

	values_type values{};

	bool operator==(DumperDone const& other) const SYMBOL_VISIBLE;
	bool operator!=(DumperDone const& other) const SYMBOL_VISIBLE;

	// clang-format off
	GENPYBIND_MANUAL({
		parent.def("__len__", [](GENPYBIND_PARENT_TYPE const& v) { return v.values.size(); })
			.def("tolist", [](GENPYBIND_PARENT_TYPE const& v) {
				py::list ret;
				for (auto const& vv : v.values) {
					py::object item;
					std::visit(
						[&item](auto const& vvv) {
							item = py::make_tuple(std::get<0>(vvv), std::get<1>(vvv));
						},
						vv);
					ret.append(item);
				}
				return ret;
				});
	})
	// clang-format on

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t);
};

/** Dumper implements an interface derived from fisch::vx::PlaybackProgramBuilder.
 * However, it does work on haldls container/halco coordinate pairs instead of
 * fisch containers. Reads are not supported as there is currently no application;
 * writes are collected into a std container holding coordinate/container
 * pairs. The std container is returned by done().
 */
class Dumper
{
public:
	typedef DumperDone done_type;

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
	 * Add instruction to block execution until specified timer has reached specified value.
	 * @param coord Timer coordinate for which to block
	 * @param time Timer value until which to block execution
	 */
	void block_until(
	    typename haldls::vx::Timer::coordinate_type const& coord, haldls::vx::Timer::Value time);

	/**
	 * Add instruction to block execution until specified barrier is completed.
	 * @param coord Barrier synchronization coordinate for which to block
	 * @param barrier Barrier synchronization value for to block execution
	 */
	void block_until(
	    halco::hicann_dls::vx::BarrierOnFPGA const& coord, haldls::vx::Barrier barrier);

	/**
	 * Add write instruction for container.
	 * @tparam ContainerT Container type
	 * @param coord Container coordinate
	 * @param config Container data
	 */
	template <typename ContainerT>
	void write(typename ContainerT::coordinate_type const& coord, ContainerT const& config);

	/** We do not support read and throw at runtime. */
	template <typename ContainerT>
	PlaybackProgram::ContainerTicket<ContainerT> read(
	    typename ContainerT::coordinate_type const& coord);

	/**
	 * Return ordered list of write coordinate-container pairs.
	 * @return Ordered list of writes of coordinate-container pairs.
	 */
	done_type done();

	/**
	 * Print coordinate/container pairs added until now.
	 * @return Altered ostream
	 */
	friend std::ostream& operator<<(std::ostream& os, Dumper const& builder);

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
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version);

	done_type m_dumpit;
};

GENPYBIND_MANUAL({
	haldls::vx::AddPickle<hate::type_list<stadls::vx::DumperDone>>::apply(parent, {"DumperDone"});

	::haldls::vx::WrapToFromFunctions<stadls::vx::DumperDone>::apply(parent);
})

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	extern template void Dumper::write<Type>(typename Type::coordinate_type const&, Type const&);  \
	extern template PlaybackProgram::ContainerTicket<Type> Dumper::read(                           \
	    typename Type::coordinate_type const&);
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/container.def"

} // namespace stadls::vx
