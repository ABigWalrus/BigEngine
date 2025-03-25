#pragma once

#include <stdint.h>

using UUID = uint32_t;
namespace util {
UUID generate_uuid();
} // namespace util