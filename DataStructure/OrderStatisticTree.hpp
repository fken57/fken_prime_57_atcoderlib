#pragma once

#include "../template.hpp"

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

// GNU PBDS-based optional containers. Include this header only when indexed
// set/map operations are needed.
template <class Key, class Compare = std::less<Key>>
using OrderedSet = __gnu_pbds::tree<
    Key,
    __gnu_pbds::null_type,
    Compare,
    __gnu_pbds::rb_tree_tag,
    __gnu_pbds::tree_order_statistics_node_update>;

template <class Key, class Mapped, class Compare = std::less<Key>>
using OrderedMap = __gnu_pbds::tree<
    Key,
    Mapped,
    Compare,
    __gnu_pbds::rb_tree_tag,
    __gnu_pbds::tree_order_statistics_node_update>;
