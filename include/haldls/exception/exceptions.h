#pragma once

#include <stdexcept>

#include "hate/visibility.h"

namespace haldls {
namespace exception {

class InvalidConfiguration : public std::runtime_error
{
public:
	InvalidConfiguration(std::string const& what) SYMBOL_VISIBLE;
};

} // namespace exception
} // namespace haldls
