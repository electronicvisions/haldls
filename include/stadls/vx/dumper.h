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

#define GENPYBIND_TAG_STADLS_VX_HIDDEN GENPYBIND(tag(stadls_vx_hidden))
namespace stadls::vx::dumper_done GENPYBIND_TAG_STADLS_VX_HIDDEN {
// Workaround for genpybind bug #3622:
// We have to hide the real type from genpybind b/c of missing namespacing when
// generating wrapper code => use "opaque" dummy type and expose manually below.
#if defined(__GENPYBIND__) && !defined(__GENPYBIND_GENERATED__)
struct GENPYBIND(hidden) type
{
	size_t size() const;
	int* begin();
	int* begin() const;
	int* end();
	int* end() const;
};
#else
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
typedef std::vector<coco_type> type;
#endif
} // namespace stadls::vx::dumper_done

namespace stadls::vx GENPYBIND_TAG_STADLS_VX {

/** Dumper implements an interface derived from fisch::vx::PlaybackProgramBuilder.
 * However, it does work on haldls container/halco coordinate pairs instead of
 * fisch containers. Reads are not supported as there is currently no application;
 * writes are collected into a std container holding coordinate/container
 * pairs. The std container is returned by done().
 */
class GENPYBIND(visible) SYMBOL_VISIBLE Dumper
{
public:
	/** Manual wrapping done_type to python.
	 */
	typedef dumper_done::type done_type;
	// clang-format off
	GENPYBIND_MANUAL({
		parent->py::template class_<stadls::vx::Dumper::done_type>(parent, "done_type")
			.def(py::init<>())
			.def("__len__", [](stadls::vx::Dumper::done_type const& v) { return v.size(); })
			.def("tolist", [](stadls::vx::Dumper::done_type const& v) {
				py::list ret;
				for (auto const& vv : v) {
					py::object item;
					std::visit(
						[&item](auto const& vvv) {
							item = py::make_tuple(std::get<0>(vvv), std::get<1>(vvv));
						},
						vv);
					ret.append(item);
				}
				return ret;
				})
		;
	})
	// clang-format on

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
	GENPYBIND(stringstream)
	SYMBOL_VISIBLE
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
	done_type m_dumpit;
};

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	extern template void Dumper::write<Type>(typename Type::coordinate_type const&, Type const&);  \
	extern template PlaybackProgram::ContainerTicket<Type> Dumper::read(                           \
	    typename Type::coordinate_type const&);
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/container.def"

} // namespace stadls::vx

PYBIND11_MAKE_OPAQUE(stadls::vx::Dumper::done_type)
