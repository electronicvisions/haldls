
// included from "stadls/vx/playback_program_builder.h"

#include <iostream>
#include <optional>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_set.hpp>
#include <log4cxx/logger.h>

#include "fisch/vx/container_cast.h"
#include "fisch/vx/playback_program_builder.h"
#include "fisch/vx/traits.h"
#include "haldls/vx/coordinate_to_container.h"
#include "haldls/vx/traits.h"
#include "hate/type_index.h"
#include "hate/type_traits.h"
#include "stadls/visitors.h"
#include "stadls/vx/addresses.h"
#include "stadls/vx/encode.h"
#include "stadls/vx/supports_empty.h"
#include <cereal/macros.hpp>

namespace stadls::vx::detail {

template <typename BuilderStorage, typename DoneType>
class PlaybackProgramBuilderAdapterImpl
{
public:
	template <typename T>
	static void write_visitor(
	    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>& builder,
	    T const& visited_encoded_config);

	template <typename T>
	static void write_visitor(
	    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>& builder,
	    T const& visited_encoded_config,
	    haldls::vx::Container::BackendCocoListVariant const& encoded_config_reference);

	static void write(
	    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>& builder,
	    haldls::vx::Container::Coordinate const& coord,
	    haldls::vx::Container const& config,
	    std::optional<haldls::vx::Backend> const& backend);

	static void write(
	    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>& builder,
	    haldls::vx::Container::Coordinate const& coord,
	    haldls::vx::Container const& config,
	    haldls::vx::Container const& config_reference,
	    std::optional<haldls::vx::Backend> const& backend);

	static ContainerTicket read(
	    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>& builder,
	    haldls::vx::Container::Coordinate const& coord,
	    std::optional<haldls::vx::Backend> const& backend);
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
{
	other.m_builder_impl = std::move(std::make_unique<BuilderStorage>());
}

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
template <typename T>
void PlaybackProgramBuilderAdapterImpl<BuilderStorage, DoneType>::write_visitor(
    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>& builder,
    T const& visited_encoded_config)
{
	auto const& [fisch_coords, fisch_configs] = visited_encoded_config;

	if (fisch_configs.size() == 0) {
		throw std::runtime_error("Container not writeable.");
	}

	typedef decltype(fisch::vx::container_cast(
	    std::declval<typename decltype(fisch_configs)::value_type>())) fisch_container_type;

	if constexpr (fisch::vx::IsWritable<fisch_container_type>::value) {
		std::vector<fisch_container_type> fisch_containers;
		fisch_containers.reserve(fisch_configs.size());
		std::transform(
		    fisch_configs.begin(), fisch_configs.end(), std::back_inserter(fisch_containers),
		    (typename decltype(fisch_containers)::value_type (*)(
		        typename decltype(fisch_configs)::value_type const&)) &
		        fisch::vx::container_cast);
		assert(builder.m_builder_impl);
		builder.m_builder_impl->write(fisch_coords, fisch_containers);
	} else {
		throw std::runtime_error(
		    "Container backend " + hate::name<fisch_container_type>() + " not writable.");
	}
}

template <typename BuilderStorage, typename DoneType>
template <typename T>
void PlaybackProgramBuilderAdapterImpl<BuilderStorage, DoneType>::write_visitor(
    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>& builder,
    T const& visited_encoded_config,
    haldls::vx::Container::BackendCocoListVariant const& encoded_config_reference)
{
	auto const& [fisch_coords, fisch_configs] = visited_encoded_config;
	auto const& [fisch_coords_reference, fisch_configs_reference] =
	    std::get<std::decay_t<decltype(visited_encoded_config)>>(encoded_config_reference);

	if (fisch_coords.size() != fisch_configs.size()) {
		throw std::logic_error("Number of addresses and words do not match for 'config'.");
	}

	if (fisch_coords_reference.size() != fisch_configs_reference.size()) {
		throw std::logic_error(
		    "Number of addresses and words do not match for 'config_reference'.");
	}

	if (fisch_configs_reference.size() != fisch_configs.size()) {
		throw std::logic_error("Number of words of 'config' and 'config_reference' do not match.");
	}

	if (fisch_configs.size() == 0) {
		throw std::runtime_error("Container not writeable.");
	}

	typedef typename decltype(fisch_configs)::value_type fisch_config_type;
	typedef typename decltype(fisch_coords)::value_type fisch_coord_type;

	std::vector<fisch_config_type> fisch_configs_reduced;
	std::vector<fisch_coord_type> fisch_coords_reduced;
	for (size_t i = 0; i < fisch_configs_reference.size(); ++i) {
		if (fisch_configs_reference[i] != fisch_configs[i]) {
			fisch_configs_reduced.push_back(fisch_configs[i]);
			fisch_coords_reduced.push_back(fisch_coords[i]);
		}
	}

	typedef decltype(fisch::vx::container_cast(
	    std::declval<typename decltype(fisch_configs)::value_type>())) fisch_container_type;

	if constexpr (fisch::vx::IsWritable<fisch_container_type>::value) {
		std::vector<fisch_container_type> fisch_containers_reduced;
		fisch_containers_reduced.reserve(fisch_configs_reduced.size());
		std::transform(
		    fisch_configs_reduced.begin(), fisch_configs_reduced.end(),
		    std::back_inserter(fisch_containers_reduced),
		    (typename decltype(fisch_containers_reduced)::value_type (*)(
		        typename decltype(fisch_configs)::value_type const&)) &
		        fisch::vx::container_cast);
		assert(builder.m_builder_impl);
		builder.m_builder_impl->write(fisch_coords_reduced, fisch_containers_reduced);
	} else {
		throw std::runtime_error(
		    "Container backend " + hate::name<fisch_container_type>() + " not writable.");
	}
}

template <typename BuilderStorage, typename DoneType>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::block_until(
    typename haldls::vx::BlockUntil::Coordinate const& coord,
    haldls::vx::BlockUntil const& condition)
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		auto const encoded_condition = condition.encode_write(coord, std::nullopt);

