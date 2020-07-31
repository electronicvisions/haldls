#include "stadls/vx/playback_program_builder.h"

#include <iostream>
#include <optional>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_set.hpp>

#include "fisch/vx/playback_program_builder.h"
#include "haldls/cerealization.h"
#include "haldls/vx/common.h"
#include "haldls/vx/is_readable.h"
#include "hate/type_traits.h"
#include "stadls/visitors.h"
#include "stadls/vx/dumper.h"
#include "stadls/vx/playback_program.h"

namespace stadls::vx::detail {

template <typename BuilderStorage, typename DoneType>
class PlaybackProgramBuilderAdapterImpl
{
public:
	template <typename T, size_t SupportedBackendIndex>
	static void write_table_entry(
	    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>& builder,
	    typename T::coordinate_type const& coord,
	    T const& config,
	    std::optional<T> const& config_reference);

	template <class T, size_t... SupportedBackendIndex>
	static void write_table_generator(
	    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>& builder,
	    typename T::coordinate_type const& coord,
	    T const& config,
	    size_t backend_index,
	    std::optional<T> const& config_reference,
	    std::index_sequence<SupportedBackendIndex...>);

	template <class T, size_t... SupportedBackendIndex>
	static PlaybackProgram::ContainerTicket<T> read_table_generator(
	    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>& builder,
	    typename T::coordinate_type const& coord,
	    size_t backend_index,
	    std::index_sequence<SupportedBackendIndex...>);
};

template <typename BuilderStorage, typename DoneType>
PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::PlaybackProgramBuilderAdapter() :
    m_builder_impl(std::make_unique<BuilderStorage>()), m_unsupported_targets()
{}

template <typename BuilderStorage, typename DoneType>
PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::PlaybackProgramBuilderAdapter(
    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>&& other) :
    m_builder_impl(std::move(other.m_builder_impl)),
    m_unsupported_targets(std::move(other.m_unsupported_targets))
{}

template <typename BuilderStorage, typename DoneType>
PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>&
PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::operator=(
    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>&& other)
{
	m_builder_impl = std::move(other.m_builder_impl);
	m_unsupported_targets = std::move(other.m_unsupported_targets);
	other.m_builder_impl = std::move(std::make_unique<BuilderStorage>());
	return *this;
}

template <typename BuilderStorage, typename DoneType>
PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::~PlaybackProgramBuilderAdapter()
{}

template <typename BuilderStorage, typename DoneType>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::wait_until(
    typename haldls::vx::Timer::coordinate_type const& coord, haldls::vx::Timer::Value const time)
{
	static bool first = true;
	if (first) {
		std::cerr << "wait_until(halco.TimerOnDLS(), value) is deprecated, use "
		             "block_until(halco.TimerOnDLS(), value) instead."
		          << std::endl;
		first = false;
	}
	block_until(coord, time);
}

template <typename BuilderStorage, typename DoneType>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::block_until(
    typename haldls::vx::Timer::coordinate_type const& coord, haldls::vx::Timer::Value const time)
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		m_builder_impl->write(
		    halco::hicann_dls::vx::WaitUntilOnFPGA(coord.toEnum()),
		    fisch::vx::WaitUntil(fisch::vx::WaitUntil::Value(time)));
	} else if constexpr (std::is_same_v<BuilderStorage, Dumper>) {
		m_builder_impl->block_until(coord, time);
	} else {
		throw std::logic_error("Unknown specialization");
	}
}

template <typename BuilderStorage, typename DoneType>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::block_until(
    typename halco::hicann_dls::vx::BarrierOnFPGA const& coord, haldls::vx::Barrier const barrier)
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		m_builder_impl->write(coord, barrier.encode());
	} else if constexpr (std::is_same_v<BuilderStorage, Dumper>) {
		m_builder_impl->block_until(coord, barrier);
	} else {
		throw std::logic_error("Unknown specialization");
	}
}

