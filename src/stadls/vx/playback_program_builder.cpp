#include "stadls/vx/playback_program_builder.h"

#include "fisch/vx/playback_program.h"
#include "haldls/vx/common.h"
#include "stadls/visitors.h"
#include "stadls/vx/playback_program.h"

namespace stadls::vx {

PlaybackProgramBuilder::PlaybackProgramBuilder() :
    m_builder_impl(std::make_unique<fisch::vx::PlaybackProgramBuilder>())
{}

void PlaybackProgramBuilder::wait_until(
    typename haldls::vx::Timer::coordinate_type const& coord, haldls::vx::Timer::Value const time)
{
	m_builder_impl->wait_until(coord, time);
}

template <typename T, size_t SupportedBackendIndex>
void PlaybackProgramBuilder::write_table_entry(
    PlaybackProgramBuilder& builder, typename T::coordinate_type const& coord, T const& config)
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

	auto& builder_impl_ref = *(builder.m_builder_impl);
	auto addr_it = write_addresses.cbegin();
	for (auto const& word : words) {
		builder_impl_ref.write(*addr_it, backend_container_type(word));
		++addr_it;
	}
}

template <typename T, size_t... SupportedBackendIndex>
void PlaybackProgramBuilder::write_table_generator(
    typename T::coordinate_type const& coord,
    T const& config,
    size_t const backend_index,
    std::index_sequence<SupportedBackendIndex...>)
{
	std::array<
	    void (*)(PlaybackProgramBuilder&, typename T::coordinate_type const&, T const&),
	    sizeof...(SupportedBackendIndex)>
	    write_table{write_table_entry<T, SupportedBackendIndex>...};

	write_table.at(backend_index)(*this, coord, config);
}

#define PLAYBACK_CONTAINER(Name, Type)                                                             \
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
		    coord, config, backend_index,                                                          \
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

		    typedef std::vector<typename backend_container_type::coordinate_type> addresses_type;
		    addresses_type read_addresses;
		    {
			    T config;

			    if constexpr (std::is_same<T, haldls::vx::PPUMemoryBlock>::value) {
				    // FIXME (Issue #3327): PPUMemoryBlock needs special size on construction
				    config = haldls::vx::PPUMemoryBlock(coord.toPPUMemoryBlockSize());
			    }

			    haldls::vx::visit_preorder(
			        config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
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
	m_builder_impl->merge_back(*(other.m_builder_impl));
}

void PlaybackProgramBuilder::merge_back(fisch::vx::PlaybackProgramBuilder& other)
{
	m_builder_impl->merge_back(other);
}

PlaybackProgram PlaybackProgramBuilder::done()
{
	return PlaybackProgram(m_builder_impl->done());
}

std::ostream& operator<<(std::ostream& os, PlaybackProgramBuilder const& builder)
{
	os << *(builder.m_builder_impl);
	return os;
}

} // namespace stadls::vx
