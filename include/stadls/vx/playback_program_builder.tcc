
// included from "stadls/vx/playback_program_builder.h"

#include <iostream>
#include <optional>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_set.hpp>
#include <log4cxx/logger.h>

#include "fisch/vx/playback_program_builder.h"
#include "haldls/cerealization.h"
#include "haldls/vx/coordinate_to_container.h"
#include "haldls/vx/traits.h"
#include "hate/type_traits.h"
#include "stadls/visitors.h"

namespace stadls::vx::detail {

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
class PlaybackProgramBuilderAdapterImpl
{
public:
	template <typename T, size_t SupportedBackendIndex>
	static void write_table_entry(
	    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>& builder,
	    typename T::coordinate_type const& coord,
	    T const& config,
	    std::optional<T> const& config_reference);

	template <class T, size_t... SupportedBackendIndex>
	static void write_table_generator(
	    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>& builder,
	    typename T::coordinate_type const& coord,
	    T const& config,
	    size_t backend_index,
	    std::optional<T> const& config_reference,
	    std::index_sequence<SupportedBackendIndex...>);

	template <class T, size_t... SupportedBackendIndex>
	static PlaybackProgram::ContainerTicket<T> read_table_generator(
	    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>& builder,
	    typename T::coordinate_type const& coord,
	    size_t backend_index,
	    std::index_sequence<SupportedBackendIndex...>);
};

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::
    PlaybackProgramBuilderAdapter() :
    m_builder_impl(std::make_unique<BuilderStorage>()), m_unsupported_targets()
{}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::
    PlaybackProgramBuilderAdapter(
        PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>&& other) :
    m_builder_impl(std::move(other.m_builder_impl)),
    m_unsupported_targets(std::move(other.m_unsupported_targets))
{
	other.m_builder_impl = std::move(std::make_unique<BuilderStorage>());
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>&
PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::operator=(
    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>&& other)
{
	m_builder_impl = std::move(other.m_builder_impl);
	m_unsupported_targets = std::move(other.m_unsupported_targets);
	other.m_builder_impl = std::move(std::make_unique<BuilderStorage>());
	return *this;
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::
    ~PlaybackProgramBuilderAdapter()
{}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::wait_until(
    typename haldls::vx::Timer::coordinate_type const& coord, haldls::vx::Timer::Value const time)
{
	static bool first = true;
	if (first) {
		auto logger = log4cxx::Logger::getLogger("stadls.playback_program_builder");
		LOG4CXX_WARN(
		    logger, "wait_until(halco.TimerOnDLS(), value) is deprecated, use "
		            "block_until(halco.TimerOnDLS(), value) instead.");
		first = false;
	}
	block_until(coord, time);
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::block_until(
    typename haldls::vx::Timer::coordinate_type const& coord, haldls::vx::Timer::Value const time)
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		m_builder_impl->write(
		    halco::hicann_dls::vx::WaitUntilOnFPGA(coord.toEnum()),
		    fisch::vx::WaitUntil(fisch::vx::WaitUntil::Value(time)));
	} else {
		m_builder_impl->block_until(coord, time);
	}
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::block_until(
    typename halco::hicann_dls::vx::BarrierOnFPGA const& coord, haldls::vx::Barrier const barrier)
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		m_builder_impl->write(coord, barrier.encode());
	} else {
		m_builder_impl->block_until(coord, barrier);
	}
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::block_until(
    typename halco::hicann_dls::vx::PollingOmnibusBlockOnFPGA const& coord,
    haldls::vx::PollingOmnibusBlock const barrier)
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		m_builder_impl->write(coord, barrier.encode());
	} else {
		m_builder_impl->block_until(coord, barrier);
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

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
template <typename T, size_t SupportedBackendIndex>
void PlaybackProgramBuilderAdapterImpl<BuilderStorage, DoneType, CoordinateToContainer>::
    write_table_entry(
        PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>& builder,
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

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
template <class T, size_t... SupportedBackendIndex>
void PlaybackProgramBuilderAdapterImpl<BuilderStorage, DoneType, CoordinateToContainer>::
    write_table_generator(
        PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>& builder,
        typename T::coordinate_type const& coord,
        T const& config,
        size_t const backend_index,
        std::optional<T> const& config_reference,
        std::index_sequence<SupportedBackendIndex...>)
{
	std::array<
	    void (*)(
	        PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>&,
	        typename T::coordinate_type const&, T const&, std::optional<T> const&),
	    sizeof...(SupportedBackendIndex)>
	    write_table{write_table_entry<T, SupportedBackendIndex>...};
	write_table.at(backend_index)(builder, coord, config, config_reference);
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
template <typename Type>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::write(
    [[maybe_unused]] typename Type::coordinate_type const& coord,
    [[maybe_unused]] Type const& config,
    [[maybe_unused]] Type const& config_reference,
    [[maybe_unused]] haldls::vx::Backend backend)
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		if (!haldls::vx::detail::BackendContainerTrait<Type>::valid(backend)) {
			throw std::runtime_error("Backend not supported for container type.");
		}
		size_t const backend_index = static_cast<size_t>(
			haldls::vx::detail::BackendContainerTrait<Type>::backend_index_lookup_table.at(
				static_cast<size_t>(backend)));
		Impl::template write_table_generator<Type>(
			*this, coord, config, backend_index, config_reference,
			std::make_index_sequence<
				hate::type_list_size<typename haldls::vx::detail::BackendContainerTrait<
					Type>::container_list>::value>());
	} else {
		throw std::runtime_error("Unsupported differential write.");
	}
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
template <typename Type>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::write(
    [[maybe_unused]] typename Type::coordinate_type const& coord,
    [[maybe_unused]] Type const& config,
    [[maybe_unused]] Type const& config_reference)
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		this->write(
			coord, config, config_reference,
			haldls::vx::detail::BackendContainerTrait<Type>::default_backend);
	} else {
		throw std::runtime_error("Unsupported differential write.");
	}
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
template <typename Type>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::write(
    [[maybe_unused]] typename Type::coordinate_type const& coord,
    [[maybe_unused]] Type const& config,
    [[maybe_unused]] haldls::vx::Backend backend)
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		if (!haldls::vx::detail::BackendContainerTrait<Type>::valid(backend)) {
			throw std::runtime_error("Backend not supported for container type.");
		}
		size_t const backend_index = static_cast<size_t>(
			haldls::vx::detail::BackendContainerTrait<Type>::backend_index_lookup_table.at(
				static_cast<size_t>(backend)));
		Impl::template write_table_generator<Type>(
			*this, coord, config, backend_index, std::nullopt,
			std::make_index_sequence<
				hate::type_list_size<typename haldls::vx::detail::BackendContainerTrait<
					Type>::container_list>::value>());
	} else {
		m_builder_impl->write(coord, config);
	}
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
template <typename Type>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::write(
    typename Type::coordinate_type const& coord, Type const& config)
{
	write(coord, config, haldls::vx::detail::BackendContainerTrait<Type>::default_backend);
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
template <class T, size_t... SupportedBackendIndex>
PlaybackProgram::ContainerTicket<T>
PlaybackProgramBuilderAdapterImpl<BuilderStorage, DoneType, CoordinateToContainer>::
    read_table_generator(
        PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>& builder,
        typename T::coordinate_type const& coord,
        size_t const backend_index,
        std::index_sequence<SupportedBackendIndex...>)
{
	using namespace haldls::vx::detail;
	std::array<
	    PlaybackProgram::ContainerTicket<T> (*)(
	        PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>&,
	        typename T::coordinate_type const&),
	    sizeof...(SupportedBackendIndex)>
	    read_table{[](PlaybackProgramBuilderAdapter<
	                      BuilderStorage, DoneType, CoordinateToContainer>& builder,
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
		    return PlaybackProgram::ContainerTicket<T>(coord, std::move(ticket_impl));
	    }...};

	return read_table.at(backend_index)(builder, coord);
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
template <typename CoordinateType>
PlaybackProgram::ContainerTicket<typename CoordinateToContainer<CoordinateType>::type>
PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::read(
    [[maybe_unused]] CoordinateType const& coord, [[maybe_unused]] haldls::vx::Backend backend)
{
	using Type = typename CoordinateToContainer<CoordinateType>::type;
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		if (!haldls::vx::detail::BackendContainerTrait<Type>::valid(backend)) {
			throw std::runtime_error("Backend not supported for container type.");
		}

		size_t const backend_index = static_cast<size_t>(
			haldls::vx::detail::BackendContainerTrait<Type>::backend_index_lookup_table.at(
				static_cast<size_t>(backend)));
		return Impl::template read_table_generator<Type>(
			*this, coord, backend_index,
			std::make_index_sequence<
				hate::type_list_size<typename haldls::vx::detail::BackendContainerTrait<
					Type>::container_list>::value>());
	} else {
		return m_builder_impl->template read<Type>(coord);
	}
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
template <typename CoordinateType>
PlaybackProgram::ContainerTicket<typename CoordinateToContainer<CoordinateType>::type>
PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::read(
    CoordinateType const& coord)
{
	using Type = typename CoordinateToContainer<CoordinateType>::type;
	return read<CoordinateType>(coord, haldls::vx::detail::BackendContainerTrait<Type>::default_backend);
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::merge_back(
    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>& other)
{
	merge_back(
	    std::forward<
	        PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>>(other));
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::merge_back(
    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>&& other)
{
	m_builder_impl->merge_back(*(other.m_builder_impl));
	m_unsupported_targets.merge(other.m_unsupported_targets);
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::merge_back(
    BuilderStorage&& other)
{
	m_builder_impl->merge_back(other);
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::merge_back(
    BuilderStorage& other)
{
	merge_back(std::forward<BuilderStorage>(other));
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::copy_back(
    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer> const& other)
{
	m_builder_impl->copy_back(*(other.m_builder_impl));
	m_unsupported_targets.insert(
	    other.m_unsupported_targets.begin(), other.m_unsupported_targets.end());
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::copy_back(
    BuilderStorage const& other)
{
	m_builder_impl->copy_back(other);
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
DoneType PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::done()
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		auto const program = PlaybackProgram(m_builder_impl->done(), m_unsupported_targets);
		m_unsupported_targets.clear();
		return program;
	} else {
		return m_builder_impl->done();
	}
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
std::ostream& operator<<(
    std::ostream& os,
    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer> const& builder)
{
	os << *(builder.m_builder_impl);
	return os;
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
bool PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::empty() const
{
	return m_builder_impl->empty();
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
size_t
PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::size_to_fpga() const
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		return m_builder_impl->size_to_fpga();
	} else {
		throw std::runtime_error("BuilderStorage does not support size_to_fpga().");
	}
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
bool PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::is_write_only()
    const
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		return m_builder_impl->is_write_only();
	} else {
		/* we don't support reads in non-fisch-PPB backends */
		return true;
	}
}

template <
    typename BuilderStorage,
    typename DoneType,
    template <typename>
    class CoordinateToContainer>
template <typename Archive>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType, CoordinateToContainer>::serialize(
    [[maybe_unused]] Archive& ar, std::uint32_t const)
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		throw std::runtime_error(
		    "fisch-based PlaybackProgramBuilderAdapter doesn't support serialization");
	} else {
		ar(CEREAL_NVP(m_builder_impl));
		ar(CEREAL_NVP(m_unsupported_targets));
	}
}

} // namespace stadls::vx::detail
