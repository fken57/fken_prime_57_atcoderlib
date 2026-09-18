#pragma once

#include "../template.hpp"

template <class T>
struct RangeMinMonoid {
    using value_type = T;
    static value_type op(const value_type& left, const value_type& right) {
        return std::min(left, right);
    }
    static value_type e() { return std::numeric_limits<T>::max(); }
};

template <class T>
struct RangeMaxMonoid {
    using value_type = T;
    static value_type op(const value_type& left, const value_type& right) {
        return std::max(left, right);
    }
    static value_type e() { return std::numeric_limits<T>::lowest(); }
};

template <class T>
struct RangeSumMonoid {
    using value_type = T;
    static value_type op(const value_type& left, const value_type& right) {
        return left + right;
    }
    static value_type e() { return value_type{}; }
};

template <class T>
struct RangeGcdMonoid {
    using value_type = T;
    static value_type op(const value_type& left, const value_type& right) {
        return std::gcd(left, right);
    }
    static value_type e() { return value_type{}; }
};

struct BracketValue {
    int sum = 0;
    int min_prefix = 0;

    friend bool operator==(const BracketValue& left,
                           const BracketValue& right) {
        return left.sum == right.sum && left.min_prefix == right.min_prefix;
    }
};

struct BracketMonoid {
    using value_type = BracketValue;

    static value_type op(const value_type& left, const value_type& right) {
        return {left.sum + right.sum,
                std::min(left.min_prefix, left.sum + right.min_prefix)};
    }

    static value_type e() { return {0, 0}; }

    static value_type from_char(char bracket) {
        assert((bracket == '(' || bracket == ')') &&
               "BracketMonoid accepts only '(' and ')'");
        return bracket == '(' ? value_type{1, 0} : value_type{-1, -1};
    }

    static bool is_valid(const value_type& value) {
        return value.sum == 0 && value.min_prefix >= 0;
    }

    static bool is_prefix_valid(const value_type& value) {
        return value.min_prefix >= 0;
    }
};

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

using BracketSegTree = MonoidSegTree<BracketMonoid>;
