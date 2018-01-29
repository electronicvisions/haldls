#include "haldls/exception/exceptions.h"

namespace haldls {
namespace exception {

InvalidConfiguration::InvalidConfiguration(std::string const& what)
	: runtime_error(("InvalidConfiguration: " + what).c_str())
{}

} // namespace exception
} // namespace haldls
