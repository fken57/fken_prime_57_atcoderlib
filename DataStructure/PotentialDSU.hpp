#pragma once

#include "../template.hpp"

// Additive potentialized DSU.
// merge(a, b, difference) adds potential(b) - potential(a) = difference.
template <class T>
class PotentialDSU {
public:
    explicit PotentialDSU(int n)
        : parent_or_size_(n, -1), difference_to_parent_(n, T{}) {}

    int leader(int vertex) {
        if (parent_or_size_[vertex] < 0) return vertex;
        const int parent = parent_or_size_[vertex];
        const int root = leader(parent);
        difference_to_parent_[vertex] += difference_to_parent_[parent];
        return parent_or_size_[vertex] = root;
    }

    T potential(int vertex) {
        leader(vertex);
        return difference_to_parent_[vertex];
    }

    bool same(int left, int right) {
        return leader(left) == leader(right);
    }

    int size(int vertex) { return -parent_or_size_[leader(vertex)]; }

    // Returns false only when the new constraint contradicts known data.
    bool merge(int left, int right, T difference) {
        difference += potential(left);
        difference -= potential(right);
        int left_root = leader(left);
        int right_root = leader(right);
        if (left_root == right_root) return difference == T{};

        if (-parent_or_size_[left_root] < -parent_or_size_[right_root]) {
            swap(left_root, right_root);
            difference = -difference;
        }
        parent_or_size_[left_root] += parent_or_size_[right_root];
        parent_or_size_[right_root] = left_root;
        difference_to_parent_[right_root] = difference;
        return true;
    }

    // Requires same(left, right). Returns potential(right)-potential(left).
    T diff(int left, int right) {
        assert(same(left, right));
        return potential(right) - potential(left);
    }

    optional<T> diff_if_same(int left, int right) {
        if (!same(left, right)) return nullopt;
        return diff(left, right);
    }

private:
    vector<int> parent_or_size_;
    vector<T> difference_to_parent_;
};
