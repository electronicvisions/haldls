#include "haldls/vx/print.tcc"

#include <array>
#include <boost/type_index.hpp>

#include "fisch/vx/container.h"
#include "haldls/vx/traits.h"
#include "haldls/vx/v1/container.h"
#include "stadls/visitors.h"

namespace haldls::vx {

namespace {

template <typename T, size_t... SupportedBackendIndex>
void print_table_generator(
    std::ostream& os,
    T const& config,
    size_t const backend_index,
    std::index_sequence<SupportedBackendIndex...>)
{
	std::array<void (*)(std::ostream&, T const&), sizeof...(SupportedBackendIndex)> print_table{
	    [](std::ostream& os, T const& config) {
		    typedef typename hate::index_type_list_by_integer<
		        SupportedBackendIndex,
		        typename detail::BackendContainerTrait<T>::container_list>::type
		        backend_container_type;

		    typedef std::vector<backend_container_type> words_type;
		    words_type words;
		    visit_preorder(
		        config, typename T::coordinate_type(), stadls::EncodeVisitor<words_type>{words});

		    for (auto const& word : words) {
			    os << "\t\t" << word << std::endl;
		    }
	    }...};

	print_table.at(backend_index)(os, config);
}

static const std::array<std::string, hate::type_list_size<BackendContainerList>::value>
    backend_name{
#define LAST_PLAYBACK_CONTAINER(Name, Type) #Type
#define PLAYBACK_CONTAINER(Name, Type) LAST_PLAYBACK_CONTAINER(Name, Type),
#include "fisch/vx/container.def"
    };

} // namespace

template <class T>
std::ostream& print_words_for_each_backend(std::ostream& os, T const& config)
{
	os << boost::typeindex::type_id<T>().pretty_name() << " encoded words:" << std::endl;

	for (auto const backend : detail::BackendContainerTrait<T>::valid_backends) {
		size_t const backend_index =
		    static_cast<size_t>(detail::BackendContainerTrait<T>::backend_index_lookup_table.at(
		        static_cast<size_t>(backend)));
		os << "\tBackend container " << backend_name.at(static_cast<size_t>(backend)) << ":"
		   << std::endl;
		print_table_generator<T>(
		    os, config, backend_index,
		    std::make_index_sequence<hate::type_list_size<
		        typename detail::BackendContainerTrait<T>::container_list>::value>());
	}
	return os;
}

#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	template std::ostream& print_words_for_each_backend<Type>(                                     \
	    std::ostream & os, Type const& config);
#include "haldls/vx/v1/container.def"

} // namespace haldls::vx
