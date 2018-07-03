#pragma once

#include <stdexcept>

#include "hate/visibility.h"

namespace haldls {
namespace exception {

/// \brief haldls-specific Exception type describing an invalid hardware
/// configuration setting.
class InvalidConfiguration : public std::runtime_error
{
public:
	InvalidConfiguration(std::string const& what) SYMBOL_VISIBLE;
};

} // namespace exception
} // namespace haldls
