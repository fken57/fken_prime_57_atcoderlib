#pragma once

#include "../template.hpp"

class DisjointUnionSet {
public:
    int dsuSize;
    int TotalConective;

    explicit DisjointUnionSet(ll n)
        : dsuSize(checked_size(n)),
          TotalConective(dsuSize),
          dsu_(dsuSize) {}

    void Merge(ll left, ll right) {
        check_index(left);
        check_index(right);
        if (!dsu_.same(left, right)) {
            dsu_.merge(left, right);
            --TotalConective;
        }
    }

    bool Same(ll left, ll right) {
        check_index(left);
        check_index(right);
        return dsu_.same(left, right);
    }

    int leader(ll vertex) {
        check_index(vertex);
        return dsu_.leader(vertex);
    }

    int size(ll vertex) {
        check_index(vertex);
        return dsu_.size(vertex);
    }

    vector<int> ReturnTree(ll vertex) {
        check_index(vertex);
        const int root = dsu_.leader(vertex);
        for (auto& group : dsu_.groups()) {
            if (!group.empty() && dsu_.leader(group.front()) == root) {
                return group;
            }
        }
        return {};
    }

private:
    static int checked_size(ll size) {
        assert(0 <= size && size <= numeric_limits<int>::max());
        return static_cast<int>(size);
    }

    void check_index(ll vertex) const {
        assert(0 <= vertex && vertex < dsuSize && "Invalid DSU index");
    }

    atcoder::dsu dsu_;
};
