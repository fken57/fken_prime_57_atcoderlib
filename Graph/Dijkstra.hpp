#pragma once

#include "../template.hpp"

template <class Cost>
struct DijkstraResult {
    Cost infinity{};
    vector<Cost> dist;
    vector<int> parent_vertex;
    vector<int> parent_edge;

    bool reachable(int vertex) const { return dist[vertex] != infinity; }

    vector<int> restore_vertices(int target) const {
        if (!reachable(target)) return {};
        vector<int> path;
        for (int vertex = target; vertex != -1;
             vertex = parent_vertex[vertex]) {
            path.push_back(vertex);
        }
        reverse(path.begin(), path.end());
        return path;
    }

    vector<int> restore_edges(int target) const {
        if (!reachable(target)) return {};
        vector<int> path;
        while (parent_vertex[target] != -1) {
            path.push_back(parent_edge[target]);
            target = parent_vertex[target];
        }
        reverse(path.begin(), path.end());
        return path;
    }
};

template <class Cost>
DijkstraResult<Cost> dijkstra(
    const WeightedGraphT<Cost>& graph,
    const vector<int>& sources,
    Cost infinity = std::numeric_limits<Cost>::max() / Cost(4)) {
    const int n = static_cast<int>(graph.size());
    DijkstraResult<Cost> result{
        infinity,
        vector<Cost>(n, infinity),
        vector<int>(n, -1),
        vector<int>(n, -1),
    };

    using QueueEntry = pair<Cost, int>;
    priority_queue<QueueEntry, vector<QueueEntry>, greater<QueueEntry>> queue;
    for (const int source : sources) {
        assert(0 <= source && source < n);
        if (result.dist[source] == Cost{}) continue;
        result.dist[source] = Cost{};
        queue.emplace(Cost{}, source);
    }

    while (!queue.empty()) {
        const auto [distance, vertex] = queue.top();
        queue.pop();
        if (result.dist[vertex] != distance) continue;

        for (int index = 0; index < static_cast<int>(graph[vertex].size());
             ++index) {
            const auto& edge = graph[vertex][index];
            assert(!(edge.weight < Cost{}) &&
                   "dijkstra requires non-negative edge costs");
            if (edge.weight > infinity ||
                result.dist[vertex] > infinity - edge.weight) {
                continue;
            }
            const Cost next_distance = result.dist[vertex] + edge.weight;
            if (next_distance >= result.dist[edge.to]) continue;
            result.dist[edge.to] = next_distance;
            result.parent_vertex[edge.to] = vertex;
            result.parent_edge[edge.to] = edge.id == -1 ? index : edge.id;
            queue.emplace(next_distance, edge.to);
        }
    }
    return result;
}

template <class Cost>
DijkstraResult<Cost> dijkstra(
    const WeightedGraphT<Cost>& graph,
    int source,
    Cost infinity = std::numeric_limits<Cost>::max() / Cost(4)) {
    return dijkstra(graph, vector<int>{source}, infinity);
}