namespace detail {

template <typename T>
using unsupported_write_targets_checker = decltype(T::unsupported_write_targets);

template <typename T>
using unsupported_read_targets_checker = decltype(T::unsupported_read_targets);

template <typename T>
constexpr static auto has_unsupported_write_targets =
    hate::is_detected_v<unsupported_write_targets_checker, T>;

template <typename T>
constexpr static auto has_unsupported_read_targets =
    hate::is_detected_v<unsupported_read_targets_checker, T>;

} // namespace detail

template <typename BuilderStorage, typename DoneType>
template <typename T, size_t SupportedBackendIndex>
void PlaybackProgramBuilderAdapterImpl<BuilderStorage, DoneType>::write_table_entry(
    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>& builder,
    typename T::coordinate_type const& coord,
    T const& config,
    std::optional<T> const& config_reference)
{
	typedef typename hate::index_type_list_by_integer<
	    SupportedBackendIndex,
	    typename haldls::vx::detail::BackendContainerTrait<T>::container_list>::type
	    backend_container_type;

	typedef std::vector<typename backend_container_type::coordinate_type> addresses_type;
	addresses_type write_addresses;
	haldls::vx::visit_preorder(
	    config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});

	typedef std::vector<backend_container_type> words_type;
	words_type words;
	haldls::vx::visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{words});

	if (words.size() != write_addresses.size()) {
		throw std::logic_error("number of addresses and words do not match");
	}

	if constexpr (detail::has_unsupported_write_targets<T>) {
		builder.m_unsupported_targets.insert(T::unsupported_write_targets);
	}

	if (words.size() == 0) {
		throw std::runtime_error("Container not writeable.");
	}

	if (config_reference) {
		if constexpr (std::is_base_of<haldls::vx::DifferentialWriteTrait, T>::value) {
			words_type reference_words;
			haldls::vx::visit_preorder(
			    *config_reference, coord, stadls::EncodeVisitor<words_type>{reference_words});
			if (reference_words.size() != words.size()) {
				throw std::logic_error("number of words of container and reference do not match");
			}
			words_type reduced_words;
			addresses_type reduced_addresses;
			for (size_t i = 0; i < reference_words.size(); ++i) {
				if (reference_words[i] != words[i]) {
					reduced_words.push_back(words[i]);
					reduced_addresses.push_back(write_addresses[i]);
				}
			}
			builder.m_builder_impl->write(reduced_addresses, reduced_words);
		} else {
			throw std::logic_error("Container type does not support differential write.");
		}
	} else {
		builder.m_builder_impl->write(write_addresses, words);
	}
}

template <typename BuilderStorage, typename DoneType>
template <class T, size_t... SupportedBackendIndex>
void PlaybackProgramBuilderAdapterImpl<BuilderStorage, DoneType>::write_table_generator(
    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>& builder,
    typename T::coordinate_type const& coord,
    T const& config,
    size_t const backend_index,
    std::optional<T> const& config_reference,
    std::index_sequence<SupportedBackendIndex...>)
{
	std::array<
	    void (*)(
	        PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>&,
	        typename T::coordinate_type const&, T const&, std::optional<T> const&),
	    sizeof...(SupportedBackendIndex)>
	    write_table{write_table_entry<T, SupportedBackendIndex>...};
	write_table.at(backend_index)(builder, coord, config, config_reference);
}

