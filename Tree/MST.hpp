#pragma once

#include "../template.hpp"

template <class Cost>
struct MSTEdge {
    int from;
    int to;
    Cost cost;
    int id = -1;
};

template <class Cost>
struct KruskalResult {
    Cost cost{};
    vector<int> edge_ids;
    int components = 0;

    bool connected() const { return components <= 1; }
};

template <class Cost>
KruskalResult<Cost> kruskal(int n, vector<MSTEdge<Cost>> edges) {
    for (int index = 0; index < static_cast<int>(edges.size()); ++index) {
        if (edges[index].id == -1) edges[index].id = index;
    }
    sort(edges.begin(), edges.end(), [](const auto& left, const auto& right) {
        if (left.cost != right.cost) return left.cost < right.cost;
        return left.id < right.id;
    });

    atcoder::dsu dsu(n);
    KruskalResult<Cost> result;
    result.components = n;
    for (const auto& edge : edges) {
        assert(0 <= edge.from && edge.from < n);
        assert(0 <= edge.to && edge.to < n);
        if (dsu.same(edge.from, edge.to)) continue;
        dsu.merge(edge.from, edge.to);
        result.cost += edge.cost;
        result.edge_ids.push_back(edge.id);
        --result.components;
    }
    return result;
}

struct MSTData {
    ll cost;
    WeightedGraph graph;
    vll index;
    bool connected = false;
};

inline MSTData MinimumSpinningTree(ll n,
                                   const vll& from,
                                   const vll& to,
                                   const vll& weight) {
    assert(from.size() == to.size() && to.size() == weight.size());
    vector<MSTEdge<ll>> edges;
    edges.reserve(from.size());
    for (int id = 0; id < static_cast<int>(from.size()); ++id) {
        edges.push_back({static_cast<int>(from[id]), static_cast<int>(to[id]),
                         weight[id], id});
    }

    const auto result = kruskal(static_cast<int>(n), edges);
    WeightedGraph graph(n);
    vll selected;
    for (const int id : result.edge_ids) {
        graph[from[id]].push_back(
            {static_cast<int>(to[id]), weight[id], id});
        graph[to[id]].push_back(
            {static_cast<int>(from[id]), weight[id], id});
        selected.push_back(id);
    }
    return {result.cost, graph, selected, result.connected()};
}
