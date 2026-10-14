#pragma once

#include "../Graph/BFS.hpp"
#include "WeightedTreeBFS.hpp"

template <class Cost>
struct TreeDiameterT {
    Cost dir{};
    pair<int, int> edge = {-1, -1};
    vector<int> path;
};

using TreeDiameter = TreeDiameterT<ll>;

namespace fib_tree_diameter_detail {

template <class Cost>
int farthest_vertex(const vector<Cost>& dist) {
    return static_cast<int>(max_element(dist.begin(), dist.end()) - dist.begin());
}

inline vector<int> restore_path(int target, const vector<int>& parent) {
    vector<int> path;
    for (int vertex = target; vertex != -1; vertex = parent[vertex]) {
        path.push_back(vertex);
    }
    reverse(path.begin(), path.end());
    return path;
}

template <class Cost>
pair<vector<Cost>, vector<int>> weighted_distances(
    const WeightedGraphT<Cost>& graph, int start) {
    const int n = static_cast<int>(graph.size());
    const Cost infinity = numeric_limits<Cost>::max() / Cost(4);
    vector<Cost> dist(n, infinity);
    vector<int> parent(n, -1);
    queue<int> queue;
    dist[start] = 0;
    queue.push(start);
    while (!queue.empty()) {
        const int vertex = queue.front();
        queue.pop();
        for (const auto& edge : graph[vertex]) {
            if (edge.to == parent[vertex]) continue;
            if (dist[edge.to] != infinity) continue;
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
template <class Cost>
TreeDiameterT<Cost> ExecuteTreeDiameter(const WeightedGraphT<Cost>& graph) {
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
