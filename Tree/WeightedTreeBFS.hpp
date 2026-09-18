#pragma once

#include "../template.hpp"

inline void WeightedTreeBFS(int start,
                            const WeightedGraph& graph,
                            vll& dist) {
    const int n = static_cast<int>(graph.size());
    assert(0 <= start && start < n);
    dist.assign(n, inf);
    queue<int> queue;
    dist[start] = 0;
    queue.push(start);

    while (!queue.empty()) {
        const int vertex = queue.front();
        queue.pop();
        for (const auto& edge : graph[vertex]) {
            if (dist[edge.to] != inf) continue;
            dist[edge.to] = dist[vertex] + edge.weight;
            queue.push(edge.to);
        }
    }
}