		auto const visitor = [this](auto const& visited_encoded_condition) {
			Impl::write_visitor(*this, visited_encoded_condition);
		};
		std::visit(visitor, encoded_condition);

		m_unsupported_targets.insert(condition.get_unsupported_write_targets());
	} else {
		assert(m_builder_impl);
		m_builder_impl->block_until(coord, condition);
	}
}

template <typename BuilderStorage, typename DoneType>
void PlaybackProgramBuilderAdapterImpl<BuilderStorage, DoneType>::write(
    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>& builder,
    [[maybe_unused]] haldls::vx::Container::Coordinate const& coord,
    [[maybe_unused]] haldls::vx::Container const& config,
    [[maybe_unused]] std::optional<haldls::vx::Backend> const& backend)
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		auto const encoded_config = config.encode_write(coord, backend);

		auto const visitor = [&builder](auto const& visited_encoded_config) {
			write_visitor(builder, visited_encoded_config);
		};
		std::visit(visitor, encoded_config);

		builder.m_unsupported_targets.insert(config.get_unsupported_write_targets());
	} else {
		assert(builder.m_builder_impl);
		builder.m_builder_impl->write(coord, config);
	}
}

template <typename BuilderStorage, typename DoneType>
void PlaybackProgramBuilderAdapterImpl<BuilderStorage, DoneType>::write(
    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>& builder,
    [[maybe_unused]] haldls::vx::Container::Coordinate const& coord,
    [[maybe_unused]] haldls::vx::Container const& config,
    [[maybe_unused]] haldls::vx::Container const& config_reference,
    [[maybe_unused]] std::optional<haldls::vx::Backend> const& backend)
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		if (typeid(config) != typeid(config_reference)) {
			throw std::runtime_error(
			    "Container types of 'config' and 'config_reference' don't match.");
		}

		if (!config.get_supports_differential_write() ||
		    !config_reference.get_supports_differential_write()) {
			throw std::logic_error("Container type does not support differential write.");
		}

		auto const encoded_config = config.encode_write(coord, backend);
		auto const encoded_config_reference = config_reference.encode_write(coord, backend);

		auto const visitor = [&builder,
		                      encoded_config_reference](auto const& visited_encoded_config) {
			write_visitor(builder, visited_encoded_config, encoded_config_reference);
		};
		std::visit(visitor, encoded_config);

		builder.m_unsupported_targets.insert(config.get_unsupported_write_targets());
	} else {
		assert(builder.m_builder_impl);
		builder.m_builder_impl->write(coord, config);
	}
}

