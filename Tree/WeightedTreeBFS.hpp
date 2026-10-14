#pragma once

#include "../common.hpp"

template <class Cost>
void WeightedTreeBFS(int start,
                     const WeightedGraphT<Cost>& graph,
                     vector<Cost>& dist,
                     Cost infinity = numeric_limits<Cost>::max() / Cost(4)) {
    const int n = static_cast<int>(graph.size());
    assert(0 <= start && start < n);
    dist.assign(n, infinity);
    queue<int> queue;
    dist[start] = 0;
    queue.push(start);

    while (!queue.empty()) {
        const int vertex = queue.front();
        queue.pop();
        for (const auto& edge : graph[vertex]) {
            if (dist[edge.to] != infinity) continue;
            dist[edge.to] = dist[vertex] + edge.weight;
            queue.push(edge.to);
        }
    }
}
