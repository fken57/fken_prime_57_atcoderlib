#pragma once

#include "Monoids.hpp"
#include <string>
#include <vector>

// Character-level wrapper. Conversion between '(' / ')' and +1 / -1 is
// intentionally kept out of BracketBalanceMonoid.
class BracketSequence {
public:
    explicit BracketSequence(std::string brackets)
        : brackets_(std::move(brackets)), tree_(encode_all(brackets_)) {}

    int size() const { return static_cast<int>(brackets_.size()); }
    char get(int index) const {
        check_index(index);
        return brackets_[index];
    }
    const std::string& str() const { return brackets_; }

    void set(int index, char bracket) {
        check_index(index);
        const BracketBalance value = encode(bracket);
        brackets_[index] = bracket;
        tree_.set(index, value);
    }

    void toggle(int index) {
        check_index(index);
        set(index, brackets_[index] == '(' ? ')' : '(');
    }

    BracketBalance prod(int left, int right) const {
        assert(0 <= left && left <= right && right <= size());
        return tree_.prod(left, right);
    }

    bool is_valid(int left, int right) const {
        return BracketBalanceMonoid::is_valid(prod(left, right));
    }
    bool is_valid() const { return is_valid(0, size()); }

    bool is_prefix_valid(int left, int right) const {
        return BracketBalanceMonoid::is_prefix_valid(prod(left, right));
    }

    // Returns the first index whose inclusion makes the balance negative.
    // Returns size() when the whole suffix keeps a non-negative balance.
    int first_invalid(int left = 0) const {
        assert(0 <= left && left <= size());
        return tree_.max_right(left, [](BracketBalance value) {
            return BracketBalanceMonoid::is_prefix_valid(value);
        });
    }

    const BracketBalanceSegTree& segtree() const { return tree_; }

private:
    static BracketBalance encode(char bracket) {
        assert((bracket == '(' || bracket == ')') &&
               "BracketSequence accepts only '(' and ')'");
        return BracketBalanceMonoid::from_number(bracket == '(' ? 1 : -1);
    }

    static std::vector<BracketBalance> encode_all(const std::string& brackets) {
        std::vector<BracketBalance> values;
        values.reserve(brackets.size());
        for (const char bracket : brackets) values.push_back(encode(bracket));
        return values;
    }

    void check_index(int index) const {
        assert(0 <= index && index < size());
    }

    std::string brackets_;
    BracketBalanceSegTree tree_;
};
