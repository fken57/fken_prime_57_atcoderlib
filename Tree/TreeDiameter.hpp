#pragma once

#include "../Graph/BFS.hpp"
#include "WeightedTreeBFS.hpp"

struct TreeDiameter {
    ll dir = 0;
    pair<ll, ll> edge = {-1, -1};
    vll path;
};

namespace fib_tree_diameter_detail {

inline int farthest_vertex(const vll& dist) {
    return static_cast<int>(max_element(dist.begin(), dist.end()) - dist.begin());
}

inline vll restore_path(int target, const vector<int>& parent) {
    vll path;
    for (int vertex = target; vertex != -1; vertex = parent[vertex]) {
        path.push_back(vertex);
    }
    reverse(path.begin(), path.end());
    return path;
}

inline pair<vll, vector<int>> weighted_distances(const WeightedGraph& graph,
                                                 int start) {
    const int n = static_cast<int>(graph.size());
    vll dist(n, inf);
    vector<int> parent(n, -1);
    queue<int> queue;
    dist[start] = 0;
    queue.push(start);
    while (!queue.empty()) {
        const int vertex = queue.front();
        queue.pop();
        for (const auto& edge : graph[vertex]) {
            if (edge.to == parent[vertex]) continue;
            if (dist[edge.to] != inf) continue;
            parent[edge.to] = vertex;
            dist[edge.to] = dist[vertex] + edge.weight;
            queue.push(edge.to);
        }
    }
    return {dist, parent};
}

}  // namespace fib_tree_diameter_detail

inline TreeDiameter ExecuteTreeDiameter(const Graph& graph) {
    if (graph.empty()) return {};
    const auto first = bfs(graph, 0);
    const int endpoint1 = fib_tree_diameter_detail::farthest_vertex(first.dist);
    const auto second = bfs(graph, endpoint1);
    const int endpoint2 = fib_tree_diameter_detail::farthest_vertex(second.dist);
    return {second.dist[endpoint2],
            {endpoint1, endpoint2},
            fib_tree_diameter_detail::restore_path(endpoint2, second.parent)};
}

// Requires a connected tree with non-negative edge weights.
inline TreeDiameter ExecuteTreeDiameter(const WeightedGraph& graph) {
    if (graph.empty()) return {};
    const auto [first_dist, first_parent] =
        fib_tree_diameter_detail::weighted_distances(graph, 0);
    (void)first_parent;
    const int endpoint1 =
        fib_tree_diameter_detail::farthest_vertex(first_dist);
    const auto [second_dist, second_parent] =
        fib_tree_diameter_detail::weighted_distances(graph, endpoint1);
    const int endpoint2 =
        fib_tree_diameter_detail::farthest_vertex(second_dist);
    return {second_dist[endpoint2],
            {endpoint1, endpoint2},
            fib_tree_diameter_detail::restore_path(endpoint2, second_parent)};
}
