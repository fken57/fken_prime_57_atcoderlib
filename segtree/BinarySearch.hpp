#pragma once

#include "Monoids.hpp"

// Returns the first index r >= left for which prefix_ok(prod(left, r + 1))
// becomes false. Returns segtree size if every suffix extension is valid.
// prefix_ok(Monoid::e()) must be true, as required by ACL max_right.
template <class SegTree, class Predicate>
int segtree_first_break(const SegTree& segtree,
                        int left,
                        Predicate prefix_ok) {
    return segtree.max_right(left, prefix_ok);
}

// Returns the minimum boundary l <= right such that suffix_ok(prod(l, right))
// is true. The first element that would break the predicate is l - 1.
// suffix_ok(Monoid::e()) must be true, as required by ACL min_left.
template <class SegTree, class Predicate>
int segtree_left_boundary(const SegTree& segtree,
                          int right,
                          Predicate suffix_ok) {
    return segtree.min_left(right, suffix_ok);
}

// For a RangeSumSegTree containing non-negative values, returns the minimum
// index i with sum[0, i + 1) >= target. Returns size() if it does not exist.
template <class T>
int segtree_prefix_lower_bound(const RangeSumSegTree<T>& segtree,
                               const T& target) {
    if (!(T{} < target)) return 0;
    return segtree.max_right(0, [&](const T& sum) { return sum < target; });
}

// Zero-indexed order statistic for a frequency segment tree.
template <class T>
int segtree_kth(const RangeSumSegTree<T>& segtree, const T& k) {
    assert(!(k < T{}) && "segtree_kth requires k >= 0");
    return segtree_prefix_lower_bound(segtree, k + T{1});
}
