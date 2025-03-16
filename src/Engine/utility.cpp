#include "utility.hpp"

namespace util {
static int n_uuids = 0;

int generate_uuid() {
    n_uuids++;
    return n_uuids;
}
} // namespace util