#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	template <typename BuilderStorage, typename DoneType>                                          \
	void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::write(                           \
	    [[maybe_unused]] typename Type::coordinate_type const& coord,                              \
	    [[maybe_unused]] Type const& config, [[maybe_unused]] Type const& config_reference,        \
	    [[maybe_unused]] haldls::vx::Backend backend)                                              \
	{                                                                                              \
		if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {         \
			if (!haldls::vx::detail::BackendContainerTrait<Type>::valid(backend)) {                \
				throw std::runtime_error("Backend not supported for container type.");             \
			}                                                                                      \
			size_t const backend_index = static_cast<size_t>(                                      \
			    haldls::vx::detail::BackendContainerTrait<Type>::backend_index_lookup_table.at(    \
			        static_cast<size_t>(backend)));                                                \
			Impl::template write_table_generator<Type>(                                            \
			    *this, coord, config, backend_index, config_reference,                             \
			    std::make_index_sequence<                                                          \
			        hate::type_list_size<typename haldls::vx::detail::BackendContainerTrait<       \
			            Type>::container_list>::value>());                                         \
		} else if constexpr (std::is_same_v<BuilderStorage, Dumper>) {                             \
			throw std::runtime_error("Unsupported differential write to dumper backend.");         \
		} else {                                                                                   \
			throw std::logic_error("Unknown specialization");                                      \
		}                                                                                          \
	}                                                                                              \
	template <typename BuilderStorage, typename DoneType>                                          \
	void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::write(                           \
	    [[maybe_unused]] typename Type::coordinate_type const& coord,                              \
	    [[maybe_unused]] Type const& config, [[maybe_unused]] Type const& config_reference)        \
	{                                                                                              \
		if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {         \
			this->write(                                                                           \
			    coord, config, config_reference,                                                   \
			    haldls::vx::detail::BackendContainerTrait<Type>::default_backend);                 \
		} else if constexpr (std::is_same_v<BuilderStorage, Dumper>) {                             \
			throw std::runtime_error("Unsupported differential write to dumper backend.");         \
		} else {                                                                                   \
			throw std::logic_error("Unknown specialization");                                      \
		}                                                                                          \
	}                                                                                              \
	template <typename BuilderStorage, typename DoneType>                                          \
	void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::write(                           \
	    [[maybe_unused]] typename Type::coordinate_type const& coord,                              \
	    [[maybe_unused]] Type const& config, [[maybe_unused]] haldls::vx::Backend backend)         \
	{                                                                                              \
		if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {         \
			if (!haldls::vx::detail::BackendContainerTrait<Type>::valid(backend)) {                \
				throw std::runtime_error("Backend not supported for container type.");             \
			}                                                                                      \
			size_t const backend_index = static_cast<size_t>(                                      \
			    haldls::vx::detail::BackendContainerTrait<Type>::backend_index_lookup_table.at(    \
			        static_cast<size_t>(backend)));                                                \
			Impl::template write_table_generator<Type>(                                            \
			    *this, coord, config, backend_index, std::nullopt,                                 \
			    std::make_index_sequence<                                                          \
			        hate::type_list_size<typename haldls::vx::detail::BackendContainerTrait<       \
			            Type>::container_list>::value>());                                         \
		} else if constexpr (std::is_same_v<BuilderStorage, Dumper>) {                             \
			m_builder_impl->write(coord, config);                                                  \
		} else {                                                                                   \
			throw std::logic_error("Unknown specialization");                                      \
		}                                                                                          \
	}                                                                                              \
	template <typename BuilderStorage, typename DoneType>                                          \
	void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::write(                           \
	    typename Type::coordinate_type const& coord, Type const& config)                           \
	{                                                                                              \
		write(coord, config, haldls::vx::detail::BackendContainerTrait<Type>::default_backend);    \
	}
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/container.def"

