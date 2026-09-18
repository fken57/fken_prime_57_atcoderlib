#pragma once

#include "../template.hpp"

template <class EdgeData>
struct RerootingEdge {
    int from;
    int to;
    EdgeData from_to;
    EdgeData to_from;

    RerootingEdge(int source, int destination, const EdgeData& data)
        : from(source),
          to(destination),
          from_to(data),
          to_from(data) {}

    RerootingEdge(int source,
                  int destination,
                  const EdgeData& forward_data,
                  const EdgeData& backward_data)
        : from(source),
          to(destination),
          from_to(forward_data),
          to_from(backward_data) {}
};

// merge combines contributions at a vertex.
// add_edge(neighbor_dp, edge_data) extends the neighboring state through the
// edge directed from the current vertex to that neighbor.
// add_vertex(merged_contributions, vertex) finalizes the DP for that vertex.
template <class DP,
          class EdgeData,
          class Merge,
          class AddEdge,
          class AddVertex>
vector<DP> rerooting_dp(int n,
                        const vector<RerootingEdge<EdgeData>>& edges,
                        const DP& identity,
                        Merge merge,
                        AddEdge add_edge,
                        AddVertex add_vertex,
                        int root = 0) {
    if (n == 0) return {};
    assert(0 <= root && root < n);
    assert(static_cast<int>(edges.size()) == n - 1 &&
           "rerooting_dp requires a tree");

    struct Arc {
        int to;
        int reverse_index;
        EdgeData data;
    };
    vector<vector<Arc>> graph(n);
    for (const auto& edge : edges) {
        assert(0 <= edge.from && edge.from < n);
        assert(0 <= edge.to && edge.to < n);
        const int from_index = static_cast<int>(graph[edge.from].size());
        const int to_index = static_cast<int>(graph[edge.to].size());
        graph[edge.from].push_back({edge.to, to_index, edge.from_to});
        graph[edge.to].push_back({edge.from, from_index, edge.to_from});
    }

    vector<int> parent(n, -2), order;
    order.reserve(n);
    parent[root] = -1;
    vector<int> stack{root};
    while (!stack.empty()) {
        const int vertex = stack.back();
        stack.pop_back();
        order.push_back(vertex);
        for (int i = 0; i < static_cast<int>(graph[vertex].size()); ++i) {
            const int next = graph[vertex][i].to;
            if (next == parent[vertex]) continue;
            assert(parent[next] == -2 && "rerooting_dp input must be a tree");
            parent[next] = vertex;
            stack.push_back(next);
        }
    }
    assert(static_cast<int>(order.size()) == n &&
           "rerooting_dp input must be connected");

    vector<DP> down(n, identity);
    for (auto iterator = order.rbegin(); iterator != order.rend(); ++iterator) {
        const int vertex = *iterator;
        DP merged = identity;
        for (const auto& arc : graph[vertex]) {
            if (arc.to == parent[vertex]) continue;
            merged = merge(merged, add_edge(down[arc.to], arc.data));
        }
        down[vertex] = add_vertex(merged, vertex);
    }

    vector<DP> from_parent(n, identity), answer(n, identity);
    for (const int vertex : order) {
        const int degree = static_cast<int>(graph[vertex].size());
        vector<DP> contribution(degree, identity);
        for (int i = 0; i < degree; ++i) {
            const auto& arc = graph[vertex][i];
            if (arc.to == parent[vertex]) {
                contribution[i] = from_parent[vertex];
            } else {
                contribution[i] = add_edge(down[arc.to], arc.data);
            }
        }

        vector<DP> prefix(degree + 1, identity);
        vector<DP> suffix(degree + 1, identity);
        for (int i = 0; i < degree; ++i) {
            prefix[i + 1] = merge(prefix[i], contribution[i]);
        }
        for (int i = degree - 1; i >= 0; --i) {
            suffix[i] = merge(contribution[i], suffix[i + 1]);
        }
        answer[vertex] = add_vertex(prefix[degree], vertex);

        for (int i = 0; i < degree; ++i) {
            const auto& arc = graph[vertex][i];
            if (arc.to == parent[vertex]) continue;
            const DP without_child =
                add_vertex(merge(prefix[i], suffix[i + 1]), vertex);
            const auto& reverse_arc = graph[arc.to][arc.reverse_index];
            from_parent[arc.to] = add_edge(without_child, reverse_arc.data);
        }
    }
    return answer;
}
