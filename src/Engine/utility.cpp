#include "utility.hpp"

namespace util {
static uint32_t n_uuids = 0;

UUID generate_uuid() {
    n_uuids++;
    return n_uuids;
}
} // namespace util