template <typename BuilderStorage, typename DoneType>
template <class T, size_t... SupportedBackendIndex>
PlaybackProgram::ContainerTicket<T>
PlaybackProgramBuilderAdapterImpl<BuilderStorage, DoneType>::read_table_generator(
    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>& builder,
    typename T::coordinate_type const& coord,
    size_t const backend_index,
    std::index_sequence<SupportedBackendIndex...>)
{
	using namespace haldls::vx::detail;
	std::array<
	    PlaybackProgram::ContainerTicket<T> (*)(
	        PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>&,
	        typename T::coordinate_type const&),
	    sizeof...(SupportedBackendIndex)>
	    read_table{[](PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>& builder,
	                  typename T::coordinate_type const& coord)
	                   -> PlaybackProgram::ContainerTicket<T> {
		    typedef typename hate::index_type_list_by_integer<
		        SupportedBackendIndex,
		        typename haldls::vx::detail::BackendContainerTrait<T>::container_list>::type
		        backend_container_type;

		    if constexpr (detail::has_unsupported_read_targets<T>) {
			    builder.m_unsupported_targets.insert(T::unsupported_read_targets);
		    }

		    typedef std::vector<typename backend_container_type::coordinate_type> addresses_type;
		    addresses_type read_addresses;
		    {
			    auto config =
			        haldls::vx::detail::coordinate_to_container<typename T::coordinate_type, T>(
			            coord);
			    haldls::vx::visit_preorder(
			        config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		    }

		    if (read_addresses.size() == 0) {
			    throw std::runtime_error("Container not readable.");
		    }

		    auto ticket_impl = builder.m_builder_impl->read(read_addresses);
		    return PlaybackProgram::ContainerTicket<T>(coord, ticket_impl);
	    }...};

	return read_table.at(backend_index)(builder, coord);
}

#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	template <typename BuilderStorage, typename DoneType>                                          \
	PlaybackProgram::ContainerTicket<Type>                                                         \
	PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::read(                                 \
	    [[maybe_unused]] typename Type::coordinate_type const& coord,                              \
	    [[maybe_unused]] haldls::vx::Backend backend)                                              \
	{                                                                                              \
		if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {         \
			if (!haldls::vx::detail::BackendContainerTrait<Type>::valid(backend)) {                \
				throw std::runtime_error("Backend not supported for container type.");             \
			}                                                                                      \
                                                                                                   \
			size_t const backend_index = static_cast<size_t>(                                      \
			    haldls::vx::detail::BackendContainerTrait<Type>::backend_index_lookup_table.at(    \
			        static_cast<size_t>(backend)));                                                \
			return Impl::template read_table_generator<Type>(                                      \
			    *this, coord, backend_index,                                                       \
			    std::make_index_sequence<                                                          \
			        hate::type_list_size<typename haldls::vx::detail::BackendContainerTrait<       \
			            Type>::container_list>::value>());                                         \
		} else if constexpr (std::is_same_v<BuilderStorage, Dumper>) {                             \
			return m_builder_impl->template read<Type>(coord);                                     \
		} else {                                                                                   \
			throw std::logic_error("Unknown specialization");                                      \
		}                                                                                          \
	}                                                                                              \
	template <typename BuilderStorage, typename DoneType>                                          \
	PlaybackProgram::ContainerTicket<Type>                                                         \
	PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::read(                                 \
	    typename Type::coordinate_type const& coord)                                               \
	{                                                                                              \
		return this->read(                                                                         \
		    coord, haldls::vx::detail::BackendContainerTrait<Type>::default_backend);              \
	}
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/container.def"


template <typename BuilderStorage, typename DoneType>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::merge_back(
    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>& other)
{
	merge_back(std::forward<PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>>(other));
}

template <typename BuilderStorage, typename DoneType>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::merge_back(
    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>&& other)
{
	m_builder_impl->merge_back(*(other.m_builder_impl));
	m_unsupported_targets.merge(other.m_unsupported_targets);
}

template <typename BuilderStorage, typename DoneType>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::merge_back(BuilderStorage&& other)
{
	m_builder_impl->merge_back(other);
}

template <typename BuilderStorage, typename DoneType>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::merge_back(BuilderStorage& other)
{
	merge_back(std::forward<BuilderStorage>(other));
}

template <typename BuilderStorage, typename DoneType>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::copy_back(
    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType> const& other)
{
	m_builder_impl->copy_back(*(other.m_builder_impl));
	m_unsupported_targets.insert(
	    other.m_unsupported_targets.begin(), other.m_unsupported_targets.end());
}

template <typename BuilderStorage, typename DoneType>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::copy_back(BuilderStorage const& other)
{
	m_builder_impl->copy_back(other);
}

template <typename BuilderStorage, typename DoneType>
DoneType PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::done()
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		auto const program = PlaybackProgram(m_builder_impl->done(), m_unsupported_targets);
		m_unsupported_targets.clear();
		return program;
	} else if constexpr (std::is_same_v<BuilderStorage, Dumper>) {
		return m_builder_impl->done();
	} else {
		throw std::logic_error("Unknown specialization");
	}
}

