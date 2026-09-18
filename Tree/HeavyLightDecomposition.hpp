#pragma once

#include "../template.hpp"

struct HeavyLightSegment {
    int left;
    int right;
    bool reversed;
};

class HeavyLightDecomposition {
public:
    explicit HeavyLightDecomposition(const Graph& graph, int root = 0)
        : n_(static_cast<int>(graph.size())),
          parent_(n_, -1),
          depth_(n_, 0),
          subtree_size_(n_, 1),
          heavy_child_(n_, -1),
          head_(n_, -1),
          position_(n_, -1),
          vertex_at_(n_, -1) {
        if (n_ == 0) return;
        assert(0 <= root && root < n_);

        vector<int> order;
        order.reserve(n_);
        vector<int> stack{root};
        parent_[root] = root;
        while (!stack.empty()) {
            const int vertex = stack.back();
            stack.pop_back();
            order.push_back(vertex);
            for (const ll next_value : graph[vertex]) {
                const int next = static_cast<int>(next_value);
                if (next == parent_[vertex]) continue;
                assert(parent_[next] == -1 && "HLD input must be a tree");
                parent_[next] = vertex;
                depth_[next] = depth_[vertex] + 1;
                stack.push_back(next);
            }
        }
        assert(static_cast<int>(order.size()) == n_ &&
               "HLD input must be connected");

        for (auto iterator = order.rbegin(); iterator != order.rend(); ++iterator) {
            const int vertex = *iterator;
            int largest_size = 0;
            for (const ll next_value : graph[vertex]) {
                const int next = static_cast<int>(next_value);
                if (parent_[next] != vertex) continue;
                subtree_size_[vertex] += subtree_size_[next];
                if (subtree_size_[next] > largest_size) {
                    largest_size = subtree_size_[next];
                    heavy_child_[vertex] = next;
                }
            }
        }

        int timer = 0;
        vector<pair<int, int>> pending{{root, root}};
        while (!pending.empty()) {
            const auto [chain_start, chain_head] = pending.back();
            pending.pop_back();
            for (int vertex = chain_start; vertex != -1;
                 vertex = heavy_child_[vertex]) {
                head_[vertex] = chain_head;
                position_[vertex] = timer;
                vertex_at_[timer++] = vertex;

                vector<int> light_children;
                for (const ll next_value : graph[vertex]) {
                    const int next = static_cast<int>(next_value);
                    if (parent_[next] == vertex && next != heavy_child_[vertex]) {
                        light_children.push_back(next);
                    }
                }
                for (auto iterator = light_children.rbegin();
                     iterator != light_children.rend(); ++iterator) {
                    pending.push_back({*iterator, *iterator});
                }
            }
        }
    }

    int size() const { return n_; }
    int parent(int vertex) const { return parent_[vertex]; }
    int depth(int vertex) const { return depth_[vertex]; }
    int position(int vertex) const { return position_[vertex]; }
    int vertex_at(int position) const { return vertex_at_[position]; }

    pair<int, int> subtree(int vertex, bool edge_mode = false) const {
        return {position_[vertex] + static_cast<int>(edge_mode),
                position_[vertex] + subtree_size_[vertex]};
    }

    int lca(int left, int right) const {
        while (head_[left] != head_[right]) {
            if (depth_[head_[left]] > depth_[head_[right]]) {
                left = parent_[head_[left]];
            } else {
                right = parent_[head_[right]];
            }
        }
        return depth_[left] < depth_[right] ? left : right;
    }

    int distance(int left, int right) const {
        const int ancestor = lca(left, right);
        return depth_[left] + depth_[right] - 2 * depth_[ancestor];
    }

    vector<HeavyLightSegment> path_segments(int from,
                                            int to,
                                            bool edge_mode = false) const {
        vector<HeavyLightSegment> front;
        vector<HeavyLightSegment> back;
        while (head_[from] != head_[to]) {
            if (depth_[head_[from]] >= depth_[head_[to]]) {
                front.push_back(
                    {position_[head_[from]], position_[from] + 1, true});
                from = parent_[head_[from]];
            } else {
                back.push_back(
                    {position_[head_[to]], position_[to] + 1, false});
                to = parent_[head_[to]];
            }
        }

        if (depth_[from] >= depth_[to]) {
            const int left = position_[to] + static_cast<int>(edge_mode);
            if (left < position_[from] + 1) {
                front.push_back({left, position_[from] + 1, true});
            }
        } else {
            const int left = position_[from] + static_cast<int>(edge_mode);
            if (left < position_[to] + 1) {
                back.push_back({left, position_[to] + 1, false});
            }
        }

        reverse(back.begin(), back.end());
        front.insert(front.end(), back.begin(), back.end());
        return front;
    }

    template <class Function>
    void for_each_path(int from,
                       int to,
                       Function function,
                       bool edge_mode = false) const {
        for (const auto& segment : path_segments(from, to, edge_mode)) {
            function(segment.left, segment.right);
        }
    }

private:
    int n_;
    vector<int> parent_;
    vector<int> depth_;
    vector<int> subtree_size_;
    vector<int> heavy_child_;
    vector<int> head_;
    vector<int> position_;
    vector<int> vertex_at_;
};
