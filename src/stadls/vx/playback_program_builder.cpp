#include "stadls/vx/playback_program_builder.h"

#include <optional>
#include "fisch/vx/playback_program_builder.h"
#include "haldls/vx/common.h"
#include "haldls/vx/container.h"
#include "haldls/vx/is_readable.h"
#include "hate/type_traits.h"
#include "stadls/visitors.h"
#include "stadls/vx/playback_program.h"

namespace stadls::vx::detail {

template <typename BuilderStorage>
class PlaybackProgramBuilderAdapterImpl
{
public:
	template <typename T, size_t SupportedBackendIndex>
	static void write_table_entry(
	    PlaybackProgramBuilderAdapter<BuilderStorage>& builder,
	    typename T::coordinate_type const& coord,
	    T const& config,
	    std::optional<T> const& config_reference);

	template <class T, size_t... SupportedBackendIndex>
	static void write_table_generator(
	    PlaybackProgramBuilderAdapter<BuilderStorage>& builder,
	    typename T::coordinate_type const& coord,
	    T const& config,
	    size_t backend_index,
	    std::optional<T> const& config_reference,
	    std::index_sequence<SupportedBackendIndex...>);

	template <class T, size_t... SupportedBackendIndex>
	static PlaybackProgram::ContainerTicket<T> read_table_generator(
	    PlaybackProgramBuilderAdapter<BuilderStorage>& builder,
	    typename T::coordinate_type const& coord,
	    size_t backend_index,
	    std::index_sequence<SupportedBackendIndex...>);
};

template <typename BuilderStorage>
PlaybackProgramBuilderAdapter<BuilderStorage>::PlaybackProgramBuilderAdapter() :
    m_builder_impl(std::make_unique<BuilderStorage>()), m_unsupported_targets()
{}

template <typename BuilderStorage>
PlaybackProgramBuilderAdapter<BuilderStorage>::PlaybackProgramBuilderAdapter(
    PlaybackProgramBuilderAdapter<BuilderStorage>&& other) :
    m_builder_impl(std::move(other.m_builder_impl)),
    m_unsupported_targets(std::move(other.m_unsupported_targets))
{}

template <typename BuilderStorage>
PlaybackProgramBuilderAdapter<BuilderStorage>&
PlaybackProgramBuilderAdapter<BuilderStorage>::operator=(
    PlaybackProgramBuilderAdapter<BuilderStorage>&& other)
{
	m_builder_impl = std::move(other.m_builder_impl);
	m_unsupported_targets = std::move(other.m_unsupported_targets);
	other.m_builder_impl = std::move(std::make_unique<BuilderStorage>());
	return *this;
}

template <typename BuilderStorage>
PlaybackProgramBuilderAdapter<BuilderStorage>::~PlaybackProgramBuilderAdapter()
{}

template <typename BuilderStorage>
void PlaybackProgramBuilderAdapter<BuilderStorage>::wait_until(
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

template <typename BuilderStorage>
void PlaybackProgramBuilderAdapter<BuilderStorage>::block_until(
    typename haldls::vx::Timer::coordinate_type const& coord, haldls::vx::Timer::Value const time)
{
	m_builder_impl->write(
	    halco::hicann_dls::vx::WaitUntilOnFPGA(coord.toEnum()),
	    fisch::vx::WaitUntil(fisch::vx::WaitUntil::Value(time)));
}

template <typename BuilderStorage>
void PlaybackProgramBuilderAdapter<BuilderStorage>::block_until(
    typename halco::hicann_dls::vx::BarrierOnFPGA const& coord, haldls::vx::Barrier const sync)
{
	m_builder_impl->write(coord, sync.encode());
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

template <typename BuilderStorage>
template <typename T, size_t SupportedBackendIndex>
void PlaybackProgramBuilderAdapterImpl<BuilderStorage>::write_table_entry(
    PlaybackProgramBuilderAdapter<BuilderStorage>& builder,
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

template <typename BuilderStorage>
template <class T, size_t... SupportedBackendIndex>
void PlaybackProgramBuilderAdapterImpl<BuilderStorage>::write_table_generator(
    PlaybackProgramBuilderAdapter<BuilderStorage>& builder,
    typename T::coordinate_type const& coord,
    T const& config,
    size_t const backend_index,
    std::optional<T> const& config_reference,
    std::index_sequence<SupportedBackendIndex...>)
{
	std::array<
	    void (*)(
	        PlaybackProgramBuilderAdapter<BuilderStorage>&, typename T::coordinate_type const&,
	        T const&, std::optional<T> const&),
	    sizeof...(SupportedBackendIndex)>
	    write_table{write_table_entry<T, SupportedBackendIndex>...};
	write_table.at(backend_index)(builder, coord, config, config_reference);
}

#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	template <typename BuilderStorage>                                                             \
	void PlaybackProgramBuilderAdapter<BuilderStorage>::write(                                     \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference, haldls::vx::Backend backend)                                 \
	{                                                                                              \
		if (!haldls::vx::detail::BackendContainerTrait<Type>::valid(backend)) {                    \
			throw std::runtime_error("Backend not supported for container type.");                 \
		}                                                                                          \
		size_t const backend_index = static_cast<size_t>(                                          \
		    haldls::vx::detail::BackendContainerTrait<Type>::backend_index_lookup_table.at(        \
		        static_cast<size_t>(backend)));                                                    \
		Impl::template write_table_generator<Type>(                                                \
		    *this, coord, config, backend_index, config_reference,                                 \
		    std::make_index_sequence<                                                              \
		        hate::type_list_size<typename haldls::vx::detail::BackendContainerTrait<           \
		            Type>::container_list>::value>());                                             \
	}                                                                                              \
	template <typename BuilderStorage>                                                             \
	void PlaybackProgramBuilderAdapter<BuilderStorage>::write(                                     \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference)                                                              \
	{                                                                                              \
		this->write(                                                                               \
		    coord, config, config_reference,                                                       \
		    haldls::vx::detail::BackendContainerTrait<Type>::default_backend);                     \
	}                                                                                              \
	template <typename BuilderStorage>                                                             \
	void PlaybackProgramBuilderAdapter<BuilderStorage>::write(                                     \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    haldls::vx::Backend backend)                                                               \
	{                                                                                              \
		if (!haldls::vx::detail::BackendContainerTrait<Type>::valid(backend)) {                    \
			throw std::runtime_error("Backend not supported for container type.");                 \
		}                                                                                          \
		size_t const backend_index = static_cast<size_t>(                                          \
		    haldls::vx::detail::BackendContainerTrait<Type>::backend_index_lookup_table.at(        \
		        static_cast<size_t>(backend)));                                                    \
		Impl::template write_table_generator<Type>(                                                \
		    *this, coord, config, backend_index, std::nullopt,                                     \
		    std::make_index_sequence<                                                              \
		        hate::type_list_size<typename haldls::vx::detail::BackendContainerTrait<           \
		            Type>::container_list>::value>());                                             \
	}                                                                                              \
	template <typename BuilderStorage>                                                             \
	void PlaybackProgramBuilderAdapter<BuilderStorage>::write(                                     \
	    typename Type::coordinate_type const& coord, Type const& config)                           \
	{                                                                                              \
		write(coord, config, haldls::vx::detail::BackendContainerTrait<Type>::default_backend);    \
	}
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/container.def"

template <typename BuilderStorage>
template <class T, size_t... SupportedBackendIndex>
PlaybackProgram::ContainerTicket<T>
PlaybackProgramBuilderAdapterImpl<BuilderStorage>::read_table_generator(
    PlaybackProgramBuilderAdapter<BuilderStorage>& builder,
    typename T::coordinate_type const& coord,
    size_t const backend_index,
    std::index_sequence<SupportedBackendIndex...>)
{
	using namespace haldls::vx::detail;
	std::array<
	    PlaybackProgram::ContainerTicket<T> (*)(
	        PlaybackProgramBuilderAdapter<BuilderStorage>&, typename T::coordinate_type const&),
	    sizeof...(SupportedBackendIndex)>
	    read_table{[](PlaybackProgramBuilderAdapter<BuilderStorage>& builder,
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
	template <typename BuilderStorage>                                                             \
	PlaybackProgram::ContainerTicket<Type> PlaybackProgramBuilderAdapter<BuilderStorage>::read(    \
	    typename Type::coordinate_type const& coord, haldls::vx::Backend backend)                  \
	{                                                                                              \
		if (!haldls::vx::detail::BackendContainerTrait<Type>::valid(backend)) {                    \
			throw std::runtime_error("Backend not supported for container type.");                 \
		}                                                                                          \
                                                                                                   \
		size_t const backend_index = static_cast<size_t>(                                          \
		    haldls::vx::detail::BackendContainerTrait<Type>::backend_index_lookup_table.at(        \
		        static_cast<size_t>(backend)));                                                    \
		return Impl::template read_table_generator<Type>(                                          \
		    *this, coord, backend_index,                                                           \
		    std::make_index_sequence<                                                              \
		        hate::type_list_size<typename haldls::vx::detail::BackendContainerTrait<           \
		            Type>::container_list>::value>());                                             \
	}                                                                                              \
	template <typename BuilderStorage>                                                             \
	PlaybackProgram::ContainerTicket<Type> PlaybackProgramBuilderAdapter<BuilderStorage>::read(    \
	    typename Type::coordinate_type const& coord)                                               \
	{                                                                                              \
		return this->read(                                                                         \
		    coord, haldls::vx::detail::BackendContainerTrait<Type>::default_backend);              \
	}
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/container.def"

template <typename BuilderStorage>
void PlaybackProgramBuilderAdapter<BuilderStorage>::merge_back(
    PlaybackProgramBuilderAdapter<BuilderStorage>& other)
{
	merge_back(std::forward<PlaybackProgramBuilderAdapter<BuilderStorage>>(other));
}

template <typename BuilderStorage>
void PlaybackProgramBuilderAdapter<BuilderStorage>::merge_back(
    PlaybackProgramBuilderAdapter<BuilderStorage>&& other)
{
	m_builder_impl->merge_back(*(other.m_builder_impl));
	m_unsupported_targets.merge(other.m_unsupported_targets);
}

template <typename BuilderStorage>
void PlaybackProgramBuilderAdapter<BuilderStorage>::merge_back(BuilderStorage&& other)
{
	m_builder_impl->merge_back(other);
}

template <typename BuilderStorage>
void PlaybackProgramBuilderAdapter<BuilderStorage>::merge_back(BuilderStorage& other)
{
	merge_back(std::forward<BuilderStorage>(other));
}

template <typename BuilderStorage>
void PlaybackProgramBuilderAdapter<BuilderStorage>::copy_back(
    PlaybackProgramBuilderAdapter<BuilderStorage> const& other)
{
	m_builder_impl->copy_back(*(other.m_builder_impl));
	m_unsupported_targets.insert(
	    other.m_unsupported_targets.begin(), other.m_unsupported_targets.end());
}

template <typename BuilderStorage>
void PlaybackProgramBuilderAdapter<BuilderStorage>::copy_back(BuilderStorage const& other)
{
	m_builder_impl->copy_back(other);
}

template <typename BuilderStorage>
PlaybackProgram PlaybackProgramBuilderAdapter<BuilderStorage>::done()
{
	auto const program = PlaybackProgram(m_builder_impl->done(), m_unsupported_targets);
	m_unsupported_targets.clear();
	return program;
}

template <typename BuilderStorage>
std::ostream& operator<<(
    std::ostream& os, PlaybackProgramBuilderAdapter<BuilderStorage> const& builder)
{
	os << *(builder.m_builder_impl);
	return os;
}

template <typename BuilderStorage>
bool PlaybackProgramBuilderAdapter<BuilderStorage>::empty() const
{
	return m_builder_impl->empty();
}

template <typename BuilderStorage>
size_t PlaybackProgramBuilderAdapter<BuilderStorage>::size_to_fpga() const
{
	return m_builder_impl->size_to_fpga();
}

template <typename BuilderStorage>
bool PlaybackProgramBuilderAdapter<BuilderStorage>::is_write_only() const
{
	return m_builder_impl->is_write_only();
}

template class PlaybackProgramBuilderAdapter<fisch::vx::PlaybackProgramBuilder>;

template std::ostream& operator<<(
    std::ostream& os,
    PlaybackProgramBuilderAdapter<fisch::vx::PlaybackProgramBuilder> const& builder);

} // namespace stadls::vx::detail