template <typename BuilderStorage, typename DoneType>
std::ostream& operator<<(
    std::ostream& os, PlaybackProgramBuilderAdapter<BuilderStorage, DoneType> const& builder)
{
	os << *(builder.m_builder_impl);
	return os;
}

template <typename BuilderStorage, typename DoneType>
bool PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::empty() const
{
	return m_builder_impl->empty();
}

template <typename BuilderStorage, typename DoneType>
size_t PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::size_to_fpga() const
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		return m_builder_impl->size_to_fpga();
	} else if constexpr (std::is_same_v<BuilderStorage, Dumper>) {
		throw std::runtime_error("Dumper does not support size_to_fpga().");
	} else {
		throw std::logic_error("Unknown specialization");
	}
}

template <typename BuilderStorage, typename DoneType>
bool PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::is_write_only() const
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		return m_builder_impl->is_write_only();
	} else if constexpr (std::is_same_v<BuilderStorage, Dumper>) {
		/* we don't support reads in the Dumper backend */
		return true;
	} else {
		throw std::logic_error("Unknown specialization");
	}
}

template <typename BuilderStorage, typename DoneType>
template <typename Archive>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::serialize(
    [[maybe_unused]] Archive& ar, std::uint32_t const)
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		throw std::runtime_error(
		    "fisch-based PlaybackProgramBuilderAdapter doesn't support serialization");
	} else if constexpr (std::is_same_v<BuilderStorage, Dumper>) {
		ar(CEREAL_NVP(m_builder_impl));
		ar(CEREAL_NVP(m_unsupported_targets));
	} else {
		throw std::logic_error("Unknown specialization");
	}
}

template class PlaybackProgramBuilderAdapter<
    fisch::vx::PlaybackProgramBuilder,
    stadls::vx::PlaybackProgram>;
template class PlaybackProgramBuilderAdapter<stadls::vx::Dumper, stadls::vx::Dumper::done_type>;

template std::ostream& operator<<(
    std::ostream&,
    PlaybackProgramBuilderAdapter<
        fisch::vx::PlaybackProgramBuilder,
        stadls::vx::PlaybackProgram> const&);

template std::ostream& operator<<(
    std::ostream&,
    PlaybackProgramBuilderAdapter<stadls::vx::Dumper, stadls::vx::Dumper::done_type> const&);

} // namespace stadls::vx::detail

namespace stadls::vx {

PlaybackProgramBuilder convert_to_builder(PlaybackProgramBuilderDumper& dumper)
{
	return convert_to_builder(dumper.done());
}

PlaybackProgramBuilder convert_to_builder(PlaybackProgramBuilderDumper&& dumper)
{
	return convert_to_builder(dumper.done());
}

PlaybackProgramBuilder convert_to_builder(Dumper::done_type const& cocos)
{
	typedef hate::type_list<haldls::vx::Timer::Value, haldls::vx::Barrier> block_types;
	PlaybackProgramBuilder builder;
	for (auto const& coco : cocos) {
		std::visit(
		    [&builder](auto const& cc) {
			    auto const& [coord, config] = cc;
			    typedef std::remove_cv_t<std::remove_reference_t<decltype(config)>> config_type;
			    if constexpr (hate::is_in_type_list<config_type, block_types>::value) {
				    builder.block_until(coord, config);
			    } else {
				    builder.write(coord, config);
			    }
		    },
		    coco);
	}
	return builder;
}

} // namespace stadls::vx

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::PlaybackProgramBuilder)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::PlaybackProgramBuilderDumper)
