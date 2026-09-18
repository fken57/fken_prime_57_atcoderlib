#pragma once

#include "../template.hpp"

class TreeDoubling {
public:
    Graph G;
    vector<vector<int>> parent;
    int DS = 1;
    int start = 0;
    vector<int> depth;
    ll N = 0;

    explicit TreeDoubling(const Graph& graph, int root = 0)
        : G(graph), start(root), depth(graph.size(), -1), N(graph.size()) {
        if (N == 0) return;
        assert(0 <= root && root < N);
        while ((1ULL << DS) <= static_cast<unsigned long long>(N)) ++DS;
        parent.assign(N, vector<int>(DS, -1));
        build();
    }

    int kth_ancestor(int vertex, long long steps) const {
        if (steps < 0) return -1;
        for (int bit = 0; bit < DS && vertex != -1; ++bit) {
            if ((steps >> bit) & 1LL) vertex = parent[vertex][bit];
        }
        if ((steps >> DS) != 0) return -1;
        return vertex;
    }

    int lca(int left, int right) const {
        assert(0 <= left && left < N && 0 <= right && right < N);
        if (depth[left] < depth[right]) swap(left, right);
        left = kth_ancestor(left, depth[left] - depth[right]);
        if (left == right) return left;
        for (int bit = DS - 1; bit >= 0; --bit) {
            if (parent[left][bit] != parent[right][bit]) {
                left = parent[left][bit];
                right = parent[right][bit];
            }
        }
        return parent[left][0];
    }

    int distance(int left, int right) const {
        const int ancestor = lca(left, right);
        return depth[left] + depth[right] - 2 * depth[ancestor];
    }

    int jump(int from, int to, long long steps) const {
        const int ancestor = lca(from, to);
        const long long up = depth[from] - depth[ancestor];
        const long long down = depth[to] - depth[ancestor];
        if (steps < 0 || steps > up + down) return -1;
        if (steps <= up) return kth_ancestor(from, steps);
        return kth_ancestor(to, up + down - steps);
    }

    int LCA(int left, int right) const { return lca(left, right); }
    int JumpOnTree(int from, int to, int steps) const {
        return jump(from, to, steps);
    }

private:
    void build() {
        queue<int> queue;
        queue.push(start);
        depth[start] = 0;
        while (!queue.empty()) {
            const int vertex = queue.front();
            queue.pop();
            for (const ll next_value : G[vertex]) {
                const int next = static_cast<int>(next_value);
                if (next == parent[vertex][0]) continue;
                if (depth[next] != -1) continue;
                parent[next][0] = vertex;
                depth[next] = depth[vertex] + 1;
                queue.push(next);
            }
        }
        assert(find(depth.begin(), depth.end(), -1) == depth.end() &&
               "TreeDoubling input must be connected");

        for (int bit = 1; bit < DS; ++bit) {
            for (int vertex = 0; vertex < N; ++vertex) {
                const int middle = parent[vertex][bit - 1];
                if (middle != -1) parent[vertex][bit] = parent[middle][bit - 1];
            }
        }
    }
};
