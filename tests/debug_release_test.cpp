#include "../debug.hpp"

#include <cassert>

struct ReleasePoint {
    int x;
};

FIB_DEBUG_FIELDS(ReleasePoint, value.x)

int main() {
    static_assert(FIB_DEBUG_ENABLED == 0, "release debug must be disabled");

    int side_effect = 0;
    FIB_DEBUG(++side_effect);
    FIB_DEBUG(FIB_VLA(++side_effect));
    FIB_LOOP_GUARD(release_loop, 1, 1);
    FIB_LOOP_TICK(release_loop, ++side_effect);

    assert(side_effect == 0);
}
