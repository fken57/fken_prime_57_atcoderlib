#pragma once

#include <algorithm>
#include <cassert>
#include <limits>
#include <numeric>
#include <utility>
#include <atcoder/segtree>

template <class T>
struct RangeMinMonoid {
    using S = T;
    using value_type = S;
    static value_type op(const value_type& left, const value_type& right) {
        return std::min(left, right);
    }
    static value_type e() { return std::numeric_limits<T>::max(); }
};

template <class T>
struct RangeMaxMonoid {
    using S = T;
    using value_type = S;
    static value_type op(const value_type& left, const value_type& right) {
        return std::max(left, right);
    }
    static value_type e() { return std::numeric_limits<T>::lowest(); }
};

template <class T>
struct RangeSumMonoid {
    using S = T;
    using value_type = S;
    static value_type op(const value_type& left, const value_type& right) {
        return left + right;
    }
    static value_type e() { return value_type{}; }
};

template <class T>
struct RangeGcdMonoid {
    using S = T;
    using value_type = S;
    static value_type op(const value_type& left, const value_type& right) {
        return std::gcd(left, right);
    }
    static value_type e() { return value_type{}; }
};

// Scalar monoids. Integer lcm and arithmetic operations must fit in T.
template <class T>
struct RangeProductMonoid {
    using S = T;
    using value_type = S;
    static S op(S left, S right) { return left * right; }
    static S e() { return T{1}; }
};

template <class T>
struct RangeLcmMonoid {
    using S = T;
    using value_type = S;
    static S op(S left, S right) { return std::lcm(left, right); }
    static S e() { return T{1}; }
};

template <class T>
struct RangeXorMonoid {
    using S = T;
    using value_type = S;
    static S op(S left, S right) { return left ^ right; }
    static S e() { return T{}; }
};

template <class T>
struct RangeAndMonoid {
    using S = T;
    using value_type = S;
    static S op(S left, S right) { return left & right; }
    static S e() { return static_cast<T>(~T{}); }
};

template <class T>
struct RangeOrMonoid {
    using S = T;
    using value_type = S;
    static S op(S left, S right) { return left | right; }
    static S e() { return T{}; }
};

// {minimum, maximum}; a leaf for x is {x, x}.
template <class T>
struct RangeMinMaxMonoid {
    using S = std::pair<T, T>;
    using value_type = S;
    static S op(S left, S right) {
        return {std::min(left.first, right.first),
                std::max(left.second, right.second)};
    }
    static S e() {
        return {std::numeric_limits<T>::max(), std::numeric_limits<T>::lowest()};
    }
    static S from_value(T value) { return {value, value}; }
};

// {minimum, occurrences}; a leaf for x is {x, 1}.
template <class T>
struct RangeMinCountMonoid {
    using S = std::pair<T, int>;
    using value_type = S;
    static S op(S left, S right) {
        if (left.first < right.first) return left;
        if (right.first < left.first) return right;
        return {left.first, left.second + right.second};
    }
    static S e() { return {std::numeric_limits<T>::max(), 0}; }
    static S from_value(T value) { return {value, 1}; }
};

template <class T>
struct RangeMaxCountMonoid {
    using S = std::pair<T, int>;
    using value_type = S;
    static S op(S left, S right) {
        if (left.first > right.first) return left;
        if (right.first > left.first) return right;
        return {left.first, left.second + right.second};
    }
    static S e() { return {std::numeric_limits<T>::lowest(), 0}; }
    static S from_value(T value) { return {value, 1}; }
};

// {a, b} represents a*x+b. op(left, right) applies left, then right.
// Intended for modint or another type with associative + and *.
template <class T>
struct AffineCompositionMonoid {
    using S = std::pair<T, T>;
    using value_type = S;
    static S op(S left, S right) {
        return {right.first * left.first,
                right.first * left.second + right.second};
    }
    static S e() { return {T{1}, T{}}; }
    static T apply(S function, T value) {
        return function.first * value + function.second;
    }
};

// Numeric representation of a bracket interval. It knows nothing about the
// character representation '(' / ')'.
struct BracketBalance {
    int sum = 0;
    int min_prefix = 0;

    friend bool operator==(const BracketBalance& left,
                           const BracketBalance& right) {
        return left.sum == right.sum && left.min_prefix == right.min_prefix;
    }
};

struct BracketBalanceMonoid {
    using S = BracketBalance;
    using value_type = S;

    static value_type op(const value_type& left, const value_type& right) {
        return {left.sum + right.sum,
                std::min(left.min_prefix, left.sum + right.min_prefix)};
    }

    static value_type e() { return {0, 0}; }

    // +1: opening element, -1: closing element, 0: neutral element.
    static value_type from_number(int value) {
        assert(-1 <= value && value <= 1);
        return {value, std::min(0, value)};
    }

    static bool is_valid(const value_type& value) {
        return value.sum == 0 && value.min_prefix >= 0;
    }

    static bool is_prefix_valid(const value_type& value) {
        return value.min_prefix >= 0;
    }
};

// Compatibility aliases; new code can use atcoder::segtree<M::S, M::op, M::e>.
template <class Monoid>
using MonoidSegTree = atcoder::segtree<typename Monoid::value_type,
                                       Monoid::op,
                                       Monoid::e>;

template <class T>
using RangeMinSegTree = MonoidSegTree<RangeMinMonoid<T>>;

template <class T>
using RangeMaxSegTree = MonoidSegTree<RangeMaxMonoid<T>>;

template <class T>
using RangeSumSegTree = MonoidSegTree<RangeSumMonoid<T>>;

template <class T>
using RangeGcdSegTree = MonoidSegTree<RangeGcdMonoid<T>>;

using BracketBalanceSegTree = MonoidSegTree<BracketBalanceMonoid>;

// Compatibility aliases. New code should prefer the explicit Balance names.
using BracketValue = BracketBalance;
using BracketMonoid = BracketBalanceMonoid;
using BracketSegTree = BracketBalanceSegTree;

