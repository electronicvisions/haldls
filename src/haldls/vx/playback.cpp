#include "fisch/vx/playback_program.h"

#include "haldls/vx/playback.h"

#include "haldls/vx/common.h"
#include "stadls/visitors.h"

namespace haldls {
namespace vx {

PlaybackProgram::PlaybackProgram() : m_program_impl() {}

PlaybackProgram::PlaybackProgram(std::shared_ptr<fisch::vx::PlaybackProgram> const& program_impl) :
    m_program_impl(program_impl)
{}

template <typename T>
T PlaybackProgram::ContainerTicket<T>::get() const
{
	if (!m_ticket_impl.valid())
		throw std::runtime_error(
		    "container data not available yet (out of bounds of available results data)");

	if constexpr (detail::BackendTrait<T>::valid(Backend::OmnibusOnChipOverJTAG)) {
		auto data_jtag = m_ticket_impl.get();
		typedef std::vector<omnibus_word_type> words_type;
		words_type data(data_jtag.size());
		std::transform(
		    std::make_move_iterator(data_jtag.begin()), std::make_move_iterator(data_jtag.end()),
		    data.begin(), [](fisch::vx::OmnibusOnChipOverJTAG&& word) -> omnibus_word_type {
			    return word.get();
		    });


		T config;
		visit_preorder(config, m_coord, stadls::DecodeVisitor<words_type>{std::move(data)});
		return config;
	} else {
		return m_ticket_impl.get();
	}
}

template <typename T>
bool PlaybackProgram::ContainerTicket<T>::valid() const
{
	return m_ticket_impl.valid();
}

template <typename T>
typename T::coordinate_type PlaybackProgram::ContainerTicket<T>::get_coordinate() const
{
	return m_coord;
}

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	template SYMBOL_VISIBLE Type PlaybackProgram::ContainerTicket<Type>::get() const;              \
	template SYMBOL_VISIBLE bool PlaybackProgram::ContainerTicket<Type>::valid() const;            \
	template SYMBOL_VISIBLE typename Type::coordinate_type                                         \
	PlaybackProgram::ContainerTicket<Type>::get_coordinate() const;
#include "haldls/vx/container.def"

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	template SYMBOL_VISIBLE Type PlaybackProgram::ContainerTicket<Type>::get() const;              \
	template SYMBOL_VISIBLE bool PlaybackProgram::ContainerTicket<Type>::valid() const;            \
	template SYMBOL_VISIBLE typename Type::coordinate_type                                         \
	PlaybackProgram::ContainerTicket<Type>::get_coordinate() const;
#include "fisch/vx/container.def"

std::ostream& operator<<(std::ostream& os, PlaybackProgram const& program)
{
	os << *(program.m_program_impl);
	return os;
}

std::shared_ptr<fisch::vx::PlaybackProgram> PlaybackProgram::impl() const
{
	return m_program_impl;
}

void PlaybackProgramBuilder::wait_until(
    typename fisch::vx::Timer::coordinate_type const& coord, fisch::vx::Timer::Value const time)
{
	m_builder_impl->wait_until(coord, time);
}

template <class T>
void PlaybackProgramBuilder::write(
    typename T::coordinate_type const& coord, T const& config, std::optional<Backend> backend)
{
	if (!backend) {
		backend = detail::BackendTrait<T>::default_backend;
	} else if (!detail::BackendTrait<T>::valid(*backend)) {
		throw std::runtime_error("Backend not supported for container type.");
	}
	if (*backend == Backend::OmnibusOnChipOverJTAG) {
		if constexpr (detail::BackendTrait<T>::valid(Backend::OmnibusOnChipOverJTAG)) {
			typedef std::vector<omnibus_address_type> addresses_type;
			addresses_type write_addresses;
			visit_preorder(
			    config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});

			typedef std::vector<omnibus_word_type> words_type;
			words_type words;
			visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{words});

			if (words.size() != write_addresses.size()) {
				throw std::logic_error("number of addresses and words do not match");
			}

			auto& builder_impl_ref = *m_builder_impl;
			auto addr_it = write_addresses.cbegin();
			for (auto const& word : words) {
				builder_impl_ref.write(*addr_it, fisch::vx::OmnibusOnChipOverJTAG(word));
				++addr_it;
			}
		}
	} else if (*backend == Backend::Direct) {
		if constexpr (detail::BackendTrait<T>::valid(Backend::Direct)) {
			m_builder_impl->write(coord, config);
		}
	} else {
		throw std::runtime_error("Write to backend not implemented.");
	}
}

template <class T>
PlaybackProgram::ContainerTicket<T> PlaybackProgramBuilder::read(
    typename T::coordinate_type const& coord, std::optional<Backend> backend)
{
	if (!backend) {
		backend = detail::BackendTrait<T>::default_backend;
	} else if (!detail::BackendTrait<T>::valid(*backend)) {
		throw std::runtime_error("Backend not supported for container type.");
	}
	if constexpr (detail::BackendTrait<T>::valid(Backend::Direct)) {
		return PlaybackProgram::ContainerTicket<T>(coord, m_builder_impl->read<T>(coord));
	} else {
		if (*backend == Backend::OmnibusOnChipOverJTAG) {
			typedef std::vector<omnibus_address_type> addresses_type;
			addresses_type read_addresses;
			{
				T config;
				visit_preorder(
				    config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
			}
			auto ticket_impl =
			    m_builder_impl->read<fisch::vx::OmnibusOnChipOverJTAG>(read_addresses);
			return PlaybackProgram::ContainerTicket<T>(coord, ticket_impl);
		}
	}
	throw std::runtime_error("Read to backend not implemented.");
}

PlaybackProgramBuilder::PlaybackProgramBuilder() :
    m_builder_impl(std::make_unique<fisch::vx::PlaybackProgramBuilder>())
{}

void PlaybackProgramBuilder::halt()
{
	m_builder_impl->halt();
}

PlaybackProgram PlaybackProgramBuilder::done()
{
	return PlaybackProgram(m_builder_impl->done());
}

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	template SYMBOL_VISIBLE void PlaybackProgramBuilder::write<Type>(                              \
	    Type::coordinate_type const& coord, Type const& config,                                    \
	    std::optional<Backend> backend = std::nullopt);
#include "haldls/vx/container.def"

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	template SYMBOL_VISIBLE void PlaybackProgramBuilder::write<Type>(                              \
	    Type::coordinate_type const& coord, Type const& config,                                    \
	    std::optional<Backend> backend = std::nullopt);
#include "fisch/vx/container.def"

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	template SYMBOL_VISIBLE PlaybackProgram::ContainerTicket<Type>                                 \
	PlaybackProgramBuilder::read<Type>(                                                            \
	    Type::coordinate_type const& coord, std::optional<Backend> backend = std::nullopt);
#include "haldls/vx/container.def"

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	template SYMBOL_VISIBLE PlaybackProgram::ContainerTicket<Type>                                 \
	PlaybackProgramBuilder::read<Type>(                                                            \
	    Type::coordinate_type const& coord, std::optional<Backend> backend = std::nullopt);
#include "fisch/vx/container.def"

} // namespace vx
} // namespace haldls
