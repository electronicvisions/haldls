#include "stadls/vx/playback_program_builder.h"

#include <optional>
#include "fisch/vx/playback_program_builder.h"
#include "haldls/vx/common.h"
#include "haldls/vx/is_readable.h"
#include "hate/type_traits.h"
#include "stadls/visitors.h"
#include "stadls/vx/playback_program.h"

namespace stadls::vx {

PlaybackProgramBuilder::PlaybackProgramBuilder() :
    m_builder_impl(std::make_unique<fisch::vx::PlaybackProgramBuilder>()), m_unsupported_targets()
{}

PlaybackProgramBuilder::PlaybackProgramBuilder(PlaybackProgramBuilder&& other) :
    m_builder_impl(std::move(other.m_builder_impl)),
    m_unsupported_targets(std::move(other.m_unsupported_targets))
{}

PlaybackProgramBuilder& PlaybackProgramBuilder::operator=(PlaybackProgramBuilder&& other)
{
	m_builder_impl = std::move(other.m_builder_impl);
	m_unsupported_targets = std::move(other.m_unsupported_targets);
	other.m_builder_impl = std::move(std::make_unique<fisch::vx::PlaybackProgramBuilder>());
	return *this;
}

PlaybackProgramBuilder::~PlaybackProgramBuilder() {}

void PlaybackProgramBuilder::wait_until(
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

void PlaybackProgramBuilder::block_until(
    typename haldls::vx::Timer::coordinate_type const& coord, haldls::vx::Timer::Value const time)
{
	m_builder_impl->write(
	    halco::hicann_dls::vx::WaitUntilOnFPGA(coord.toEnum()),
	    fisch::vx::WaitUntil(fisch::vx::WaitUntil::Value(time)));
}

void PlaybackProgramBuilder::block_until(
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

template <typename T, size_t SupportedBackendIndex>
void PlaybackProgramBuilder::write_table_entry(
    PlaybackProgramBuilder& builder,
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

template <typename T, size_t... SupportedBackendIndex>
void PlaybackProgramBuilder::write_table_generator(
    typename T::coordinate_type const& coord,
    T const& config,
    size_t const backend_index,
    std::optional<T> const& config_reference,
    std::index_sequence<SupportedBackendIndex...>)
{
	std::array<
	    void (*)(
	        PlaybackProgramBuilder&, typename T::coordinate_type const&, T const&,
	        std::optional<T> const&),
	    sizeof...(SupportedBackendIndex)>
	    write_table{write_table_entry<T, SupportedBackendIndex>...};

	write_table.at(backend_index)(*this, coord, config, config_reference);
}

#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	void PlaybackProgramBuilder::write(                                                            \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference, haldls::vx::Backend backend)                                 \
	{                                                                                              \
		if (!haldls::vx::detail::BackendContainerTrait<Type>::valid(backend)) {                    \
			throw std::runtime_error("Backend not supported for container type.");                 \
		}                                                                                          \
		size_t const backend_index = static_cast<size_t>(                                          \
		    haldls::vx::detail::BackendContainerTrait<Type>::backend_index_lookup_table.at(        \
		        static_cast<size_t>(backend)));                                                    \
		write_table_generator<Type>(                                                               \
		    coord, config, backend_index, config_reference,                                        \
		    std::make_index_sequence<                                                              \
		        hate::type_list_size<typename haldls::vx::detail::BackendContainerTrait<           \
		            Type>::container_list>::value>());                                             \
	}                                                                                              \
	void PlaybackProgramBuilder::write(                                                            \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    Type const& config_reference)                                                              \
	{                                                                                              \
		write(                                                                                     \
		    coord, config, config_reference,                                                       \
		    haldls::vx::detail::BackendContainerTrait<Type>::default_backend);                     \
	}                                                                                              \
	void PlaybackProgramBuilder::write(                                                            \
	    typename Type::coordinate_type const& coord, Type const& config,                           \
	    haldls::vx::Backend backend)                                                               \
	{                                                                                              \
		if (!haldls::vx::detail::BackendContainerTrait<Type>::valid(backend)) {                    \
			throw std::runtime_error("Backend not supported for container type.");                 \
		}                                                                                          \
		size_t const backend_index = static_cast<size_t>(                                          \
		    haldls::vx::detail::BackendContainerTrait<Type>::backend_index_lookup_table.at(        \
		        static_cast<size_t>(backend)));                                                    \
		write_table_generator<Type>(                                                               \
		    coord, config, backend_index, std::nullopt,                                            \
		    std::make_index_sequence<                                                              \
		        hate::type_list_size<typename haldls::vx::detail::BackendContainerTrait<           \
		            Type>::container_list>::value>());                                             \
	}                                                                                              \
	void PlaybackProgramBuilder::write(                                                            \
	    typename Type::coordinate_type const& coord, Type const& config)                           \
	{                                                                                              \
		write(coord, config, haldls::vx::detail::BackendContainerTrait<Type>::default_backend);    \
	}
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/container.def"

template <class T, size_t... SupportedBackendIndex>
PlaybackProgram::ContainerTicket<T> PlaybackProgramBuilder::read_table_generator(
    typename T::coordinate_type const& coord,
    size_t backend_index,
    std::index_sequence<SupportedBackendIndex...>)
{
	using namespace haldls::vx::detail;
	std::array<
	    PlaybackProgram::ContainerTicket<T> (*)(
	        PlaybackProgramBuilder&, typename T::coordinate_type const&),
	    sizeof...(SupportedBackendIndex)>
	    read_table{[](PlaybackProgramBuilder& builder, typename T::coordinate_type const& coord)
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

	return read_table.at(backend_index)(*this, coord);
}

#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	PlaybackProgram::ContainerTicket<Type> PlaybackProgramBuilder::read(                           \
	    typename Type::coordinate_type const& coord, haldls::vx::Backend backend)                  \
	{                                                                                              \
		if (!haldls::vx::detail::BackendContainerTrait<Type>::valid(backend)) {                    \
			throw std::runtime_error("Backend not supported for container type.");                 \
		}                                                                                          \
                                                                                                   \
		size_t const backend_index = static_cast<size_t>(                                          \
		    haldls::vx::detail::BackendContainerTrait<Type>::backend_index_lookup_table.at(        \
		        static_cast<size_t>(backend)));                                                    \
		return read_table_generator<Type>(                                                         \
		    coord, backend_index,                                                                  \
		    std::make_index_sequence<                                                              \
		        hate::type_list_size<typename haldls::vx::detail::BackendContainerTrait<           \
		            Type>::container_list>::value>());                                             \
	}                                                                                              \
	PlaybackProgram::ContainerTicket<Type> PlaybackProgramBuilder::read(                           \
	    typename Type::coordinate_type const& coord)                                               \
	{                                                                                              \
		return read(coord, haldls::vx::detail::BackendContainerTrait<Type>::default_backend);      \
	}
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/container.def"

void PlaybackProgramBuilder::merge_back(PlaybackProgramBuilder& other)
{
	merge_back(std::forward<PlaybackProgramBuilder>(other));
}

void PlaybackProgramBuilder::merge_back(PlaybackProgramBuilder&& other)
{
	m_builder_impl->merge_back(*(other.m_builder_impl));
	m_unsupported_targets.merge(other.m_unsupported_targets);
}

void PlaybackProgramBuilder::merge_back(fisch::vx::PlaybackProgramBuilder&& other)
{
	m_builder_impl->merge_back(other);
}

void PlaybackProgramBuilder::merge_back(fisch::vx::PlaybackProgramBuilder& other)
{
	merge_back(std::forward<fisch::vx::PlaybackProgramBuilder>(other));
}

void PlaybackProgramBuilder::copy_back(PlaybackProgramBuilder const& other)
{
	m_builder_impl->copy_back(*(other.m_builder_impl));
	m_unsupported_targets.insert(
	    other.m_unsupported_targets.begin(), other.m_unsupported_targets.end());
}

void PlaybackProgramBuilder::copy_back(fisch::vx::PlaybackProgramBuilder const& other)
{
	m_builder_impl->copy_back(other);
}

PlaybackProgram PlaybackProgramBuilder::done()
{
	auto const program = PlaybackProgram(m_builder_impl->done(), m_unsupported_targets);
	m_unsupported_targets.clear();
	return program;
}

std::ostream& operator<<(std::ostream& os, PlaybackProgramBuilder const& builder)
{
	os << *(builder.m_builder_impl);
	return os;
}

bool PlaybackProgramBuilder::empty() const
{
	return m_builder_impl->empty();
}

size_t PlaybackProgramBuilder::size_to_fpga() const
{
	return m_builder_impl->size_to_fpga();
}

bool PlaybackProgramBuilder::is_write_only() const
{
	return m_builder_impl->is_write_only();
}

} // namespace stadls::vx
