#pragma once

#include <stdexcept>

#include "haldls/common/visibility.h"

namespace haldls {
namespace exception {

class InvalidConfiguration : public std::runtime_error
{
public:
	InvalidConfiguration(std::string const& what) HALDLS_VISIBLE;
};

} // namespace exception
} // namespace haldls
