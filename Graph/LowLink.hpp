#pragma once

#include "../template.hpp"

class LowLink {
public:
    LowLink(int n, const vector<pair<int, int>>& edges)
        : n_(n),
          edges_(edges),
          graph_(n),
          order_(n, -1),
          low_(n, -1),
          parent_(n, -1),
          parent_edge_(n, -1),
          articulation_(n, false),
          bridge_(edges.size(), false) {
        for (int id = 0; id < static_cast<int>(edges_.size()); ++id) {
            const auto [u, v] = edges_[id];
            assert(0 <= u && u < n_ && 0 <= v && v < n_);
            graph_[u].push_back({v, id});
            graph_[v].push_back({u, id});
        }
        build();
    }

    const vector<int>& order() const { return order_; }
    const vector<int>& low() const { return low_; }
    const vector<bool>& articulation_flags() const { return articulation_; }
    const vector<bool>& bridge_flags() const { return bridge_; }
    const vector<pair<int, int>>& edges() const { return edges_; }
    const vector<vector<pair<int, int>>>& graph() const { return graph_; }

    vector<int> articulation_points() const {
        vector<int> result;
        for (int vertex = 0; vertex < n_; ++vertex) {
            if (articulation_[vertex]) result.push_back(vertex);
        }
        return result;
    }

    vector<int> bridge_ids() const {
        vector<int> result;
        for (int id = 0; id < static_cast<int>(bridge_.size()); ++id) {
            if (bridge_[id]) result.push_back(id);
        }
        return result;
    }

private:
    void build() {
        struct Frame {
            int vertex;
            int next_edge;
            int child_count;
        };

        int timer = 0;
        for (int start = 0; start < n_; ++start) {
            if (order_[start] != -1) continue;
            order_[start] = low_[start] = timer++;
            vector<Frame> stack{{start, 0, 0}};

            while (!stack.empty()) {
                Frame& frame = stack.back();
                const int vertex = frame.vertex;
                if (frame.next_edge < static_cast<int>(graph_[vertex].size())) {
                    const auto [to, edge_id] =
                        graph_[vertex][frame.next_edge++];
                    if (edge_id == parent_edge_[vertex]) continue;
                    if (order_[to] == -1) {
                        ++frame.child_count;
                        parent_[to] = vertex;
                        parent_edge_[to] = edge_id;
                        order_[to] = low_[to] = timer++;
                        stack.push_back({to, 0, 0});
                    } else {
                        low_[vertex] = min(low_[vertex], order_[to]);
                    }
                    continue;
                }

                const int child_count = frame.child_count;
                stack.pop_back();
                const int parent = parent_[vertex];
                if (parent == -1) {
                    articulation_[vertex] = child_count >= 2;
                    continue;
                }

                low_[parent] = min(low_[parent], low_[vertex]);
                if (order_[parent] < low_[vertex]) {
                    bridge_[parent_edge_[vertex]] = true;
                }
                if (parent_[parent] != -1 &&
                    order_[parent] <= low_[vertex]) {
                    articulation_[parent] = true;
                }
            }
        }
    }

    int n_;
    vector<pair<int, int>> edges_;
    vector<vector<pair<int, int>>> graph_;
    vector<int> order_;
    vector<int> low_;
    vector<int> parent_;
    vector<int> parent_edge_;
    vector<bool> articulation_;
    vector<bool> bridge_;
};

class TwoEdgeConnectedComponents {
public:
    explicit TwoEdgeConnectedComponents(const LowLink& lowlink)
        : component_(lowlink.order().size(), -1) {
        const auto& graph = lowlink.graph();
        const auto& is_bridge = lowlink.bridge_flags();
        const int n = static_cast<int>(graph.size());

        for (int start = 0; start < n; ++start) {
            if (component_[start] != -1) continue;
            const int component_id = static_cast<int>(groups_.size());
            groups_.push_back({});
            vector<int> stack{start};
            component_[start] = component_id;
            while (!stack.empty()) {
                const int vertex = stack.back();
                stack.pop_back();
                groups_.back().push_back(vertex);
                for (const auto& [to, edge_id] : graph[vertex]) {
                    if (is_bridge[edge_id] || component_[to] != -1) continue;
                    component_[to] = component_id;
                    stack.push_back(to);
                }
            }
        }

        tree_.assign(groups_.size(), {});
        const auto& edges = lowlink.edges();
        for (const int edge_id : lowlink.bridge_ids()) {
            const auto [u, v] = edges[edge_id];
            const int a = component_[u];
            const int b = component_[v];
            tree_[a].push_back(b);
            tree_[b].push_back(a);
        }
    }

    int operator[](int vertex) const { return component_[vertex]; }
    int size() const { return static_cast<int>(groups_.size()); }
    const vector<int>& component_ids() const { return component_; }
    const vector<vector<int>>& groups() const { return groups_; }
    const vector<vector<int>>& bridge_tree() const { return tree_; }

private:
    vector<int> component_;
    vector<vector<int>> groups_;
    vector<vector<int>> tree_;
};
