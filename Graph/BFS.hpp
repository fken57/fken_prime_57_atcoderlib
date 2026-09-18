#pragma once

#include "../template.hpp"

struct BFSResult {
    vll dist;
    vector<int> parent;

    vector<int> restore_path(int target) const {
        if (target < 0 || target >= static_cast<int>(dist.size()) ||
            dist[target] == inf) {
            return {};
        }
        vector<int> path;
        for (int vertex = target; vertex != -1; vertex = parent[vertex]) {
            path.push_back(vertex);
        }
        reverse(path.begin(), path.end());
        return path;
    }
};

inline BFSResult bfs(const Graph& graph, const vector<int>& sources) {
    const int n = static_cast<int>(graph.size());
    BFSResult result{vll(n, inf), vector<int>(n, -1)};
    queue<int> queue;
    for (const int source : sources) {
        assert(0 <= source && source < n);
        if (result.dist[source] == 0) continue;
        result.dist[source] = 0;
        queue.push(source);
    }

    while (!queue.empty()) {
        const int vertex = queue.front();
        queue.pop();
        for (const ll next_value : graph[vertex]) {
            const int next = static_cast<int>(next_value);
            if (result.dist[next] != inf) continue;
            result.dist[next] = result.dist[vertex] + 1;
            result.parent[next] = vertex;
            queue.push(next);
        }
    }
    return result;
}

inline BFSResult bfs(const Graph& graph, int source) {
    return bfs(graph, vector<int>{source});
}

template <class Start>
void BFS(Start start, const Graph& graph, vll& dist) {
    vector<int> sources;
    if constexpr (is_integral_v<decay_t<Start>>) {
        sources.push_back(static_cast<int>(start));
    } else {
        for (const auto vertex : start) {
            sources.push_back(static_cast<int>(vertex));
        }
    }
    dist = bfs(graph, sources).dist;
}