template <typename BuilderStorage, typename DoneType>
ContainerTicket PlaybackProgramBuilderAdapterImpl<BuilderStorage, DoneType>::read(
    PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>& builder,
    [[maybe_unused]] haldls::vx::Container::Coordinate const& coord,
    [[maybe_unused]] std::optional<haldls::vx::Backend> const& backend)
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		auto container = haldls::vx::construct_container(coord);
		assert(container);

		auto const addresses = container->encode_read(coord, backend);

		auto const visitor =
		    [&builder](auto const& visited_addresses) -> ContainerTicket::ticket_impl_type {
			if (visited_addresses.size() == 0) {
				throw std::runtime_error("Container not readable.");
			}

			assert(builder.m_builder_impl);
			return builder.m_builder_impl->read(visited_addresses);
		};
		auto ticket_impl = std::visit(visitor, addresses);

		builder.m_unsupported_targets.insert(container->get_unsupported_read_targets());

		return ContainerTicket(coord.clone(), std::move(container), std::move(ticket_impl));
	} else {
		assert(builder.m_builder_impl);
		return builder.m_builder_impl->read(coord);
	}
}

template <typename BuilderStorage, typename DoneType>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::write(
    [[maybe_unused]] haldls::vx::Container::Coordinate const& coord,
    [[maybe_unused]] haldls::vx::Container const& config)
{
	Impl::write(*this, coord, config, std::nullopt);
}

template <typename BuilderStorage, typename DoneType>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::write(
    [[maybe_unused]] haldls::vx::Container::Coordinate const& coord,
    [[maybe_unused]] haldls::vx::Container const& config,
    [[maybe_unused]] haldls::vx::Backend const backend)
{
	Impl::write(*this, coord, config, backend);
}

template <typename BuilderStorage, typename DoneType>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::write(
    [[maybe_unused]] haldls::vx::Container::Coordinate const& coord,
    [[maybe_unused]] haldls::vx::Container const& config,
    [[maybe_unused]] haldls::vx::Container const& config_reference)
{
	Impl::write(*this, coord, config, config_reference, std::nullopt);
}

template <typename BuilderStorage, typename DoneType>
void PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::write(
    [[maybe_unused]] haldls::vx::Container::Coordinate const& coord,
    [[maybe_unused]] haldls::vx::Container const& config,
    [[maybe_unused]] haldls::vx::Container const& config_reference,
    [[maybe_unused]] haldls::vx::Backend const backend)
{
	Impl::write(*this, coord, config, config_reference, backend);
}

template <typename BuilderStorage, typename DoneType>
ContainerTicket PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::read(
    [[maybe_unused]] haldls::vx::Container::Coordinate const& coord)
{
	return Impl::read(*this, coord, std::nullopt);
}

template <typename BuilderStorage, typename DoneType>
ContainerTicket PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::read(
    [[maybe_unused]] haldls::vx::Container::Coordinate const& coord,
    [[maybe_unused]] haldls::vx::Backend const backend)
{
	return Impl::read(*this, coord, backend);
}

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
	} else {
		return m_builder_impl->done();
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
	} else {
		throw std::runtime_error("BuilderStorage does not support size_to_fpga().");
	}
}

template <typename BuilderStorage, typename DoneType>
size_t PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::size_from_fpga() const
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		return m_builder_impl->size_from_fpga();
	} else {
		throw std::runtime_error("BuilderStorage does not support size_from_fpga().");
	}
}

template <typename BuilderStorage, typename DoneType>
bool PlaybackProgramBuilderAdapter<BuilderStorage, DoneType>::is_write_only() const
{
	if constexpr (std::is_same_v<BuilderStorage, fisch::vx::PlaybackProgramBuilder>) {
		return m_builder_impl->is_write_only();
	} else {
		/* we don't support reads in non-fisch-PPB backends */
		return true;
	}
}

template<typename Res, typename In>
auto convert_to_builder(In& cocos) -> Res
{
	Res builder;
	for (auto const& coco : cocos.values) {
		auto const& [coord, config] = coco;
		if (!coord || !config) {
			throw std::logic_error("Unexpected access to moved-from object.");
		}
		if (auto const config_ptr = dynamic_cast<haldls::vx::Container const*>(config.get())) {
			builder.write(*coord, *config_ptr);

		} else if (
		    auto const config_ptr = dynamic_cast<haldls::vx::BlockUntil const*>(config.get())) {
			builder.block_until(*coord, *config_ptr);
		} else {
			throw std::logic_error("Encodable type not supported.");
		}
	}
	return builder;
}

} // namespace stadls::vx::detail
