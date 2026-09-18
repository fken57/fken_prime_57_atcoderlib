#pragma once

#include "../template.hpp"

struct GraphCycle {
    vector<int> vertices;
    vector<int> edges;

    explicit operator bool() const { return !edges.empty(); }
};

namespace fib_cycle_detail {

inline GraphCycle restore_cycle(int from,
                                int ancestor,
                                int closing_edge,
                                const vector<int>& parent,
                                const vector<int>& parent_edge) {
    vector<int> vertices{from};
    vector<int> edges;
    for (int vertex = from; vertex != ancestor; vertex = parent[vertex]) {
        assert(vertex != -1);
        edges.push_back(parent_edge[vertex]);
        vertices.push_back(parent[vertex]);
    }
    reverse(vertices.begin(), vertices.end());
    reverse(edges.begin(), edges.end());
    edges.push_back(closing_edge);
    return {vertices, edges};
}

}  // namespace fib_cycle_detail

// The i-th input pair is directed from edges[i].first to edges[i].second.
inline GraphCycle find_directed_cycle(
    int n,
    const vector<pair<int, int>>& edges) {
    vector<vector<int>> graph(n);
    for (int id = 0; id < static_cast<int>(edges.size()); ++id) {
        const auto [from, to] = edges[id];
        assert(0 <= from && from < n && 0 <= to && to < n);
        graph[from].push_back(id);
    }

    vector<int> color(n, 0), parent(n, -1), parent_edge(n, -1);
    struct Frame {
        int vertex;
        int next_edge;
    };

    for (int start = 0; start < n; ++start) {
        if (color[start] != 0) continue;
        vector<Frame> stack{{start, 0}};
        color[start] = 1;
        while (!stack.empty()) {
            Frame& frame = stack.back();
            const int vertex = frame.vertex;
            if (frame.next_edge == static_cast<int>(graph[vertex].size())) {
                color[vertex] = 2;
                stack.pop_back();
                continue;
            }

            const int edge_id = graph[vertex][frame.next_edge++];
            const int to = edges[edge_id].second;
            if (color[to] == 0) {
                color[to] = 1;
                parent[to] = vertex;
                parent_edge[to] = edge_id;
                stack.push_back({to, 0});
            } else if (color[to] == 1) {
                return fib_cycle_detail::restore_cycle(
                    vertex, to, edge_id, parent, parent_edge);
            }
        }
    }
    return {};
}

// Supports self loops and parallel edges. The returned vertices and edges have
// the same length; edges[i] connects vertices[i] and vertices[(i + 1) % L].
inline GraphCycle find_undirected_cycle(
    int n,
    const vector<pair<int, int>>& edges) {
    vector<vector<int>> graph(n);
    for (int id = 0; id < static_cast<int>(edges.size()); ++id) {
        const auto [u, v] = edges[id];
        assert(0 <= u && u < n && 0 <= v && v < n);
        graph[u].push_back(id);
        graph[v].push_back(id);
    }

    vector<int> color(n, 0), parent(n, -1), parent_edge(n, -1);
    struct Frame {
        int vertex;
        int next_edge;
    };

    for (int start = 0; start < n; ++start) {
        if (color[start] != 0) continue;
        vector<Frame> stack{{start, 0}};
        color[start] = 1;
        while (!stack.empty()) {
            Frame& frame = stack.back();
            const int vertex = frame.vertex;
            if (frame.next_edge == static_cast<int>(graph[vertex].size())) {
                color[vertex] = 2;
                stack.pop_back();
                continue;
            }

            const int edge_id = graph[vertex][frame.next_edge++];
            if (edge_id == parent_edge[vertex]) continue;
            const auto [u, v] = edges[edge_id];
            const int to = u ^ v ^ vertex;
            if (color[to] == 0) {
                color[to] = 1;
                parent[to] = vertex;
                parent_edge[to] = edge_id;
                stack.push_back({to, 0});
            } else if (color[to] == 1) {
                return fib_cycle_detail::restore_cycle(
                    vertex, to, edge_id, parent, parent_edge);
            }
        }
    }
    return {};
}
